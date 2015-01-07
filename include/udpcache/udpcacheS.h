#pragma once

#ifndef _UDP_CACHE_SERVICE_H_
#define _UDP_CACHE_SERVICE_H_

#include "udpcache.h"


/*
	通过 SendUDPMessage() 函数向 CacheServer 里丢需要往外传输的信息
	CacheServer 并不马上向外传递丢进来的信息

	只有当外部接收消息的服务器向 CacheServer 发送了cmd0-cmd2的命令后
	CacheServer 服务器才向发送命令过来的服务器发送缓存的信息

	MAXCNT = 33200 定义了最大缓存的信息数量, 超过后, 后续信息将丢弃
	INFLIMIT = 1392 定义了每条信息最大长度, 超过长度的信息直接放弃

	CacheServer 发出的每个 UDP 包最大限制在 PAKLEN = 1400 内, 太大的信息会被拆小到 1400 内
	CacheServer 工作在多线程状态, 发出 UDP 的同时不妨碍外部通过
	SendUDPMessage() 函数向 CacheServer 里丢信息

	cmd0 表示上次接收数据正确, 继续新数据传输
	cmd1 表示不关心上次接收数据的正确与否, 直接进行新数据传输
	cmd2 表示上次接收数据错误, 请求重新发送一次

	使用这个类, 只需要引用本单元, 在需要的地方写如下代码即可发出一个 "Send a message!" 的信息包:
	cacheUDP.Send("Send a message!");
	其中的 cacheUDP 对象被声明为静态对象, 在模块初始化的时候自动生成

	其他未尽, 请查阅代码 或 函致 wangxinyue@gameabc.com [2013-01-18 18:00]
*/

namespace UdpCache
{
	namespace Server
	{

		class CacheServer
		{
		public:
			CacheServer(boost::asio::io_service& io_service) :
			  socket_(io_service, udp::endpoint(udp::v4(), CACHESVRPROT))
			  {
				  inbuff.clear();
				  sendbuff.clear();
				  start_receive();
				  isrun = true;
			  }
			  virtual ~CacheServer()
			  {
				  for(std::vector<UdpCache::udpdata*> :: iterator i = sendbuff.begin(); i < sendbuff.end(); i++)
					  delete *i;
				  for(std::vector<UdpCache::udpdata*> :: iterator i = inbuff.begin(); i < inbuff.end(); i++)
					  delete *i;
			  }
			  bool SendUDP(int op, int subop, std::string info)
			  {
				  // 限制最长字符串
				  if(info.length() >= INFLIMIT)
					  return false;

				  m_lock.lock();
					  // 限制最大缓存数量
					  if(inbuff.size() > MAXCNT)
					  {
						  m_lock.unlock();
						  return false;
					  }
					  int len = info.length();
					  udpdata* tmpdata = new udpdata;
					  tmpdata->OP = (BYTE)op;					  
					  tmpdata->SUBOP = (BYTE)subop;
					  tmpdata->LEN = (WORD)len + 4;
					  memcpy(tmpdata->DATA, info.c_str(), len);

					  inbuff.push_back(tmpdata);			  
				  m_lock.unlock();
				  return true;
			  }
			  void Stop()
			  {
				  if(socket_.is_open())
					  socket_.close();
				  isrun = false;
			  }
			  bool IsRun()
			  {
				  return isrun;
			  }
		private:
			void start_receive()
			{
				socket_.async_receive_from(
					boost::asio::buffer(recv_buffer), remote_endpoint,
					boost::bind(&CacheServer::handle_receive, this, 
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
			void doSend()
			{
				if(!sendbuff.empty())
				{
					int idxs = 0;
					udptrans data = {0};
					int cppoint = (int)(data.data);
					int flen = 0;
					for( std::vector<udpdata*> :: iterator i = sendbuff.begin(); i < sendbuff.end();)
					{						
						udpdata* s = *i;
						int tmplen = s->LEN;
						if( (tmplen + flen) < (PAKLEN - 4) )
						{
							memcpy((void *)cppoint, s, tmplen);
							cppoint += tmplen;
							flen += tmplen;
							idxs++;
							i++;
						}
						else
						{
							data.size = flen + 4;
							data.count = idxs;
							char* senddata = (char*)&data;					
							// 先发送掉
							socket_.async_send_to(boost::asio::buffer(senddata, data.size), remote_endpoint,
								boost::bind(&CacheServer::handle_send, this, boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred));
							idxs = 0;
							flen = 0;
							cppoint = (int)(data.data);
							memset(&data, 0, sizeof(udptrans));
						}
					}

					if(idxs != 0)
					{
						data.size = flen + 4;
						data.count = idxs;
						char* senddata = (char*)&data;

						// 也要发送掉
						socket_.async_send_to(boost::asio::buffer(senddata, data.size), remote_endpoint,
							boost::bind(&CacheServer::handle_send, this, boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
					}

					// 不是必须的
					//Sleep(20);
				}

				// 发送空数据包表达一次传输会话期结束
				udptrans emptypak = {5, 0, {0}};
				socket_.async_send_to(boost::asio::buffer((char*)&emptypak, emptypak.size), remote_endpoint,
					boost::bind(&CacheServer::handle_send, this, boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			}
			void handle_receive(const boost::system::error_code& error,	std::size_t bytes_trans)
			{
				if (!error || bytes_trans > 0)
				{
					udpcommand* buff = (udpcommand *)recv_buffer.c_array();
					if (bytes_trans == buff->length)
					{
						switch(buff->command)
						{
						case cmd0:							// 前一次传输正确完成, 现在申请后续传输
						case cmd1:							// 前一次传输没有正确完成, 但放弃再次传输前一次数据, 现在申请后续传输
							for(std::vector<UdpCache::udpdata*> :: iterator i = sendbuff.begin(); i < sendbuff.end(); i++)
								delete *i;
							sendbuff.clear();
							m_lock.lock();
								sendbuff.swap(inbuff);
							m_lock.unlock();
							doSend();
							break;
						case cmd2:							// 前一次传输没有正确完成, 现在申请重新再次发送上次数据
							doSend();
							break;
						}				
					}
					start_receive();
				}
			}

			void handle_send(const boost::system::error_code& error, std::size_t bytes_trans)
			{
				//if( bytes_trans != 5)
				//	std::cout << "数据包尺寸: [" << bytes_trans << "] 已经发送"<< std::endl;
			}

			std::vector<UdpCache::udpdata*> inbuff;
			std::vector<UdpCache::udpdata*> sendbuff;
			boost::asio::detail::mutex m_lock;

			volatile bool isrun;
			int command;
			udp::socket socket_;
			udp::endpoint remote_endpoint;
			boost::array<char, CMDLEN> recv_buffer;
		};

		class Service
		{
		public:
			static Service& getInstance()
			{
				static Service cacheservice;
				return cacheservice;
			}
			virtual ~Service()
			{
				if(SendServer->IsRun())
					SendServer->Stop();
				WaitForSingleObject(RunEvent, INFINITE);
			}
		public:
			bool SendOP(int op, int subop, std::string msg)
			{
				// 已经部分定义类型格式和行为
				if(NULL != SendServer)
					return SendServer->SendUDP(op, subop, msg);
				return false;
			}
			bool Send(std::string msg)
			{
				// 未定义类型的 messages 输出
				return SendOP(UdpCache::OPUKNOW, UdpCache::SOPUKNW, msg);
			}
		private:
			Service():SendServer(NULL),RunEvent(NULL)
			{
				if(SendServer == NULL)
				{
					RunEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
					boost::thread m_thread(boost::bind(&Service::doRun, this));
					WaitForSingleObject(RunEvent, INFINITE);
					ResetEvent(RunEvent);
				}
			}
			void doRun()
			{
				try
				{
					boost::asio::io_service io_service;
					SendServer = new CacheServer(io_service);
					while(!SendServer->IsRun()){ Sleep(50);}
					SetEvent(RunEvent);
					io_service.run();
					io_service.stop();
					delete SendServer;
					SendServer = NULL;
					SetEvent(RunEvent);
					CloseHandle(RunEvent);
					RunEvent = NULL;
				}
				catch (std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
			}
			CacheServer* SendServer;
			HANDLE RunEvent;
		};
	}
}

static UdpCache::Server::Service& cacheUDP(UdpCache::Server::Service::getInstance());

#endif