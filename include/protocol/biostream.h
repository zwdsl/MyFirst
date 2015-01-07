/*///////////////////////////////////////////////////////////////////////////////////
//�ṩ������������
//bios			����
//	bistream	������
//	bostream	�����
//biosexception	�쳣��
//
//�ṩ������ C �������͵� << �� >> ����������
//���ж�char* ��д�뷽ʽ��MFC��CString���ݡ������е���һ���ֽڵ��޷���ֵС��255ʱ,��ֵ
//��ʾ����ַ������ֽڳ��ȣ������е�һ���ֽڵ��޷���ֵ����255ʱ,������unsigned short
//ֵΪ�ַ������ȣ��ڸ�unsigned short֮��Ϊ�ַ������ݡ���unsigned short==0xffff,������
//���ֽ�ֵ(unsigned long)Ϊ�ַ������ȣ���unsigned long֮�����ַ�������.
//
//bios �ṩ:
//  attach���� �����󶨵��ڴ滺������
//  length���� �������ǰ��ʹ�õĳ���
//  avail����  ��û�������ǰ���õĳ���
//
//bostream �ṩ:
//  write����    ������д�뻺����
//  seekp����	   �����ж�λ
//  
//bistream �ṩ:
//  read����	   �����ж�����
//  seekg����    �����ж�λ
// peek         ������Ԥ��һ���ֽڣ���Ӱ�����ĵ�ǰλ�ã�
//
//biosexception
// �ṩ����ö�ٳ���
//     end;	   �����������ʱ�������Ļ�����ĩβ ��bistream.read �׳�	
//	 overflow: ������д����ʱ�������Ļ�����ĩβ ��bostream.write �׳�
////////////////////////////////////////////////////////////////////////////////////*/

#ifndef BIOSTREAM_H
#define	BIOSTREAM_H


#ifdef OS_WIN

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <cstring>
#else
#include <string.h>
#include <algorithm>
#endif

#include <string>
#include <vector>

inline
bool IS_BIGENDIAN( void )
{
	static unsigned short data = 0x1122;
	static unsigned char* pdata= (unsigned char*)&data; 
	static bool bbigendian = (*pdata == 0x11);
	
	return bbigendian;
}

inline
bool IS_SAME_ENDIAN( bool bigendian )
{
	return (bigendian&&IS_BIGENDIAN())||(!bigendian&&!IS_BIGENDIAN());
}

class agproexception
{
public:
	agproexception():m_cause(none) {}
	agproexception(int cause):m_cause(cause) {}
	enum ERRORTYPE {
		none = 0,		//no error;
		invalidcmd,		//���Ϸ���Э������
		rangeerror		//������Χ
	};
	int m_cause;

};

class biosexception 
{
public:
	enum BIOSEXCEPTION_CODE {
		none = 0,
		end = 1,		//�����������ʱ�������Ļ�����ĩβ
		overflow = 2,	//������д����ʱ�������Ļ�����ĩβ
		stringtoolong	//����׼C�ַ���ʱ����
	};

	BIOSEXCEPTION_CODE	m_cause;

	biosexception(BIOSEXCEPTION_CODE cause = none):m_cause(cause){}
};

class bios 
{
public:
	enum mode {	
		in = 1,		//������
		out = 2		//�����
	};
	enum seek_dir {
		beg = 0,	
		cur ,
		end} ;
protected:
	char*	pbase;	//��������ͷָ��
	char*	pcurr;	//�������еĵ�ǰָ��
	char*	pend;	//��������βָ��
	int		m_mode; //������ģʽ in or out
	bool	m_bigendian;// 
public:

	bios(bool bigendian):pbase(0),pcurr(0),pend(0),m_mode(0),m_bigendian(bigendian)
	{
	}

	bios(char* pch,long nLength,int mode,bool bigendian):pbase(pch),pcurr(pch),m_mode(mode),m_bigendian(bigendian)
	{
		pend = pch+nLength;
	}
	~bios() {}

	//����������һ��������
	//pch �������Ŀ�ʼ��ַ
	//nLength: �������Ĵ�С
	void attach(char* pch,long nLength) {
		pbase = pcurr = pch;
		pend = pbase+nLength;
	}

	//�����뻺��������
	void detach() { pbase = pcurr = pend = 0 ; }

	//�õ�������ʣ����õĳ���
	//seek����ͬ��λ�û�Ӱ��ú������
	long avail() const { return (long)(pend - pcurr); }

	//�õ���������ʹ�õĳ���
	//seek����ͬ��λ�û�Ӱ��ú������
	long length() const { return (long)(pcurr - pbase); }
};



class bostream : public bios
{
private:
	bostream& write_order(const char* p,size_t len)
	{
		if(avail() < (long)len)
			throw biosexception(biosexception::overflow);

		memcpy(pcurr,p,len);
		pcurr+=len;

		return *this;
	}

	bostream& write_opposite(const char* p,size_t len)
	{
		if(avail() < (long)len)
			throw biosexception(biosexception::overflow);

		size_t	times	= len/sizeof(char);
		char*	pos		= (char*)p+times-1;

		do 
		{
			*pcurr++ = *pos--;
			times--;
		} while ( times );

		return *this;
	}

public:
	bostream(bool bigendian=false):bios(bigendian)													{ m_mode = bios::out; }
	bostream(char* pch,long nLength, bool bigendian=false):bios(pch,nLength,bios::out, bigendian)	{ }

	//�������д������
	//p Ҫд�������
	//len p �ĳ���
	bostream& write(const char* p,size_t len)
	{
		if(avail() < (long)len)
			throw biosexception(biosexception::overflow);

		memcpy(pcurr,p,len);
		pcurr+=len;

		return *this;
	}

	bostream& write_care_endian( const char* p,size_t len )
	{
		if ( IS_SAME_ENDIAN( m_bigendian ) )
		{
			return write_order( p, len );
		}
		else
		{
			return write_opposite( p, len );
		}
	}

	bostream& writecstring( const char* _s )
	{
		return operator<<(_s);
	}

	~bostream() {}

	//�ƶ�������ĵ�ǰд��λ��
	//offset: �ƶ����ֽ���
	//seekmode: ���ĸ�λ�ÿ�ʼ�ƶ�
	bostream& seekp(long offset,bios::seek_dir seekmode = bios::cur) {
		switch(seekmode)
		{
		case bios::beg:
			pcurr = pbase+offset;
			break;
		case bios::cur:
			pcurr += offset;
			break;
		case bios::end:
			pcurr = pend+offset;
			break;
		}
		if(avail() < 0 || avail() > (pend-pbase))
			throw biosexception(biosexception::overflow);
		return *this;
	}

	inline bostream& operator<<(char);
	inline bostream& operator<<(unsigned char);
	inline bostream& operator<<(signed char);
	inline bostream& operator<<(short);
	inline bostream& operator<<(unsigned short);
	inline bostream& operator<<(int);
	inline bostream& operator<<(unsigned int);
	inline bostream& operator<<(long);
	inline bostream& operator<<(unsigned long);
	inline bostream& operator<<(long long);
	inline bostream& operator<<(unsigned long long);
	inline bostream& operator<<(float);
	inline bostream& operator<<(double);
	inline bostream& operator<<(long double);
	inline bostream& operator<<(bool);
	inline bostream& operator<<(const char*);
	inline bostream& operator<<(char*);
	inline bostream& operator<<(unsigned char*);
	inline bostream& operator<<(const unsigned char*);
	inline bostream& operator<<(std::string&);
	inline bostream& operator<<(const std::string&);
};

class bistream : public bios
{
private:
	bistream& read_order(char* p,size_t len)
	{
		if(avail() < (long)len)
			throw biosexception(biosexception::overflow);

		memcpy(p, pcurr, len);
		pcurr+=len;

		return *this;
	}

	bistream& read_opposite(char* p,size_t len)
	{
		if(avail() < (long)len)
			throw biosexception(biosexception::overflow);

		size_t	times	= len/sizeof(char);
		char*	pos		= (char*)p+times-1;

		do 
		{
			*pos-- = *pcurr++;
			times--;
		} while ( times );

		return *this;
	}

public:
	bistream(bool bigendian=false):bios(bigendian)													{ m_mode = bios::in; }
	bistream(char* pch,long nLength, bool bigendian=false):bios(pch,nLength,bios::in,bigendian)		{ }
	~bistream() {}

	//���������ж�len���ȵ��ֽڷ���p��
    bistream& read(char* p,size_t len) {
		if(avail() < (long)len)
			throw biosexception(biosexception::end);

		memcpy(p,pcurr,len);
		pcurr += len;

		return *this;
	}

	bistream& read(std::string& p, size_t len) {
		if(avail() < (long)len)
			throw biosexception(biosexception::end);

		p.append( pcurr, len );
		pcurr += len;

		return *this;
	}

	bistream& read_care_endian( char* p,size_t len )
	{
		if ( IS_SAME_ENDIAN( m_bigendian ) )
		{
			return read_order( p, len );
		}
		else
		{
			return read_opposite( p, len );
		}
	}

	bistream& readcstring( char* _s, unsigned long _ssize )
	{
		if( _ssize<=0 ) throw agproexception(agproexception::rangeerror);
		_s[_ssize-1] = 0;

		unsigned char blen;
		operator>>(blen);
		if( blen < 0xff )
		{
			if ( (unsigned long)blen>=_ssize )
			{
				seekg( 0-sizeof(blen) );
				throw agproexception(agproexception::rangeerror);
			}
			return read(_s,blen);
		}

		unsigned short wlen;
		operator>>(wlen);
		if(wlen < 0xfffe)
		{
			if ( (unsigned long)wlen>=_ssize )
			{
				seekg( 0-sizeof(wlen) );
				throw agproexception(agproexception::rangeerror);
			}
			return read(_s,wlen);
		}

		unsigned long dwlen;
		operator>>(dwlen);
		if ( dwlen>=_ssize )
		{
			seekg( 0-sizeof(dwlen) );
			throw agproexception(agproexception::rangeerror);
		}

		return read(_s,dwlen);
	}

	//Ԥ����һ���ֽ�
	int peek() {
		if(pcurr>=pend)
		{
			throw biosexception(biosexception::end);
		}
		else
			return static_cast<int>(*(pcurr));
	}

	//�ƶ��������ĵ�ǰ����ָ��
	bistream& seekg(long offset,bios::seek_dir dir=bios::cur) {
		switch(dir)
		{
		case bios::beg:
			pcurr = pbase+offset;
			break;
		case bios::cur:
			pcurr += offset;
			break;
		case bios::end:
			pcurr = pend+offset;
			break;
		}
		if(avail() < 0 || avail() > (pend-pbase))
			throw biosexception(biosexception::end);
		return *this;
	}

	inline bistream& operator>>(char&);
	inline bistream& operator>>(unsigned char&);
	inline bistream& operator>>(signed char&);
	inline bistream& operator>>(short&);
	inline bistream& operator>>(unsigned short&);
	inline bistream& operator>>(int&);
	inline bistream& operator>>(unsigned int&);
	inline bistream& operator>>(long&);
	inline bistream& operator>>(unsigned long&);
	inline bistream& operator>>(long long&);
	inline bistream& operator>>(unsigned long long&);
	inline bistream& operator>>(float&);
	inline bistream& operator>>(double&);
	inline bistream& operator>>(long double&);
	inline bistream& operator>>(bool&);
	inline bistream& operator>>(char*);
	inline bistream& operator>>(unsigned char*);
	inline bistream& operator>>(std::string&);
};


inline bostream& bostream::operator<<(char _c) {
	write((const char*)&_c,sizeof(char));
	return *this;
}
inline bostream& bostream::operator<<(unsigned char _c) {
	write((const char*)&_c,sizeof(unsigned char));
	return *this;
}
inline bostream& bostream::operator<<(signed char _c) {
	write((const char*)&_c,sizeof(signed char));
	return *this;
}
inline bostream& bostream::operator<<(short _s) 
{
	write_care_endian( (const char*)&_s, sizeof(short) );
	return *this;
}

inline bostream& bostream::operator<<(unsigned short _s) 
{
	write_care_endian( (const char*)&_s, sizeof(unsigned short) );
	return *this;
}
inline bostream& bostream::operator<<(int _i) {
	write_care_endian((const char*)&_i,sizeof(int));
	return *this;
}
inline bostream& bostream::operator<<(unsigned int _i) {
	write_care_endian((const char*)&_i,sizeof(unsigned int));
	return *this;
}
inline bostream& bostream::operator<<(long _l) {
	write_care_endian((const char*)&_l,sizeof(long));
	return *this;
}
inline bostream& bostream::operator<<(unsigned long _l) {
	write_care_endian((const char*)&_l,sizeof(unsigned long));
	return *this;
}
inline bostream& bostream::operator<<(long long _i64)
{
	write_care_endian((const char*)&_i64,sizeof(long long));
	return *this;
}
inline bostream& bostream::operator<<(unsigned long long _i64)
{
	write_care_endian((const char*)&_i64,sizeof(unsigned long long));
	return *this;
}
inline bostream& bostream::operator<<(float _f) {
	write_care_endian((const char*)&_f,sizeof(float));
	return *this;
}
inline bostream& bostream::operator<<(double _d) {
	write_care_endian((const char*)&_d,sizeof(double));
	return *this;
}
inline bostream& bostream::operator<<(long double _d) {
	write_care_endian((const char*)&_d,sizeof(long double));
	return *this;
}
inline bostream& bostream::operator<<(bool _b) {
	write((const char*)&_b,sizeof(bool));
	return *this;
}
inline bostream& bostream::operator<<(const char* _s) {
	int  slen = (int)strlen(_s);
	if(slen < 255)
	{
		operator<<((unsigned char)slen);
	}
	else if(slen < 0xfffe)
	{
		operator<<((unsigned char)0xff);
		operator<<((unsigned short)slen);
	}
	else
	{
		operator<<((unsigned char)0xff);
		operator<<((unsigned short)0xffff);
		operator<<((unsigned long)slen);
	}
	write((const char*)_s,slen);
	return *this;
}
inline bostream& bostream::operator<<(char* _s) {
	 return operator<<((const char*)_s) ;
}
inline bostream& bostream::operator<<(unsigned char* _s) {
	return operator<<((const char*)_s);
}
inline bostream& bostream::operator<<(const unsigned char* _s) {
	return operator<<((const char*)_s);
}
inline bostream& bostream::operator<<(std::string& _s) {
	return operator<<((const char*)_s.c_str());
}
inline bostream& bostream::operator<<(const std::string& _s) {
	return operator<<((const char*)_s.c_str());
}




inline bistream& bistream::operator>>(char& _c) {
	read(&_c,sizeof(char));
	return *this;
}
inline bistream& bistream::operator>>(signed char& _c) {
	read((char*)&_c,sizeof(signed char));
	return *this;
}
inline bistream& bistream::operator>>(unsigned char& _c) {
	read((char*)&_c,sizeof(unsigned char));
	return *this;
}
inline bistream& bistream::operator>>(short& _s) {
	read_care_endian((char*)&_s,sizeof(short));
	return *this;
}
inline bistream& bistream::operator>>(unsigned short& _s) {
	read_care_endian((char*)&_s,sizeof(unsigned short));
	return *this;
}
inline bistream& bistream::operator>>(int& _i) {
	read_care_endian((char*)&_i,sizeof(int));
	return *this;
}
inline bistream& bistream::operator>>(unsigned int& _i) {
	read_care_endian((char*)&_i,sizeof(unsigned int));
	return *this;
}
inline bistream& bistream::operator>>(long& _l) {
	read_care_endian((char*)&_l,sizeof(long));
	return *this;
}
inline bistream& bistream::operator>>(unsigned long& _l) {
	read_care_endian((char*)&_l,sizeof(unsigned long));
	return *this;
}
inline bistream& bistream::operator>>(long long& _i64) {
	read_care_endian((char*)&_i64,sizeof(long long));
	return *this;
}
inline bistream& bistream::operator>>(unsigned long long& _i64)
{
	read_care_endian((char*)&_i64,sizeof(unsigned long long));
	return *this;
}
inline bistream& bistream::operator>>(float& _f) {
	read_care_endian((char*)&_f,sizeof(float));
	return *this;
}
inline bistream& bistream::operator>>(double& _d) {
	read_care_endian((char*)&_d,sizeof(double));
	return *this;
}
inline bistream& bistream::operator>>(long double& _d) {
	read_care_endian((char*)&_d,sizeof(long double));
	return *this;
}
inline bistream& bistream::operator>>(bool& _b) {
	read((char*)&_b,sizeof(bool));
	return *this;
}
inline bistream& bistream::operator>>(char* _s) {
	unsigned char blen;
	operator>>(blen);
	if(blen < 0xff)
		return read(_s,blen);

	unsigned short wlen;
	operator>>(wlen);
	if(wlen < 0xfffe)
		return read(_s,wlen);

	unsigned long dwlen;
	operator>>(dwlen);
	return read(_s,dwlen);
}
inline bistream& bistream::operator>>(unsigned char* _s) {
	return operator>>((char*)_s);
}
inline bistream& bistream::operator>>(std::string& _s) {
	_s.clear();
	unsigned char blen;
	operator>>(blen);
	if(blen < 0xff)
		return read(_s,blen);

	unsigned short wlen;
	operator>>(wlen);
	if(wlen < 0xfffe)
		return read(_s,wlen);

	unsigned long dwlen;
	operator>>(dwlen);
	return read(_s,dwlen);
}









// bostream֧��std::vector
// bistream֧��std::vector

template <class T>
bostream& operator<<(bostream& bos, std::vector<T>& _s)
{
	std::size_t size = _s.size( );

	bos << size;
	for ( std::vector<T>::iterator it=_s.begin(); it!=_s.end(); ++it )
	{
		bos << (*it);
	}

	return bos;
}

template <class T>
bostream& operator<<(bostream& bos, const std::vector<T>& _s)
{
	std::size_t size = _s.size( );

	bos << (size);
	for ( std::vector<T>::const_iterator it=_s.begin(); it!=_s.end(); ++it )
	{
		bos << (*it);
	}

	return bos;
}

template<class T>
bistream& operator>>(bistream& bis, std::vector<T>& _s)
{
	_s.clear();
	std::size_t size = 0;
	bis>>(size);
	for ( std::size_t i=0; i<size; ++i )
	{
		_s.push_back( T() );
		bis>>_s[i];
	}

	return bis;
}


#endif

