#pragma once

#ifndef _UDP_CACHE_CONFIG_H_
#define _UDP_CACHE_CONFIG_H_


#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0600
#endif

#ifndef _STRING_
	#include <iostream>
#endif

#ifndef _STRING_
	#include <string>
#endif

#ifndef _VECTOR_
	#include <vector>
#endif

#ifndef _CTIME_
	#include <ctime>
#endif

#ifndef BOOST_ASIO_HPP
	#include <boost/asio.hpp>
#endif

#ifndef BOOST_ARRAY_HPP
	#include <boost/array.hpp>
#endif

#ifndef BOOST_BIND_HPP_INCLUDED
	#include <boost/bind.hpp>
#endif

#ifndef BOOST_SHARED_PTR_HPP_INCLUDED
	#include <boost/shared_ptr.hpp>
#endif

#ifndef BOOST_THREAD_THREAD_HPP
	#include <boost/thread.hpp>
#endif

#ifndef BOOST_THREAD_MUTEX_HPP
	#include <boost/thread/mutex.hpp>
#endif

namespace UdpCache
{
	using namespace boost::asio;
	using ip::udp;


	const std::string CACHESVRIP = "127.0.0.1";	// �����ַ
	const int CACHESVRPROT = 2369;				// ����˿�
	const int CACHECliPROT = CACHESVRPROT + 5;	// �ͻ��Ⱥ�˿�

	static const int MAXCNT = 3120;				// ��󻺴����� (Լռ4MB�ڴ�)
	static const int CMDLEN = 256;				// �ͻ�->����ͨѶ�����ߴ�
	static const int PAKLEN = 1400;				// ����->�ͻ�ÿ�������ߴ�
	static const int INFLIMIT = PAKLEN - 8;		// ����->�ͻ�ÿ��Ϣ���ߴ�

	static const BYTE cmd0 = 0;					// �ͻ�->����, ֪ͨ�ɹ�
	static const BYTE cmd1 = 1;					// �ͻ�->����, ֪ͨ����
	static const BYTE cmd2 = 2;					// �ͻ�->����, ֪ͨ�ط�

/* ���¶������������й� */	
	static const BYTE OPUKNOW = 0;				// ����-�ͻ�, OP ����δ����
	static const BYTE OPGAME  = 2;				// ����-�ͻ�, OP ���� games
	static const BYTE OPUIGM  = 3;				// ����-�ͻ�, OP ���� ingame
	static const BYTE OPUSER  = 4;				// ����-�ͻ�, OP ���� player

	static const BYTE SOPUKNW = 0;				// ����-�ͻ�, SUB OP ����δ����
	static const BYTE SOPINST = 1;				// ����-�ͻ�, SUB OP ����INSERT
	static const BYTE SOPUPDT = 2;				// ����-�ͻ�, SUB OP ����UPDATE


	static const int STATEBASE = 50;			// ״̬��׼
	static const int STATE1 = STATEBASE + 1;	// �������, ��δ������Ϸ
	static const int STATE2 = STATEBASE + 2;	// ������Ϸ, ��δ�������
	static const int STATE3 = STATEBASE + 3;	// ������Ϸ, ������Ϸ����
	static const int STATE4 = STATEBASE + 4;	// ��ʼ��Ϸ
	static const int STATE5 = STATEBASE + 5;	// �����������, ��δ������Ϸ
	static const int STATE6 = STATEBASE + 6;	// �����������, ������Ϸ
	static const int STATE7 = STATEBASE + 7;	// ������Ϸ���, ��¼����ʱ��

	static const int STATEBASEEXIT = STATEBASE + 100;	// �뿪��״̬��׼
	static const int STATEEXIT1 = STATEBASEEXIT + 1;	// ��Ϸδ���ؾ��뿪
	static const int STATEEXIT2 = STATEBASEEXIT + 2;	// ��Ϸ�ֳ�ʤ�����뿪


	struct udpcommand
	{
		BYTE length;
		BYTE command;
		char Param[CMDLEN - 2];
	};

	struct udpdata
	{
		BYTE OP;
		BYTE SUBOP;
		WORD LEN;
		char DATA[INFLIMIT];
	};

	struct udptrans
	{
		WORD size;
		WORD count;
		char data[sizeof(udpdata)];
	};

	static std::string IntToStr(int i)
	{
		char buf[10];
		itoa(i, buf, 10);
		return std::string(buf);
	}

	//static int CommaSplit(std::string& str, std::string interval, std::vector<std::string>& out)
	//{
	//	std::string comma = interval;
	//	std::string::size_type pos;
	//	str += comma;
	//	int size = str.length();
	//	int ret = 0;

	//	for(int i = 0; i < size; i++)
	//	{
	//		pos = str.find(comma,i);
	//		if((int)pos < size)
	//		{
	//			std::string s = str.substr(i, pos - i);
	//			out.push_back(s);
	//			ret++;
	//			i = pos + comma.length() - 1;
	//		}
	//	}
	//	return ret;
	//}

	static int ParseUDPData(udpdata* data, int size, std::vector<udpdata*>& out)
	{
		int length = 0;
		int result = 0;
		udpdata* dt = data;

		while(length < size)
		{
			udpdata* udt = new udpdata;
			memcpy(udt, dt, dt->LEN);
			length += dt->LEN;
			dt = (udpdata*)((int)dt + dt->LEN);			
			out.push_back(udt);
			result++;
		}
		return result;
	}

	// ��ʽ����� ��ȷ���� or ��ȷ������
	const char fmtsec[] = "%02d-%02d-%02d %02d:%02d:%02d";
	//const char fmtmisec[] = "%02d-%02d-%02d %02d:%02d:%02d.%03d";
	//static std::string DataTimeFormat(bool ms_precision = false)
	//{
	//	char buf[128] = {0};
	//	SYSTEMTIME t = {0};
	//	GetLocalTime(&t);
	//	if(ms_precision)
	//		sprintf_s(buf, 128, fmtmisec, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	//	else
	//		sprintf_s(buf, 128, fmtsec, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	//	return std::string(buf);
	//}

	// ֱ�Ӱ� time_t �ı���ʱ������ֵת�����ַ����
	// ������Ҫ����ʱ�䴮���� FormatTtdt() ���н���
	static std::string DataTimeToString()
	{
		char buf[128] = {0};
		time_t now = time(0);
		sprintf_s(buf, 128, "%d", now);
		return std::string(buf);
	}

	// �� time_t �ַ���ת��Ϊ��ʽ��ʱ�䴮��� (Format TimeStr To DateTime)
	static std::string FormatTtdt(std::string& time)
	{
		char buf[128] = {0};
		time_t t = (time_t)atoi(time.c_str());
		tm tmpTM = *localtime(&t);
		sprintf_s(buf, 128, fmtsec, tmpTM.tm_year + 1900, tmpTM.tm_mon + 1, tmpTM.tm_mday, tmpTM.tm_hour, tmpTM.tm_min, tmpTM.tm_sec);
		return std::string(buf);
	}
}

#endif