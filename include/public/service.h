
#pragma once

#include <string>
#include "imbasignal.h"

class CService
{
public:
	static CService& get_instance( void );

	// 掉用此函数后会阻塞进程
	// Signal( function )
	void run( const std::string& servername, Signal runf, Signal stopf, Signal helpf, int argc, char** argv );

	std::string get_configfile( const std::string& defaultfile );

private:
	CService( void ) : m_running( false ) { }
	~CService( void )	{ }

public:
	std::string	m_servername;

	Signal		m_runfunction;
	Signal		m_stopfunction;
	Signal		m_helpfunction;

	std::string	m_configfile;
	bool		m_running;
};

#include "service.hpp"

