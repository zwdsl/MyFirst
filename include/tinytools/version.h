
#pragma once

#include <string>

// 关于版本的比较 可以直接使用numver的大小比较
#define STRVER_2_NUMVER(strVer)		(long)(CVerNumber(strVer))
#define NUMVER_2_STRVER(numVer)		(std::string)(CVerNumber(numVer))

// 
// 一般情况下for （server-client）
// 

#define SECOND_BIT_LEN	13  // 2000-8191	0x1FFF
#define THIRD_BIT_LEN	4	// 1-12			0xF
#define FORTH_BIT_LEN	5   // 1-31			0x1F

#define FIRST_BIT_LEN	(32-SECOND_BIT_LEN-THIRD_BIT_LEN-FORTH_BIT_LEN) // 10 // 3FF

class CVerNumber
{
	friend bool	operator == (const CVerNumber& v1, const CVerNumber& v2);
	friend bool	operator >  (const CVerNumber& v1, const CVerNumber& v2);
	friend bool	operator >= (const CVerNumber& v1, const CVerNumber& v2);
	friend bool	operator <  (const CVerNumber& v1, const CVerNumber& v2);
	friend bool	operator <= (const CVerNumber& v1, const CVerNumber& v2);

public:
	CVerNumber( const std::string& strVer );
	CVerNumber( long lVer );
	CVerNumber( int first, int second, int third, int fourth );
	CVerNumber( const CVerNumber& ver );
	~CVerNumber( void );

public:
	CVerNumber& operator = ( const CVerNumber& rhs );

	operator long( void );
	operator std::string();

public:
	int	   GetFirst( void ) const;
	int    GetSecond( void ) const;
	int	   GetThird( void ) const;
	int	   GetFourth( void ) const;

private:
	int	 m_first, m_second, m_third, m_fourth;
	long m_lver;
	std::string m_strver;
};

// 不比较是否有效
bool operator == (const CVerNumber& v1, const CVerNumber& v2);
bool operator >  (const CVerNumber& v1, const CVerNumber& v2);
bool operator >= (const CVerNumber& v1, const CVerNumber& v2);
bool operator <  (const CVerNumber& v1, const CVerNumber& v2);
bool operator <= (const CVerNumber& v1, const CVerNumber& v2);

#include "version_helper.h"

