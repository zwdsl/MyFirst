
#ifndef __PUBLIC_AGSESSIONID_H__
#define __PUBLIC_AGSESSIONID_H__


#include <string>
#include <vector>
#include <map>
#include "../public/typedefine.h"
#include "biostream.h"

#if !defined(WIN32) && !defined(WIN64)
#include <uuid/uuid.h>
#else
#include <ObjBase.h>
#endif

typedef		unsigned char	AGSESSIONID[16];

const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class CImbaUUID
{
private:
//added by wuhongfang_20080804, begin
#if !defined(WIN32) && !defined(WIN64)
	typedef struct {
		unsigned int  Data1;
		unsigned short Data2;
		unsigned short Data3;
		byte           Data4[8];
	} GUID;

	typedef unsigned char uuid_t[16];
#endif
//end

	typedef std::vector<std::string> stringarray;
	class CSplitString
	{
	private:
		stringarray m_list;
		std::string m_string;
	public:
		void Split(const char *str, const char *spt = ",")
		{
			m_string = str;
			std::string st = str;
			std::string sp = spt;
			size_t spl = sp.length();
			m_list.clear();
			int pos;
			pos = (int)st.find(spt);
			while (pos != -1)
			{
				m_list.push_back(st.substr(0,pos));
				st.erase(0,pos+spl);
				pos = (int)st.find(spt);
			}
			m_list.push_back(st);
		}

		int getCount()
		{
			return (int)m_list.size();
		};

		std::string& get(int idx)
		{
			return m_list[idx];
		}
	};
private:
	AGSESSIONID m_sessionid;
	DWORD HexToDWORD(std::string hex) const
	{
		DWORD retval = 0;
		std::map<char, DWORD> hm;
		hm['0'] = 0;
		hm['1'] = 1;
		hm['2'] = 2;
		hm['3'] = 3;
		hm['4'] = 4;
		hm['5'] = 5;
		hm['6'] = 6;
		hm['7'] = 7;
		hm['8'] = 8;
		hm['9'] = 9;
		hm['A'] = 10;
		hm['B'] = 11;
		hm['C'] = 12;
		hm['D'] = 13;
		hm['E'] = 14;
		hm['F'] = 15;
		hm['a'] = 10;
		hm['b'] = 11;
		hm['c'] = 12;
		hm['d'] = 13;
		hm['e'] = 14;
		hm['f'] = 15;
#if defined WIN32 || defined WIN64
		int j = min((int)hex.size(),8);
#else
		int j = (std::min)((int)hex.size(),8);
#endif
		for (int i=0; i<j; i++)
		{
			retval *= 16;
			retval += hm[hex[i]];
		}
		return retval;
	}
	void string_replace(std::string & strBig, const std::string & strsrc, const std::string &strdst)
	{
		std::string::size_type pos=0;
		std::string::size_type srclen=strsrc.size();
		std::string::size_type dstlen=strdst.size();
		while( (pos=strBig.find(strsrc, pos)) != std::string::npos)
		{
			strBig.replace(pos, srclen, strdst);
			pos += dstlen;
		}
	}
	bool StrToGUID(std::string hex, AGSESSIONID sid)
	{
		if (hex.size() != 38)
			return false;
		if (hex[0] != '{' || hex[37] != '}')
			return false;
		string_replace(hex,"{","");
		string_replace(hex,"}","");
		size_t i;//int i;
		for (i=0; i<hex.size(); i++)
			if (!((hex[i]>='0' && hex[i]<='9') ||
				(hex[i]>='a' && hex[i]<='f') ||
				(hex[i]>='A' && hex[i]<='F') ||
				(hex[i] == '-')))
				return false;
		CSplitString spltstr;
		spltstr.Split(hex.c_str(),"-");
		if (spltstr.getCount() != 5)
			return false;
		if (spltstr.get(0).size() != 8)
			return false;
		if (spltstr.get(1).size() != 4)
			return false;
		if (spltstr.get(2).size() != 4)
			return false;
		if (spltstr.get(3).size() != 4)
			return false;
		if (spltstr.get(4).size() != 12)
			return false;
		GUID guid;
		guid.Data1 = HexToDWORD(spltstr.get(0));
		guid.Data2 = (unsigned short)HexToDWORD(spltstr.get(1));
		guid.Data3 = (unsigned short)HexToDWORD(spltstr.get(2));
		std::string hs;
		hs = spltstr.get(3)[0];
		hs += spltstr.get(3)[1];
		guid.Data4[0] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(3)[2];
		hs += spltstr.get(3)[3];
		guid.Data4[1] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[0];
		hs += spltstr.get(4)[1];
		guid.Data4[2] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[2];
		hs += spltstr.get(4)[3];
		guid.Data4[3] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[4];
		hs += spltstr.get(4)[5];
		guid.Data4[4] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[6];
		hs += spltstr.get(4)[7];
		guid.Data4[5] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[8];
		hs += spltstr.get(4)[9];
		guid.Data4[6] = (unsigned char)HexToDWORD(hs);
		hs = spltstr.get(4)[10];
		hs += spltstr.get(4)[11];
		guid.Data4[7] = (unsigned char)HexToDWORD(hs);
		memcpy(sid, &guid, sizeof(AGSESSIONID));
		return true;
	}
	int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen) const
	{
		unsigned char c1, c2, c3;    // 输入缓冲区读出3个字节
		int nDstLen = 0;             // 输出的字符计数
		int nLineLen = 0;            // 输出的行长度计数
		int nDiv = nSrcLen / 3;      // 输入数据长度除以3得到的倍数
		int nMod = nSrcLen % 3;      // 输入数据长度除以3得到的余数

		// 每次取3个字节，编码成4个字符
		for (int i = 0; i < nDiv; i ++)
		{
			// 取3个字节
			c1 = *pSrc++;
			c2 = *pSrc++;
			c3 = *pSrc++;

			// 编码成4个字符
			*pDst++ = EnBase64Tab[c1 >> 2];
			*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
			*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
			*pDst++ = EnBase64Tab[c3 & 0x3f];
			nLineLen += 4;
			nDstLen += 4;

			// 输出换行？
			//if (nLineLen > nMaxLineLen - 4)
			//{
			//	*pDst++ = '\r';
			//	*pDst++ = '\n';
			//	nLineLen = 0;
			//	nDstLen += 2;
			//}
		}

		// 编码余下的字节
		if (nMod == 1)
		{
			c1 = *pSrc++;
			*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
			*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
			*pDst++ = '=';
			*pDst++ = '=';
			nLineLen += 4;
			nDstLen += 4;
		}
		else if (nMod == 2)
		{
			c1 = *pSrc++;
			c2 = *pSrc++;
			*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
			*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
			*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
			*pDst++ = '=';
			nDstLen += 4;
		}

		// 输出加个结束符
		*pDst = '\0';

		return nDstLen;
	}
public:
	CImbaUUID()
	{
		//modified by wuhongfang_20080804, begin
		//ZeroMemory(m_sessionid, sizeof(AGSESSIONID));
		memset(m_sessionid, 0, sizeof(AGSESSIONID));
		//end
	}
	CImbaUUID(const CImbaUUID& asi)
	{
		memcpy(m_sessionid, asi.ID(), sizeof(AGSESSIONID));
	}
	CImbaUUID(const AGSESSIONID& asi)
	{
		memcpy(m_sessionid, asi, sizeof(AGSESSIONID));
	}
	CImbaUUID(const char* ssi)
	{
		//modified by wuhongfang_20080804, begin
		//ZeroMemory(m_sessionid, sizeof(AGSESSIONID));
		memset(m_sessionid, 0, sizeof(AGSESSIONID));
		//end

		StrToGUID(ssi, m_sessionid);
	}
	void Empty()
	{
		//modified by wuhongfang_20080804, begin
		//ZeroMemory(m_sessionid, sizeof(AGSESSIONID));
		memset(m_sessionid, 0, sizeof(AGSESSIONID));
		//end
	}
	bool IsValid() const
	{
		bool retval = false;
		for (size_t i=0; i<sizeof(AGSESSIONID); i++)
			if (m_sessionid[i] != 0)
			{
				retval = true;
				break;
			}
		return retval;
	}
	bool Generate()
	{
		//modified by wuhongfang_20080804, begin
		//GUID guid;
		//if (S_OK == ::CoCreateGuid(&guid))
		//{
		//	memcpy(m_sessionid,&guid,sizeof(AGSESSIONID));
		//	return true;
		//}
		//else
		//	return false;
#if defined(WIN32) || defined(WIN64)
		GUID guid;
		if (S_OK == ::CoCreateGuid(&guid))
		{
			memcpy(m_sessionid,&guid,sizeof(AGSESSIONID));
			return true;
		}
		else
			return false;
#else
		uuid_t uuid;
		uuid_generate(uuid);
		memcpy(m_sessionid, &uuid, sizeof(AGSESSIONID));
		//memcpy(guid.Data1, uuid, sizeof(guid.Data1));	//4 bytes
		//memcpy(guid.Data2, uuid + 4, sizeof(guid.Data2));	//2 bytes
		//memcpy(guid.Data3, uuid + 6, sizeof(guid.Data3));	//2 bytes
		//memcpy(guid.Data4, uuid + 8, sizeof(guid.Data4));	//8 bytes

		return true;
#endif
		//end
	}

	const AGSESSIONID& ID() const
	{
		return m_sessionid;
	}

	std::string StrID() const
	{
		char buf[64] = {0};
		GUID guid;
		memcpy(&guid, m_sessionid, sizeof(AGSESSIONID));
		//modified by wuhongfang_20080804, begin
#if defined(WIN32) || defined(WIN64)
		_snprintf(buf, sizeof(buf)
			, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
		);
#else
		snprintf(buf, sizeof(buf)
			, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
		);
#endif
		//end
		return std::string(buf);
	}

	std::string Base64StrID(int maxlen = 0) const
	{
		char buf[64];
		/*int dstlen = */EncodeBase64((const unsigned char*)m_sessionid, buf, sizeof(AGSESSIONID));
		if (maxlen>0 && maxlen<(int)sizeof(buf))
			buf[maxlen] = 0;
		return buf;
	}

	DWORD LongID( void ) const
	{
		char buf[64];

#if defined(WIN32) || defined(WIN64)
		_snprintf(buf, sizeof(buf)
			, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, m_sessionid[0], m_sessionid[1], m_sessionid[2], m_sessionid[3]
			, m_sessionid[4], m_sessionid[5], m_sessionid[6], m_sessionid[7]
			, m_sessionid[8], m_sessionid[9], m_sessionid[10], m_sessionid[11]
			, m_sessionid[12], m_sessionid[13], m_sessionid[14], m_sessionid[15]
		);
#else
		snprintf(buf, sizeof(buf)
			, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, m_sessionid[0], m_sessionid[1], m_sessionid[2], m_sessionid[3]
			, m_sessionid[4], m_sessionid[5], m_sessionid[6], m_sessionid[7]
			, m_sessionid[8], m_sessionid[9], m_sessionid[10], m_sessionid[11]
			, m_sessionid[12], m_sessionid[13], m_sessionid[14], m_sessionid[15]
		);
#endif

		return HexToDWORD( buf );
	}

	const CImbaUUID& operator=(const CImbaUUID& asi)
	{
		if ( this == &asi )
		{
			return *this;
		}

		memcpy(m_sessionid, asi.ID(), sizeof(AGSESSIONID));
		return *this;
	}

	const CImbaUUID& operator=(const AGSESSIONID& asi)
	{
		memcpy(m_sessionid, asi, sizeof(AGSESSIONID));
		return *this;
	}

	const CImbaUUID& operator=(const char* ssi)
	{
		StrToGUID(ssi, m_sessionid);
		return *this;
	}

	bool operator==(const CImbaUUID& asi) const
	{
		bool retval = true;
		for (size_t i=0; i<sizeof(AGSESSIONID); i++)
			if (m_sessionid[i] != asi.ID()[i])
			{
				retval = false;
				break;
			}
		return retval;
	}

	bool operator==(const AGSESSIONID& asi) const
	{
		bool retval = true;
		for (size_t i=0; i<sizeof(AGSESSIONID); i++)
			if (m_sessionid[i] != asi[i])
			{
				retval = false;
				break;
			}
		return retval;
	}

	//bool operator==(const char* ssi) const
	//{
	//	bool retval = true;
	//	AGSESSIONID isi;
	//	if (!StrToGUID(ssi,isi))
	//		retval = false;
	//	if (retval)
	//	{
	//		for (int i=0; i<sizeof(AGSESSIONID); i++)
	//			if (m_sessionid[i] != isi[i])
	//			{
	//				retval = false;
	//				break;
	//			}
	//	}
	//	return retval;
	//}

	bool operator<(const CImbaUUID& asi) const
	{
		bool retval = false;
		for (size_t i=0; i<sizeof(AGSESSIONID); i++)
			if(m_sessionid[i] != asi.ID()[i])
			{
				if (m_sessionid[i] < asi.ID()[i])
					retval = true;
				break;
			}
		return retval;

	}

	bool operator<(const AGSESSIONID& asi) const
	{
		bool retval = false;
		for (size_t i=0; i<sizeof(AGSESSIONID); i++)
			if (m_sessionid[i] != asi[i])
			{
				if (m_sessionid[i] < asi[i])
					retval = true;
				break;
			}
		return retval;
	}

	//bool operator<(const char* ssi) const
	//{
	//	bool retval = false;
	//	bool ischange = true;
	//	AGSESSIONID isi;
	//	if (!StrToGUID(ssi,isi))
	//		ischange = false;
	//	if (ischange)
	//	{
	//		for (int i=0; i<sizeof(AGSESSIONID); i++)
	//			if (m_sessionid[i] < isi[i])
	//			{
	//				retval = true;
	//				break;
	//			}
	//	}
	//	return retval;
	//}

	friend bostream& operator<<(bostream& bos, const CImbaUUID& msg)
	{
		bos.write( (const char*)msg.m_sessionid, sizeof(msg.m_sessionid) );

		return bos;
	}

	friend bistream& operator>>(bistream& bis, CImbaUUID& msg)
	{
		msg.Empty();
		bis.read( (char*)msg.m_sessionid, sizeof(msg.m_sessionid) );

		return bis;
	}
};

#endif // __PUBLIC_AGSESSIONID_H__
