#pragma once

#include <string>
#include <vector>

enum LuaVarType
{
	LVT_STRING_KEY	= 0,
	LVT_NUM_KEY,

	LVT_UNKNOW
};

class CLuaNode
{
	friend class CLuaParser;
public:
	CLuaNode();
	~CLuaNode();

	// ���Ƿǳ��ϸ�ķ�������
	// �����ڲ�ȡֵ 
	LuaVarType		GetVarType();
	std::string		GetVar();
	char			GetValType();
	std::string		GetVal();

private:
	// 
	CLuaNode(const CLuaNode& node);
	// ������Ҫ�ĳ�(const CLuaNode& node), ��copy���������ӽڵ��й�copy����;��Ҫcopy�����нڵ㶼�ع�
	CLuaNode& operator=(const CLuaNode node);	

	void			Release();
	CLuaNode*	NewCopy(CLuaNode* pNode);

	char			_type;
	std::string		_var;			// LVT_NUM_KEY���ͱ����ڲ�ȷ��������£�����Ϊ"%d",num
	std::string		_val;			// LUA_TNUMBER����ֵ�ڲ�ȷ��������£�����Ϊ"%f",num
									// ��һ��������ֵȥ����һ����������ʱ����Ҫע��ת������
	// ���Բ��ܶ��ڲ�ָ�����,Ψһ���ԶԴ˲�������CLuaParser
	CLuaNode*		_nextsibling;
	CLuaNode*		_firstchild;
};

class CLuaParser
{
public:
	CLuaParser(void);
	~CLuaParser(void);

private:
	// ����,����Ҫ���壬���õ�ʱ��ض�����
	CLuaParser(const CLuaParser& L);
	CLuaParser& operator=(const CLuaParser& L);

public:
	void	StatckDump();
	/*
	#define LUA_REGISTRYINDEX	(-10000)
	#define LUA_ENVIRONINDEX	(-10001)
	#define LUA_GLOBALSINDEX	(-10002)
	#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))
	*/
	void	StatckDumpEx(int index);

	// Getϵ��
	bool	DoString(const std::string& luastr);
	bool	DoFile	(const std::string& luafile);

	bool	GetTable	( const std::string& inVar, CLuaNode& outTable	);
	bool	GetInt		( const std::string& inVar, int&	  outVal	);
	bool	GetDouble	( const std::string& inVar, double&	  outVal	);
	bool	GetBoolean	( const std::string& inVar, bool&	  outVal	);
	bool	GetString	( const std::string& inVar, std::string&outVal	);

	bool	FirstChildTable( const CLuaNode& inTable, CLuaNode& outTable );
	bool	NextSiblingTable(const CLuaNode& inTable, CLuaNode& outTable );
	bool	GetTable	( const std::string& inVar, const CLuaNode& inTable, CLuaNode& outTable);
	bool	GetInt		( const std::string& inVar, const CLuaNode& inTable, int&	   outVal	);
	bool	GetDouble	( const std::string& inVar, const CLuaNode& inTable, double&   outVal	);
	bool	GetBoolean	( const std::string& inVar, const CLuaNode& inTable, bool&	   outVal	);
	bool	GetString	( const std::string& inVar, const CLuaNode& inTable, std::string&outVal	);

	// �ַ��� sign "dd>d" ��ʾ����������double���͵Ĳ�����һ��double���͵ķ��ؽ����
	// ��ĸ 'd' ��ʾdouble��'i' ��ʾinteger��'s' ��ʾstrings��'>' ��Ϊ�����ͽ���ķָ���
	// ForExample: int ret=0; CallFunction("funAdd", "ii>i", 10, 5, &ret);
	bool	CallFunction( const std::string& inFunction,  const char* sign,...	);

private:
	void	ExpandNewTableList( CLuaNode* outList );

	bool	GetLuaList( const std::string& inVar, CLuaNode& outHead);
	bool	DivideString(const std::string& inString, char inDevided, std::vector<std::string>& outStrings);
	bool	CheckFunctionSig( const char* sign );

private:
	struct lua_State*	m_Lua;
};

#include "lua_parser_helper.h"

