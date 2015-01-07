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


	const std::string CACHESVRIP = "127.0.0.1";	// 服务地址
	const int CACHESVRPROT = 2369;				// 服务端口
	const int CACHECliPROT = CACHESVRPROT + 5;	// 客户等候端口

	static const int MAXCNT = 3120;				// 最大缓存条数 (约占4MB内存)
	static const int CMDLEN = 256;				// 客户->服务通讯包最大尺寸
	static const int PAKLEN = 1400;				// 服务->客户每个包最大尺寸
	static const int INFLIMIT = PAKLEN - 8;		// 服务->客户每信息最大尺寸

	static const BYTE cmd0 = 0;					// 客户->服务, 通知成功
	static const BYTE cmd1 = 1;					// 客户->服务, 通知丢弃
	static const BYTE cmd2 = 2;					// 客户->服务, 通知重发

/* 以下定义与具体输出有关 */	
	static const BYTE OPUKNOW = 0;				// 服务-客户, OP 操作未定义
	static const BYTE OPGAME  = 2;				// 服务-客户, OP 操作 games
	static const BYTE OPUIGM  = 3;				// 服务-客户, OP 操作 ingame
	static const BYTE OPUSER  = 4;				// 服务-客户, OP 操作 player

	static const BYTE SOPUKNW = 0;				// 服务-客户, SUB OP 操作未定义
	static const BYTE SOPINST = 1;				// 服务-客户, SUB OP 操作INSERT
	static const BYTE SOPUPDT = 2;				// 服务-客户, SUB OP 操作UPDATE


	static const int STATEBASE = 50;			// 状态基准
	static const int STATE1 = STATEBASE + 1;	// 进入服务, 还未进入游戏
	static const int STATE2 = STATEBASE + 2;	// 进入游戏, 还未进入大厅
	static const int STATE3 = STATEBASE + 3;	// 进入游戏, 进入游戏大厅
	static const int STATE4 = STATEBASE + 4;	// 开始游戏
	static const int STATE5 = STATEBASE + 5;	// 重连进入服务, 还未进入游戏
	static const int STATE6 = STATEBASE + 6;	// 重连进入服务, 进入游戏
	static const int STATE7 = STATEBASE + 7;	// 加载游戏完成, 记录加载时间

	static const int STATEBASEEXIT = STATEBASE + 100;	// 离开的状态基准
	static const int STATEEXIT1 = STATEBASEEXIT + 1;	// 游戏未加载就离开
	static const int STATEEXIT2 = STATEBASEEXIT + 2;	// 游戏分出胜负后离开


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

	// 格式化输出 精确到秒 or 精确到毫秒
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

	// 直接把 time_t 的本地时间数字值转换成字符表达
	// 后期需要日期时间串就用 FormatTtdt() 进行解码
	static std::string DataTimeToString()
	{
		char buf[128] = {0};
		time_t now = time(0);
		sprintf_s(buf, 128, "%d", now);
		return std::string(buf);
	}

	// 把 time_t 字符串转化为格式化时间串输出 (Format TimeStr To DateTime)
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