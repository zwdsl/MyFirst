
#pragma once

#include <vector>

inline bool DivideString(const std::string& inString, char inDevided, std::vector<std::string>& outStrings)
{
	char* pStart = const_cast<char*>(inString.c_str());
	char* pEnd	 = pStart;

	while (*pEnd != 0)
	{
		if (*pEnd == inDevided)
		{
			if (pEnd-pStart>0)
			{
				std::string key;
				key.assign(pStart, pEnd-pStart);
				outStrings.push_back(key);
			}
			pStart = ++pEnd;
		}
		else
		{
			++pEnd;
		}
	}

	if (*pStart != 0)
	{
		if (pEnd-pStart>0)
		{
			std::string key;
			key.assign(pStart, pEnd-pStart);
			outStrings.push_back(key);
		}
	}
	return true;
}

inline CVerNumber::CVerNumber( const std::string& strVer ) : m_first(0),m_second(0),m_third(0),m_fourth(0), m_lver(0)
{
	std::vector<std::string>	datas;
	DivideString(strVer, '.', datas);

	if (datas.size()<4)
	{
		datas.clear();
		DivideString(strVer, ',', datas);
	}
	if (datas.size()<4)		return;

	int	 first	= atoi(datas[0].c_str());
	int  second = atoi(datas[1].c_str());
	int  third	= atoi(datas[2].c_str());
	int  fourth  = atoi(datas[3].c_str());

	first	= (first  & ((1<<FIRST_BIT_LEN)-1) );
	second	= (second & ((1<<SECOND_BIT_LEN)-1));
	third	= (third  & ((1<<THIRD_BIT_LEN)-1) );
	fourth	= (fourth  & ((1<<FORTH_BIT_LEN)-1) );

	m_first = first;
	m_second = second;
	m_third= third;
	m_fourth  = fourth;

	m_strver = strVer;
	m_lver = ( (first<<(SECOND_BIT_LEN+THIRD_BIT_LEN+FORTH_BIT_LEN)) 
			 | (second<<(THIRD_BIT_LEN+FORTH_BIT_LEN))
			 | (third<<FORTH_BIT_LEN)
			 | (fourth) );
}

inline CVerNumber::CVerNumber( long lVer ) : m_first(0), m_second(0), m_third(0), m_fourth(0), m_lver(0)
{
	unsigned long ulVer = lVer;

	int	 first = (ulVer>>(SECOND_BIT_LEN+THIRD_BIT_LEN+FORTH_BIT_LEN));
	int  second = ((ulVer<<FIRST_BIT_LEN)>>(FIRST_BIT_LEN+THIRD_BIT_LEN+FORTH_BIT_LEN));
	int  third= ((ulVer<<(FIRST_BIT_LEN+SECOND_BIT_LEN))>>(FIRST_BIT_LEN+SECOND_BIT_LEN+FORTH_BIT_LEN));
	int  fourth  = ((ulVer<<(FIRST_BIT_LEN+SECOND_BIT_LEN+THIRD_BIT_LEN))>>(FIRST_BIT_LEN+SECOND_BIT_LEN+THIRD_BIT_LEN));

	first = (first & ((1<<FIRST_BIT_LEN)-1) );
	second = (second & ((1<<SECOND_BIT_LEN)-1) );
	third= (third& ((1<<THIRD_BIT_LEN)-1));
	fourth  = (fourth  & ((1<<FORTH_BIT_LEN)-1)  );

	m_first = first;
	m_second = second;
	m_third= third;
	m_fourth  = fourth;

	m_lver = lVer;
	char charVer[256];
	sprintf(charVer, "%d.%d.%d.%d", first, second, third, fourth);
	m_strver= charVer;
}

inline CVerNumber::CVerNumber( int first, int second, int third, int fourth ) : m_first(0),m_second(0),m_third(0),m_fourth(0), m_lver(0)
{
	first = (first & ((1<<FIRST_BIT_LEN)-1) );
	second = (second & ((1<<SECOND_BIT_LEN)-1) );
	third= (third& ((1<<THIRD_BIT_LEN)-1));
	fourth  = (fourth  & ((1<<FORTH_BIT_LEN)-1)  );

	m_first = first;
	m_second = second;
	m_third= third;
	m_fourth  = fourth;

	char charVer[256];
	sprintf(charVer, "%d.%d.%d.%d", first, second, third, fourth);
	m_strver= charVer;
	m_lver = ( (first<<(SECOND_BIT_LEN+THIRD_BIT_LEN+FORTH_BIT_LEN)) 
		| (second<<(THIRD_BIT_LEN+FORTH_BIT_LEN))
		| (third<<FORTH_BIT_LEN)
		| (fourth) );
}

inline CVerNumber::CVerNumber( const CVerNumber& ver )
{
	m_first	= ver.m_first;
	m_second	= ver.m_second;
	m_third	= ver.m_third;
	m_fourth	= ver.m_fourth;

	m_lver	= ver.m_lver;
	m_strver= ver.m_strver;
}

inline CVerNumber::~CVerNumber( void )
{

}

inline CVerNumber& CVerNumber::operator = ( const CVerNumber& rhs )
{
	if ( &rhs != this)
	{
		m_first	= rhs.m_first;
		m_second	= rhs.m_second;
		m_third	= rhs.m_third;
		m_fourth	= rhs.m_fourth;

		m_lver	= rhs.m_lver;
		m_strver= rhs.m_strver;
	}
	return *this;
}

inline CVerNumber::operator long( void )
{
	return m_lver;
}

inline CVerNumber::operator std::string()
{
	return m_strver;
}

inline int	CVerNumber::GetFirst( void ) const
{
	return m_first;
}

inline int CVerNumber::GetSecond( void ) const
{
	return m_second;
}

inline int	CVerNumber::GetThird( void ) const
{
	return m_third;
}

inline int	CVerNumber::GetFourth( void ) const
{
	return m_fourth;
}


inline bool operator == (const CVerNumber& v1, const CVerNumber& v2)
{
	return ( v1.m_lver==v2.m_lver );
}

inline bool operator >  (const CVerNumber& v1, const CVerNumber& v2)
{
	return ( v1.m_lver>v2.m_lver );
}

inline bool operator >= (const CVerNumber& v1, const CVerNumber& v2)
{
	return ( v1.m_lver>=v2.m_lver );;
}

inline bool operator <  (const CVerNumber& v1, const CVerNumber& v2)
{
	return ( v1.m_lver<v2.m_lver );
}

inline bool operator <= (const CVerNumber& v1, const CVerNumber& v2)
{
	return ( v1.m_lver<=v2.m_lver );
}

