#pragma once

#ifndef _UDP_CACHE_CLIENT_H_
#define _UDP_CACHE_CLIENT_H_

#include "udpcache.h"


/*
	接口类IProcessData是外部需要实现的函数体, 为接收数据, 需要这个接口类实例
	调用 AddReceiver(IProcessData& cls) 添加数据接收对象后, CacheClient 就可以工作

	外部根据需要, 分别调用 cmd0-cmd2 进行数据申请, CacheClient 会在得到数据后回调
	IProcessData.disposeData(), 这个函数需要把分解处理后的数据条目数量返回, 
	CacheClient 内部会根据这个返回值判断本次收发是否正常, 并将判断后的数据,
	在一个数据接收期完成后(即一个会话期), 调用IProcessData.sessionEnd()把结果返回

	根据返回来的结果, 外部可以继续发送cmd0-cmd2命令进行下一步操作, 发送cmd0-cmd2
	命令使用函数 Recv()

	cmd0 表示上次接收数据正确, 继续新数据传输
	cmd1 表示不关心上次接收数据的正确与否, 直接进行新数据传输
	cmd2 表示上次接收数据错误, 请求重新发送一次

	// 先声明一个接口类的实现
	class myProcessData:public IProcessData
	{
	public:
		myProcessData()
		{
			sessnResult = 1;
			out.clear();
		}
		virtual int disposeData(std::string& data)
		{
			int ret = UdpCache::CommaSplit(data, out);
			// 一次接收完成, 检查数据条数
			//std::cout << out.size() << std::endl;
			return ret;
		}
		virtual void sessionEnd(int errCount)
		{
			if(errCount != 0xFFFFFFFF && sessnResult != 0xFFFFFFFF)
			{
				if(errCount > 0)
				{
					sessnResult = 2;
					out.clear();
					return;
				}
				else
					sessnResult = 0;
			}
			else
				sessnResult = 0xFFFFFFFF;
			// 一次会话完成, 检查数据总条数
			//std::cout << out.size() << std::endl;
			if(out.size() > 0)
			{
				for( std::vector<std::string> :: iterator i = out.begin(); i < out.end(); i++)
					std::cout << *i << std::endl;

				// 准备下一次会话
				out.clear();
			}
		}
		int sessionCtl()
		{
			return sessnResult;
		}
	private:
		int sessnResult;
		std::vector<std::string> out;
	};

	
	// 实例化接口类
	myProcessData myRecv;
	// 作为接收器添加到CacheClient里
	if (AddReceiver(&myRecv))
	{
		// 开始接收数据
		while(IsLoop && myRecv.sessionCtl() != 0xFFFFFFFF)
		{
			switch (myRecv.sessionCtl())
			{
			case 0:
				Recv();
				break;
			case 1:
				Recv(1);
				break;
			case 2:
				Recv(2);
				break;
			}
			Sleep(10);
		}
	}
	// 从CacheClient里脱离
	AddReceiver(NULL);

	其他未尽, 请查阅代码 或 函致 wangxinyue@gameabc.com [2013-01-18 18:00]
*/

// 接口类
class IProcessData
{
public:
	// 参数data是接收到的以分号分隔的数据
	// 返回int报告分解后统计的数据条数
	// 一个会话期, 本函数会被多次调用, int 代表解析后的条目数量, 每次都要返回
	// 一个会话期内, 并不一定会有数据收到
	virtual int disposeData(UdpCache::udpdata* data, int size) = 0;
	// 通知事件, 告知一个会话期结束,
	// 会话期是指发送一个cmd命令到sessionEnd()被调用的一个周期
	// 参数 errCount 返回会话期中错误数据条目数量
	virtual void sessionEnd(int errCount) = 0;
};

namespace UdpCache
{
	namespace Client
	{
		class CacheClient
		{
		public:
			CacheClient(boost::asio::io_service& io_service) :
			  socket_(io_service, udp::endpoint(udp::v4(), CACHECliPROT)), 
				  send_toremote_endpoint(boost::asio::ip::address::from_string(CACHESVRIP), CACHESVRPROT)
		    {
				Errcount = 0;
				command = 1;
				process = NULL;
				recvinte = 300;
				start_receive();
				isrun = true;
		    }
			virtual ~CacheClient()
			{
				if(process)
				{
					int ret = 0xFFFFFFFF;
					process->sessionEnd(ret);
					process = NULL;
				}
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
			bool AddAccept(IProcessData* cls)
			{
				if(cls == NULL) // && process != NULL)
				{
					process = NULL;
					return true;
				}

				if(!process)
				{
					process = cls;
					return true;
				}
				
				return false;
			}
			void CMD0()
			{
				udpcommand cmd0 = {3, 0, {0}};
				command = 0;
				doSend(cmd0);
			}
			void CMD1()
			{
				udpcommand cmd1 = {3, 1, {0}};
				command = 1;
				doSend(cmd1);
			}
			void CMD2()
			{
				udpcommand cmd2 = {3, 2, {0}};
				command = 2;
				doSend(cmd2);
			}
			void SetRecvIntervel(int intrvl)
			{
				if(intrvl > 20)
					recvinte = intrvl;
				else
					recvinte = 20;
			}
		private:
			void doSend(udpcommand& cmd)
			{
				char* senddata = (char*)&cmd;
				socket_.async_send_to(boost::asio::buffer(senddata, cmd.length), send_toremote_endpoint,
					boost::bind(&CacheClient::handle_send, this, boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			}
			void start_receive()
			{
				socket_.async_receive_from(
					boost::asio::buffer(recv_buffer), remote_endpoint,
					boost::bind(&CacheClient::handle_receive, this, 
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
			void handle_receive(const boost::system::error_code& error,	std::size_t bytes_trans)
			{
				if (!error || bytes_trans > 0)
				{
					udptrans* buff = (udptrans *)recv_buffer.c_array();
					if (bytes_trans == buff->size)
					{
						// 收到会话结束的空包
						if(buff->count == 0)
						{
							// 发送给外部处理数据
							// Errcount > 0 不允许数据错误, 就需要发送 CMD2() 命令重新传输数据, 并丢弃会话期间的数据
							// Errcount = 0 表示接收数据正确, 可以发送 CMD0() 继续请求传输新数据
							// 可以允许数据错误的话, 任何时候都可以发送 CMD1() 命令, 继续要求传输数据, 意味着忽略是否有传输错误
							if(process)
								process->sessionEnd(Errcount);

							Sleep(recvinte); // 这个并不是必须的
							Errcount = 0;
						}
						else
						{
							if(process)
							{
								try
								{
									int ncount = process->disposeData((UdpCache::udpdata*)&buff->data, buff->size - 4);
									if(ncount != buff->count)
									{
										Errcount += (buff->count - ncount);
										std::cout << "接收数据缺失: "<< Errcount << std::endl;
									}
								}
								catch (std::exception& e)
								{
									std::cerr << e.what() << std::endl;									
								}
							}
						}
					}
					start_receive();
				}
				else
				{
					std::cout << error.message() << std::endl;
					if(command == 1 && error.value() == 10061)
						start_receive();
				}
			}
			void handle_send(const boost::system::error_code& error, std::size_t bytes_trans)
			{
				//std::cout << bytes_trans << std::endl;
			}

			
			int Errcount;
			int command;
			int recvinte;
			volatile bool isrun;
			udp::socket socket_;
			udp::endpoint send_toremote_endpoint;
			udp::endpoint remote_endpoint;
			boost::array<char, PAKLEN> recv_buffer;
			IProcessData* process;
		};

		static CacheClient* RecvServer = NULL;
		static HANDLE RunEvent = NULL;
		class Clientele
		{
		public:
			static Clientele& getInstance()
			{
				static Clientele* cacheclient = NULL;
				if(cacheclient == NULL)
					cacheclient = new Clientele();
				else
				{
					if(RecvServer == NULL)
					{
						WaitForSingleObject(RunEvent, INFINITE);
						delete cacheclient;
						cacheclient = new Clientele();
					}
				}				

				return *cacheclient;
			}
			virtual ~Clientele()
			{
				if(NULL != RecvServer)
				{
					if(RecvServer->IsRun())
						RecvServer->Stop();
					WaitForSingleObject(RunEvent, INFINITE);
				}
			}
		public:
			bool AddAccept(IProcessData* cls)
			{
				if(NULL != RecvServer)
					return RecvServer->AddAccept(cls);
				else
					return false;
			}
			void Recv()
			{
				if(NULL != RecvServer)
					RecvServer->CMD0();
			}
			void Recv1()
			{
				if(NULL != RecvServer)
					RecvServer->CMD1();
			}
			void ReRecv()
			{
				if(NULL != RecvServer)
					RecvServer->CMD2();
			}
			void SetRecvIntervel(int value)
			{
				if(NULL != RecvServer)
					RecvServer->SetRecvIntervel(value);
			}
		private:
			Clientele()/*:RecvServer(NULL),RunEvent(NULL)*/
			{
				if(RecvServer == NULL)
				{
					RunEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
					boost::thread m_thread(boost::bind(&Clientele::doRun, this));
					WaitForSingleObject(RunEvent, INFINITE);
					ResetEvent(RunEvent);
				}
			}
			void doRun()
			{
				try
				{
					boost::asio::io_service io_service;
					RecvServer = new CacheClient(io_service);
					while(!RecvServer->IsRun()){ Sleep(50);}
					SetEvent(RunEvent);
					io_service.run();
					io_service.stop();
					CacheClient* tmpSvr = RecvServer;
					RecvServer = NULL;
					delete tmpSvr;					
					SetEvent(RunEvent);
					CloseHandle(RunEvent);
					RunEvent = NULL;
				}
				catch (std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
			}
			//static CacheClient* RecvServer;
			//HANDLE RunEvent;
		};
	}
}

// 注意这里 receUDP 没有用 static 限定, 意味着可以被多个 obj 引用独立的静态地址
// 如果链接发生多处被引用的错误报告的话, 需要限定它为 siatic
//UdpCache::Client::Clientele& receUDP(UdpCache::Client::Clientele::getInstance());

void SetIntervel(int value)
{
	UdpCache::Client::Clientele::getInstance().SetRecvIntervel(value);
}

bool AddReceiver(IProcessData* cls)
{
	return UdpCache::Client::Clientele::getInstance().AddAccept(cls);
}

void Recv(int act = UdpCache::cmd0)
{
	switch (act)
	{
	case UdpCache::cmd0:
		return UdpCache::Client::Clientele::getInstance().Recv();
	case UdpCache::cmd1:
		return UdpCache::Client::Clientele::getInstance().Recv1();
	case UdpCache::cmd2:
		return UdpCache::Client::Clientele::getInstance().ReRecv();
	}
}

#endif