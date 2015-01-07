#pragma once

#ifndef _UDP_CACHE_SERVICE_H_
#define _UDP_CACHE_SERVICE_H_

#include "udpcache.h"


/*
	ͨ�� SendUDPMessage() ������ CacheServer �ﶪ��Ҫ���⴫�����Ϣ
	CacheServer �����������⴫�ݶ���������Ϣ

	ֻ�е��ⲿ������Ϣ�ķ������� CacheServer ������cmd0-cmd2�������
	CacheServer ��������������������ķ��������ͻ������Ϣ

	MAXCNT = 33200 ��������󻺴����Ϣ����, ������, ������Ϣ������
	INFLIMIT = 1392 ������ÿ����Ϣ��󳤶�, �������ȵ���Ϣֱ�ӷ���

	CacheServer ������ÿ�� UDP ����������� PAKLEN = 1400 ��, ̫�����Ϣ�ᱻ��С�� 1400 ��
	CacheServer �����ڶ��߳�״̬, ���� UDP ��ͬʱ�������ⲿͨ��
	SendUDPMessage() ������ CacheServer �ﶪ��Ϣ

	cmd0 ��ʾ�ϴν���������ȷ, ���������ݴ���
	cmd1 ��ʾ�������ϴν������ݵ���ȷ���, ֱ�ӽ��������ݴ���
	cmd2 ��ʾ�ϴν������ݴ���, �������·���һ��

	ʹ�������, ֻ��Ҫ���ñ���Ԫ, ����Ҫ�ĵط�д���´��뼴�ɷ���һ�� "Send a message!" ����Ϣ��:
	cacheUDP.Send("Send a message!");
	���е� cacheUDP ��������Ϊ��̬����, ��ģ���ʼ����ʱ���Զ�����

	����δ��, ����Ĵ��� �� ���� wangxinyue@gameabc.com [2013-01-18 18:00]
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
				  // ������ַ���
				  if(info.length() >= INFLIMIT)
					  return false;

				  m_lock.lock();
					  // ������󻺴�����
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
							// �ȷ��͵�
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

						// ҲҪ���͵�
						socket_.async_send_to(boost::asio::buffer(senddata, data.size), remote_endpoint,
							boost::bind(&CacheServer::handle_send, this, boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
					}

					// ���Ǳ����
					//Sleep(20);
				}

				// ���Ϳ����ݰ����һ�δ���Ự�ڽ���
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
						case cmd0:							// ǰһ�δ�����ȷ���, ���������������
						case cmd1:							// ǰһ�δ���û����ȷ���, �������ٴδ���ǰһ������, ���������������
							for(std::vector<UdpCache::udpdata*> :: iterator i = sendbuff.begin(); i < sendbuff.end(); i++)
								delete *i;
							sendbuff.clear();
							m_lock.lock();
								sendbuff.swap(inbuff);
							m_lock.unlock();
							doSend();
							break;
						case cmd2:							// ǰһ�δ���û����ȷ���, �������������ٴη����ϴ�����
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
				//	std::cout << "���ݰ��ߴ�: [" << bytes_trans << "] �Ѿ�����"<< std::endl;
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
				// �Ѿ����ֶ������͸�ʽ����Ϊ
				if(NULL != SendServer)
					return SendServer->SendUDP(op, subop, msg);
				return false;
			}
			bool Send(std::string msg)
			{
				// δ�������͵� messages ���
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