#pragma once

#ifndef _UDP_CACHE_CLIENT_H_
#define _UDP_CACHE_CLIENT_H_

#include "udpcache.h"


/*
	�ӿ���IProcessData���ⲿ��Ҫʵ�ֵĺ�����, Ϊ��������, ��Ҫ����ӿ���ʵ��
	���� AddReceiver(IProcessData& cls) ������ݽ��ն����, CacheClient �Ϳ��Թ���

	�ⲿ������Ҫ, �ֱ���� cmd0-cmd2 ������������, CacheClient ���ڵõ����ݺ�ص�
	IProcessData.disposeData(), ���������Ҫ�ѷֽ⴦����������Ŀ��������, 
	CacheClient �ڲ�������������ֵ�жϱ����շ��Ƿ�����, �����жϺ������,
	��һ�����ݽ�������ɺ�(��һ���Ự��), ����IProcessData.sessionEnd()�ѽ������

	���ݷ������Ľ��, �ⲿ���Լ�������cmd0-cmd2���������һ������, ����cmd0-cmd2
	����ʹ�ú��� Recv()

	cmd0 ��ʾ�ϴν���������ȷ, ���������ݴ���
	cmd1 ��ʾ�������ϴν������ݵ���ȷ���, ֱ�ӽ��������ݴ���
	cmd2 ��ʾ�ϴν������ݴ���, �������·���һ��

	// ������һ���ӿ����ʵ��
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
			// һ�ν������, �����������
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
			// һ�λỰ���, �������������
			//std::cout << out.size() << std::endl;
			if(out.size() > 0)
			{
				for( std::vector<std::string> :: iterator i = out.begin(); i < out.end(); i++)
					std::cout << *i << std::endl;

				// ׼����һ�λỰ
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

	
	// ʵ�����ӿ���
	myProcessData myRecv;
	// ��Ϊ��������ӵ�CacheClient��
	if (AddReceiver(&myRecv))
	{
		// ��ʼ��������
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
	// ��CacheClient������
	AddReceiver(NULL);

	����δ��, ����Ĵ��� �� ���� wangxinyue@gameabc.com [2013-01-18 18:00]
*/

// �ӿ���
class IProcessData
{
public:
	// ����data�ǽ��յ����Էֺŷָ�������
	// ����int����ֽ��ͳ�Ƶ���������
	// һ���Ự��, �������ᱻ��ε���, int ������������Ŀ����, ÿ�ζ�Ҫ����
	// һ���Ự����, ����һ�����������յ�
	virtual int disposeData(UdpCache::udpdata* data, int size) = 0;
	// ֪ͨ�¼�, ��֪һ���Ự�ڽ���,
	// �Ự����ָ����һ��cmd���sessionEnd()�����õ�һ������
	// ���� errCount ���ػỰ���д���������Ŀ����
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
						// �յ��Ự�����Ŀհ�
						if(buff->count == 0)
						{
							// ���͸��ⲿ��������
							// Errcount > 0 ���������ݴ���, ����Ҫ���� CMD2() �������´�������, �������Ự�ڼ������
							// Errcount = 0 ��ʾ����������ȷ, ���Է��� CMD0() ����������������
							// �����������ݴ���Ļ�, �κ�ʱ�򶼿��Է��� CMD1() ����, ����Ҫ��������, ��ζ�ź����Ƿ��д������
							if(process)
								process->sessionEnd(Errcount);

							Sleep(recvinte); // ��������Ǳ����
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
										std::cout << "��������ȱʧ: "<< Errcount << std::endl;
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

// ע������ receUDP û���� static �޶�, ��ζ�ſ��Ա���� obj ���ö����ľ�̬��ַ
// ������ӷ����ദ�����õĴ��󱨸�Ļ�, ��Ҫ�޶���Ϊ siatic
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