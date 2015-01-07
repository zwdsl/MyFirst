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

	// 不是非常严格的返回类型
	// 相信内部取值 
	LuaVarType		GetVarType();
	std::string		GetVar();
	char			GetValType();
	std::string		GetVal();

private:
	// 
	CLuaNode(const CLuaNode& node);
	// 参数不要改成(const CLuaNode& node), 自copy，或者与子节点有关copy问题;需要copy，所有节点都重构
	CLuaNode& operator=(const CLuaNode node);	

	void			Release();
	CLuaNode*	NewCopy(CLuaNode* pNode);

	char			_type;
	std::string		_var;			// LVT_NUM_KEY类型变量在不确定的情况下，保存为"%d",num
	std::string		_val;			// LUA_TNUMBER类型值在不确定的情况下，保存为"%f",num
									// 用一个变量的值去做另一个变量名的时候需要注意转化问题
	// 绝对不能对内部指针操作,唯一可以对此操作的是CLuaParser
	CLuaNode*		_nextsibling;
	CLuaNode*		_firstchild;
};

class CLuaParser
{
public:
	CLuaParser(void);
	~CLuaParser(void);

private:
	// 屏蔽,不需要定义，调用的时候必定报错
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

	// Get系列
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

	// 字符串 sign "dd>d" 表示函数有两个double类型的参数，一个double类型的返回结果。
	// 字母 'd' 表示double；'i' 表示integer，'s' 表示strings；'>' 作为参数和结果的分隔符
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

