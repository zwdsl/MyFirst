
#pragma once

#if defined _WINDOWS_ || defined WIN32
	#include <Windows.h>
#else
	#include <iconv.h>
	#include <errno.h>
#endif

#include <string>


class CCodeConv
{
public:
	// GB2312 转为 UTF-8
	static void GB2312ToUTF_8(std::string& pOut, const char *pText, int pLen);
	// UTF-8 转为 GB2312
	static void UTF_8ToGB2312(std::string& pOut, const char *pText, int pLen);

private:
#if defined _WINDOWS_ || defined WIN32
	// 把UTF-8转换成Unicode
	static void UTF_8ToUnicode_WIN(wchar_t* pOut, const char *pText); 
	// Unicode 转换成UTF-8
	static void UnicodeToUTF_8_WIN(char* pOut, wchar_t* pText);
	// 把Unicode 转换成 GB2312
	static void UnicodeToGB2312_WIN(char* pOut, wchar_t uData);
	// GB2312 转换成　Unicode
	static void Gb2312ToUnicode_WIN(wchar_t* pOut, const char *gbBuffer);

	static wchar_t* Utf8ToUnicode(const char* pUtf8);
	static char* UnicodeToUtf8(const wchar_t* pUnicode);

	static char* MbcsToUtf8(const char* pMbcs);
	static char* Utf8ToMbcs(const char* pUtf8);

	static wchar_t* MbcsToUnicode(const char* pMbcs);
	static char*	UnicodeToMbcs(const wchar_t* pUnicode);

#else
	//static int Convert_LIUNX(const char *from_charset, const char *to_charset, const char *inbuf, size_t insize, char *outbuf, size_t outsize);
	static std::string Convert_LIUNX( const char *from_charset, const char *to_charset, const char *inbuf, size_t insize, char *outbuf, size_t outsize );
#endif
};

#if defined _WINDOWS_ || defined WIN32
inline
void CCodeConv::GB2312ToUTF_8(std::string& pOut, const char *pText, int pLen)
{
	char *p = MbcsToUtf8(pText);
	pOut = p;
	delete []p;
	//pOut = MbcsToUtf8(pText);
	return;
	if( pText==0 || pLen<=0 )	return;

	char	buf[4];
	int		nLength = pLen*3;
	char*	rst = new char[nLength+1];

	memset(buf,0,4);
	memset(rst,0,nLength+1);

	int i = 0;
	int j = 0;   
	while(i < pLen)
	{
		//如果是英文直接复制就能
		if( *(pText+i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;

			Gb2312ToUnicode_WIN(&pbuffer, pText+i);
			UnicodeToUTF_8_WIN(buf,	&pbuffer);

			rst[j]		= buf[0];
			rst[j+1]	= buf[1];
			rst[j+2]	= buf[2];   

			j += 3;
			i += 2;
		}
	}
	rst[j] = 0;

	pOut = rst;   
	delete []rst;   
}

inline
void CCodeConv::UTF_8ToGB2312(std::string &pOut, const char *pText, int pLen)
{
	char *p = Utf8ToMbcs(pText);
	pOut = p;
	delete []p;
	return;
	if( pText==0 || pLen<=0 )	return;

	char *	newBuf = new char[pLen+1];
	char	temp[4];

	memset(temp,   0, 4);
	memset(newBuf, 0, pLen+1);

	int i =0;
	int j = 0;

	while(i < pLen)
	{
		if(*(pText+i) >= 0)
		{
			newBuf[j++] = pText[i++];   
		}
		else   
		{
			WCHAR Wtemp;

			UTF_8ToUnicode_WIN(&Wtemp, pText+i);
			UnicodeToGB2312_WIN(temp, Wtemp);

			newBuf[j]	= temp[0];
			newBuf[j+1] = temp[1];

			i += 3;   
			j += 2;   
		}
	}
	newBuf[j] = 0;

	pOut = newBuf;
	delete [] newBuf; 
} 

inline
void CCodeConv::UTF_8ToUnicode_WIN(wchar_t* pOut, const char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

inline
void CCodeConv::UnicodeToUTF_8_WIN(char* pOut,wchar_t* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

inline
void CCodeConv::UnicodeToGB2312_WIN(char* pOut,wchar_t uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	return;
}   

inline
void CCodeConv::Gb2312ToUnicode_WIN(wchar_t* pOut, const char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
	return ;
}
inline
wchar_t* CCodeConv::Utf8ToUnicode(const char* pUtf8)
{
	assert(pUtf8);
	int nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, NULL, 0);

	int nSize = nChar*sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, pwchBuffer, nSize);
	if(nChar == 0)
	{
		delete []pwchBuffer;
		return NULL;
	}

	pwchBuffer[nChar] = 0;
	return pwchBuffer;
}

inline
char* CCodeConv::UnicodeToUtf8(const wchar_t* pUnicode)
{
	assert(pUnicode);
	int nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, 0, 0, 0, 0);

	int nSize = nChar*sizeof(char) + 1;
	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pchBuffer, nSize, 0, 0);
	if( nChar == 0 )
	{
		delete[] pchBuffer;
		return NULL;
	}

	pchBuffer[nChar]= 0;
	return pchBuffer;
}
inline
wchar_t* CCodeConv::MbcsToUnicode(const char* pMbcs)
{
	int codepage = 936;
	int nChar = MultiByteToWideChar(codepage, 0, pMbcs, -1, NULL,0);

	int nSize = nChar * sizeof(wchar_t) + 1;
	wchar_t* pwchBuffer = new wchar_t[nSize];

	nChar = MultiByteToWideChar(codepage, 0, pMbcs, -1, pwchBuffer, nSize);
	if(nChar == 0)
	{
		delete[] pwchBuffer;
		return NULL;
	}

	pwchBuffer[nChar]=0;

	return pwchBuffer;
}
inline
char*	CCodeConv::UnicodeToMbcs(const wchar_t* pUnicode)
{
	int codepage = 936;
	int nChar = WideCharToMultiByte(codepage, 0, pUnicode, -1, 0, 0, 0, 0);
	int nSize = nChar * sizeof(char) + 1;

	char* pchBuffer = new char[nSize];

	nChar = WideCharToMultiByte(codepage, 0, pUnicode, -1, pchBuffer, nSize, 0, 0);
	if( nChar == 0 )
	{
		delete[] pchBuffer;
		return NULL;
	}

	pchBuffer[nChar]= 0;
	return pchBuffer;
}
inline
char* CCodeConv::MbcsToUtf8(const char* pMbcs)
{
	assert(pMbcs);
	wchar_t* pUnicode = MbcsToUnicode(pMbcs);
	char*pRet =  UnicodeToUtf8(pUnicode);
	delete[] pUnicode;
	return pRet;
}
inline
char* CCodeConv::Utf8ToMbcs(const char* pUtf8)
{
	assert(pUtf8);
	wchar_t* pUnicode = Utf8ToUnicode(pUtf8);
	char* pRet = UnicodeToMbcs(pUnicode);
	delete[] pUnicode;
	return pRet;
}

#else
//inline
//int CCodeConv::Convert_LIUNX(const char *from_charset, const char *to_charset, const char *inbuf, size_t insize, char *outbuf, size_t outsize)   
//{
//	iconv_t c_pt;
//	char *pin	= (char*)inbuf;
//	char *pout	= outbuf;		
//
//	c_pt = iconv_open(to_charset, from_charset);
//	if ( c_pt == (iconv_t)(-1) )
//	{
//		fprintf(stderr, "CCodeConv::Convert_LIUNX iconv_open err=%d\n", errno);
//		return -1;
//	}
//
//	size_t len = outsize;
//	if ( iconv(c_pt, &pin, &insize, &pout, &len) == (size_t)(-1) )
//	{
//		fprintf(stderr, "CCodeConv::Convert_LIUNX iconv err=%d\n", errno);
//		iconv_close(c_pt);
//		return -1;
//	}
//
//	if ( iconv_close(c_pt) == -1 )
//	{
//		fprintf(stderr, "CCodeConv::Convert_LIUNX iconv_close err=%d\n", errno);
//	}
//
//	return (outsize-len);
//}
inline
std::string CCodeConv::Convert_LIUNX( const char *from_charset, const char *to_charset, const char *inbuf, size_t insize, char *outbuf, size_t outsize )
{
	std::string result;

	iconv_t cd = iconv_open(to_charset, from_charset);
	if ( (iconv_t)-1 == cd )
	{
		fprintf(stderr, "CCodeConv::Convert_LIUNX iconv_open err=%d\n", errno);
		return result;
	}

	char*  pinbuf		= const_cast< char* >(inbuf);
	size_t inbytesleft	= insize;
	char*  poutbuf		= outbuf;
	size_t outbytesleft = outsize;

	size_t retbytes		= 0;
	int done			= 0;
	int errno_save		= 0;

	while( inbytesleft > 0 && !done )
	{
		poutbuf		= outbuf;
		outbytesleft= outsize;

		retbytes	= iconv( cd, &pinbuf, &inbytesleft, &poutbuf, &outbytesleft );
		errno_save	= errno;

		if ( outbuf != poutbuf ) // we have something to write
		{
			result.append(outbuf, poutbuf-outbuf);
		} 

		if ( retbytes != (size_t)-1 )
		{
			poutbuf		= outbuf;
			outbytesleft= outsize;
			iconv( cd, NULL, NULL, &poutbuf, &outbytesleft );

			if ( outbuf != poutbuf ) // we have something to write
			{
				result.append(outbuf, poutbuf-outbuf);
			}

			errno_save = 0;
			break;
		}

		switch(errno_save)
		{
		case E2BIG: 
			break;
		case EILSEQ:
			errno_save	= 0;
			inbytesleft = insize-(pinbuf-inbuf); // forward one illegal byte
			--inbytesleft;
			++pinbuf;
			break;
		case EINVAL:
			done = 1;
			break;
		default:
			done = 1;
			break; 
		}
	}

	iconv_close(cd);
	errno = errno_save;

	return result;
}

inline
void CCodeConv::GB2312ToUTF_8(std::string& pOut, const char *pText, int pLen)
{
	if( pText==0 || pLen<=0 )	return;

	int		outsize = pLen* 3;
	char*	outbuff	= new char[outsize+1];

	memset(outbuff, 0, outsize+1);

	pOut = Convert_LIUNX( "gbk", "utf-8", pText, pLen, outbuff, outsize);

	delete [] outbuff;
}

inline
void CCodeConv::UTF_8ToGB2312(std::string& pOut, const char *pText, int pLen)
{
	if( pText==0 || pLen<=0 )	return;

	int		outsize = pLen;
	char*	outbuff = new char[pLen+1];

	memset(outbuff, 0, outsize+1);

	pOut = Convert_LIUNX( "utf-8", "gbk", pText, pLen, outbuff, outsize);

	delete [] outbuff;
}

#endif

