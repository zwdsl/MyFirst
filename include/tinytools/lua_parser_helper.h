
#pragma once

#include <stdarg.h>
#include <iostream>

extern "C"
{
#include <tools/lua-5.1.4/src/lua.h>
#include <tools/lua-5.1.4/src/lualib.h>
#include <tools/lua-5.1.4/src/lauxlib.h>
}

//#define _LUA_TEST_

#ifdef _LUA_TEST_
int cnt = 0;
#endif

inline CLuaNode::CLuaNode():_type(LUA_TNONE),_nextsibling(0),_firstchild(0)
{
#ifdef _LUA_TEST_
	cnt++;
	std::cout << "CLuaNode Count:"<<cnt<<std::endl;
#endif
}

inline CLuaNode::~CLuaNode()
{
	Release();
#ifdef _LUA_TEST_
	cnt --;
	std::cout << "CLuaNode Count:"<<cnt<<std::endl;
#endif
}

inline LuaVarType CLuaNode::GetVarType()
{
	if (_var.length()<=0)
	{
		return LVT_UNKNOW;
	}

	if (_var.at(0) >= '0' && _var.at(0)<='9')
	{
		return LVT_NUM_KEY;
	}

	return LVT_STRING_KEY;
}

inline std::string CLuaNode::GetVar()
{
	return _var;
}

inline char CLuaNode::GetValType()
{
	return _type;
}

inline std::string CLuaNode::GetVal()
{
	return _val;
}

inline void CLuaNode::Release()
{
	if (_firstchild)
	{
		delete _firstchild;
		_firstchild = NULL;
	}
	if (_nextsibling)
	{
		delete _nextsibling;
		_nextsibling = NULL;
	}
	_type = LUA_TNONE;
}

inline CLuaNode* CLuaNode::NewCopy(CLuaNode* pNode)
{
	if (pNode == NULL)
	{
		return NULL;
	}

	CLuaNode* pNewNode = new CLuaNode();

	pNewNode->_type = pNode->_type;
	pNewNode->_var  = pNode->_var;
	pNewNode->_val  = pNode->_val;

	if (pNode->_firstchild)
	{
		pNewNode->_firstchild = NewCopy(pNode->_firstchild);
	}

	if (pNode->_nextsibling)
	{
		pNewNode->_nextsibling = NewCopy(pNode->_nextsibling);
	}

	return pNewNode;
}

inline CLuaNode::CLuaNode(const CLuaNode& node)
{
	_type	= node._type;
	_var	= node._var;
	_val	= node._val;
	_nextsibling = NewCopy(node._nextsibling);
	_firstchild  = NewCopy(node._firstchild);

#ifdef _LUA_TEST_
	cnt ++;
	std::cout << "CLuaNode Count:"<<cnt<<std::endl;
#endif
}

inline CLuaNode& CLuaNode::operator=(const CLuaNode node)
{
	Release();

	_type	= node._type;
	_var	= node._var;
	_val	= node._val;
	_nextsibling = NewCopy(node._nextsibling);
	_firstchild  = NewCopy(node._firstchild);

	return *this;
}

inline CLuaParser::CLuaParser(void)
{
	m_Lua = lua_open();

	//luaopen_base(m_Lua);
	//luaopen_table(m_Lua);
	//luaopen_io(m_Lua);
	//luaopen_os(m_Lua);
	//luaopen_string(m_Lua);
	//luaopen_math(m_Lua);
	//luaopen_debug(m_Lua);
	//luaopen_package(m_Lua);

	luaL_openlibs(m_Lua);
//	StatckDump();
}

inline CLuaParser::~CLuaParser(void)
{
	lua_close( m_Lua );
}

inline void CLuaParser::StatckDumpEx(int index)
{
	if ( index> LUA_REGISTRYINDEX) return;

	int vartype = 0;
	int type	= 0;
	int	 cnt	= 0;

	type = lua_type(m_Lua, index);
	switch(type)
	{
	case LUA_TSTRING:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TSTRING; value="<<lua_tostring(m_Lua, index)<<std::endl;
		break;
	case LUA_TBOOLEAN:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TBOOLEAN; value="<<(lua_toboolean(m_Lua, index)?"true":"false")<<std::endl;
		break;
	case LUA_TNUMBER:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TNUMBER; value="<<lua_tonumber(m_Lua, index)<<std::endl;
		break;
	case LUA_TNIL:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TNIL"<<std::endl;
		break;
	case LUA_TLIGHTUSERDATA:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TLIGHTUSERDATA"<<std::endl;
		break;
	case LUA_TTABLE:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TTABLE"<<std::endl;
		break;
	case LUA_TFUNCTION:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TFUNCTION"<<std::endl;
		break;
	case LUA_TUSERDATA:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TUSERDATA"<<std::endl;
		break;
	case LUA_TTHREAD:
		std::cout << "---------STACK["<<index<<"]: type=LUA_TTHREAD"<<std::endl;
		break;
	}

	if (type == LUA_TTABLE)
	{
		lua_pushnil( m_Lua );
		while ( 0 != lua_next(m_Lua, index) )
		{
			lua_pushvalue(m_Lua, -2);

			vartype = lua_type(m_Lua, -1);
			type	= lua_type(m_Lua, -2);
			cnt ++;

			if (LUA_TSTRING == vartype)
			{
				switch(type)
				{
				case LUA_TSTRING:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TSTRING; "<<lua_tostring(m_Lua, -1)<<" = "<<lua_tostring(m_Lua, -2)<<std::endl;
					break;
				case LUA_TBOOLEAN:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TBOOLEAN; "<<lua_tostring(m_Lua, -1)<<" = "<<(lua_toboolean(m_Lua, -2)?"true":"false")<<std::endl;
					break;
				case LUA_TNUMBER:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TNUMBER; "<<lua_tostring(m_Lua, -1)<<" = "<<lua_tonumber(m_Lua, -2)<<std::endl;
					break;
				case LUA_TNIL:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TNIL; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				case LUA_TLIGHTUSERDATA:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TLIGHTUSERDATA; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				case LUA_TTABLE:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TTABLE; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				case LUA_TFUNCTION:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TFUNCTION; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				case LUA_TUSERDATA:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TUSERDATA; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				case LUA_TTHREAD:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TTHREAD; "<<lua_tostring(m_Lua, -1)<<std::endl;
					break;
				}
			}
			else if(LUA_TNUMBER == vartype)
			{
				switch(type)
				{
				case LUA_TSTRING:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TSTRING; "<<lua_tonumber(m_Lua, -1)<<" = "<<lua_tostring(m_Lua, -2)<<std::endl;
					break;
				case LUA_TBOOLEAN:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TBOOLEAN; "<<lua_tonumber(m_Lua, -1)<<" = "<<(lua_toboolean(m_Lua, -2)?"true":"false")<<std::endl;
					break;
				case LUA_TNUMBER:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TNUMBER; "<<lua_tonumber(m_Lua, -1)<<" = "<<lua_tonumber(m_Lua, -2)<<std::endl;
					break;
				case LUA_TNIL:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TNIL; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				case LUA_TLIGHTUSERDATA:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TLIGHTUSERDATA; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				case LUA_TTABLE:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TTABLE; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				case LUA_TFUNCTION:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TFUNCTION; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				case LUA_TUSERDATA:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TUSERDATA; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				case LUA_TTHREAD:
					std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TTHREAD; "<<lua_tonumber(m_Lua, -1)<<std::endl;
					break;
				}
			}
			else
			{
				std::cout << "---------STACK["<<index<<":"<<cnt<<"]: type=LUA_TUNKNOW"<<std::endl;
			}

			lua_pop(m_Lua, 2);
		}
	}
	
	std::cout << std::endl;
}

inline void CLuaParser::StatckDump()
{
	int top = lua_gettop(m_Lua);

	for (int i=1; i<=top; i++)
	{
		int type = lua_type(m_Lua, i);

		switch(type)
		{
		case LUA_TSTRING:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TSTRING; value="<<lua_tostring(m_Lua, i)<<std::endl;
			break;
		case LUA_TBOOLEAN:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TBOOLEAN; value="<<(lua_toboolean(m_Lua, i)?"true":"false")<<std::endl;
			break;
		case LUA_TNUMBER:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TNUMBER; value="<<lua_tonumber(m_Lua, i)<<std::endl;
			break;
		case LUA_TNIL:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TNIL"<<std::endl;
			break;
		case LUA_TLIGHTUSERDATA:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TLIGHTUSERDATA"<<std::endl;
			break;
		case LUA_TTABLE:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TTABLE"<<std::endl;
			break;
		case LUA_TFUNCTION:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TFUNCTION"<<std::endl;
			break;
		case LUA_TUSERDATA:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TUSERDATA"<<std::endl;
			break;
		case LUA_TTHREAD:
			std::cout << "---------STACK["<<i<<"]: type=LUA_TTHREAD"<<std::endl;
			break;
		}
	}
	std::cout << std::endl;
}


inline bool CLuaParser::DoString(const std::string& luastr)
{
	if (luaL_dostring(m_Lua, luastr.c_str()) != 0)
	{
		std::cout << "CLuaParser::DoString return Err:"<<lua_tostring(m_Lua, -1)<<std::endl;
		lua_pop(m_Lua, 1);

		return false;
	}
	return true;
}

inline bool CLuaParser::DoFile(const std::string& luafile)
{
	if (luaL_dofile(m_Lua, luafile.c_str()) != 0)
	{
		std::cout << "CLuaParser::DoFile return Err:"<<lua_tostring(m_Lua, -1)<<std::endl;
		lua_pop(m_Lua, 1);

		return false;
	}
	return true;
}

inline bool CLuaParser::GetTable( const std::string& inVar, CLuaNode& outTable	)
{
	return GetLuaList(inVar, outTable);
}

inline bool CLuaParser::GetInt( const std::string& inVar, int&	  outVal)
{
	//CLuaNode outTable;
	//if ( GetLuaList( inVar, outTable ) )
	//{
	//	outVal = atoi( outTable._var.c_str() );
	//	return true;
	//}
	//return false;
	lua_getglobal(m_Lua, inVar.c_str());

	if (lua_type(m_Lua, -1) == LUA_TNUMBER)
	{
		outVal = static_cast<int>(lua_tointeger(m_Lua, -1));

		lua_pop(m_Lua, 1);
		return true;
	}
	
	lua_pop(m_Lua, 1);
	return false;
}

inline bool CLuaParser::GetDouble( const std::string& inVar, double&	  outVal)
{
	lua_getglobal(m_Lua, inVar.c_str());

	if (lua_type(m_Lua, -1) == LUA_TNUMBER)
	{
		outVal = lua_tonumber(m_Lua, -1);

		lua_pop(m_Lua, 1);
		return true;
	}

	lua_pop(m_Lua, 1);
	return false;
}

inline bool CLuaParser::GetBoolean( const std::string& inVar, bool&	  outVal)
{
	lua_getglobal(m_Lua, inVar.c_str());

	if (lua_type(m_Lua, -1) == LUA_TBOOLEAN)
	{
		outVal = lua_toboolean(m_Lua, -1)?true:false;

		lua_pop(m_Lua, 1);
		return true;
	}

	lua_pop(m_Lua, 1);
	return false;
}

inline bool CLuaParser::GetString( const std::string& inVar, std::string& outVal)
{
	lua_getglobal(m_Lua, inVar.c_str());

	if (lua_type(m_Lua, -1) == LUA_TSTRING)
	{
		outVal = lua_tostring(m_Lua, -1);

		lua_pop(m_Lua, 1);
		return true;
	}

	lua_pop(m_Lua, 1);
	return false;
}

inline bool CLuaParser::FirstChildTable( const CLuaNode& inTable, CLuaNode& outTable )
{
	if (inTable._firstchild)
	{
		outTable = *inTable._firstchild;
		return true;
	}
	return false;
}

inline bool CLuaParser::NextSiblingTable(const CLuaNode& inTable, CLuaNode& outTable )
{
	if (inTable._nextsibling)
	{
		outTable = *inTable._nextsibling;
		return true;
	}
	return false;
}

inline bool CLuaParser::GetTable( const std::string& inVar, const CLuaNode& inTable, CLuaNode& outTable)
{
	CLuaNode* pNode = inTable._firstchild;

	while (pNode)
	{
		if ( pNode->_type==LUA_TTABLE && inVar.compare(pNode->_var)==0)
		{
			outTable = *pNode;

			return true;
		}

		pNode = pNode->_nextsibling;
	}
	return false;
}

inline bool CLuaParser::GetInt( const std::string& inVar, const CLuaNode& inTable, int& outVal)
{
	CLuaNode* pNode = inTable._firstchild;

	while (pNode)
	{
		if ( pNode->_type==LUA_TNUMBER && inVar.compare(pNode->_var)==0)
		{
			outVal = atoi(pNode->_val.c_str());

			return true;
		}

		pNode = pNode->_nextsibling;
	}
	return false;
}

inline bool CLuaParser::GetDouble( const std::string& inVar, const CLuaNode& inTable, double& outVal)
{
	CLuaNode* pNode = inTable._firstchild;

	while (pNode)
	{
		if ( pNode->_type==LUA_TNUMBER && inVar.compare(pNode->_var)==0)
		{
			outVal = atof(pNode->_val.c_str());

			return true;
		}

		pNode = pNode->_nextsibling;
	}
	return false;
}

inline bool CLuaParser::GetBoolean( const std::string& inVar, const CLuaNode& inTable, bool&	outVal)
{
	CLuaNode* pNode = inTable._firstchild;

	while (pNode)
	{
		if ( pNode->_type==LUA_TBOOLEAN && inVar.compare(pNode->_var)==0)
		{
			outVal = atoi(pNode->_val.c_str())?true:false;

			return true;
		}

		pNode = pNode->_nextsibling;
	}
	return false;
}

inline bool CLuaParser::GetString( const std::string& inVar, const CLuaNode& inTable, std::string& outVal)
{
	CLuaNode* pNode = inTable._firstchild;

	while (pNode)
	{
		if ( pNode->_type==LUA_TSTRING && inVar.compare(pNode->_var)==0)
		{
			outVal = pNode->_val;

			return true;
		}

		pNode = pNode->_nextsibling;
	}
	return false;
}

inline bool CLuaParser::CallFunction( const std::string& inFunction,  const char* sign,...	)
{
	va_list vl;
	int		narg, nres, nresmem;

	if ( !CheckFunctionSig(sign) )
	{
		std::cout << "CallFunction pcall failed :wrong sign"<<std::endl;
		return false;
	}

	lua_getglobal(m_Lua, inFunction.c_str());
	if ( lua_type(m_Lua, -1)!= LUA_TFUNCTION )
	{
		std::cout << "CallFunction pcall failed :no function"<<std::endl;
		lua_pop( m_Lua, 1);
		return false;
	}

	char* sig = const_cast<char*>(sign);
	va_start(vl, sign);
	
	narg = 0;
	while (*sig)
	{
		switch(*sig++) 
		{
		case 'd':
			lua_pushnumber(m_Lua, va_arg(vl, double));
			narg ++;
			break;
		case 'i':
			lua_pushinteger(m_Lua, va_arg(vl, int));
			narg ++;
			break;
		case 's':
			lua_pushstring(m_Lua, va_arg(vl, char*));
			narg ++;
			break;
		case '>':
			goto callcontinue;
			break;
		default:
			break;
		}
		luaL_checkstack(m_Lua, 1, "too manyarguments");
	}

callcontinue:

#ifdef _LUA_TEST_
	StatckDump();
#endif

	nres = static_cast<int>(strlen(sig));
	if (lua_pcall(m_Lua, narg, nres, 0) != 0)
	{
		std::cout << "CallFunction pcall "<< inFunction<<": "<< lua_tostring(m_Lua, -1) <<std::endl;
		va_end(vl);

		lua_pop(m_Lua, 1);
		return false;
	}

	nresmem = nres;
	nres = -nres;
	while (*sig)
	{
		switch(*sig++)
		{
		case 'd':
			if ( lua_isnumber(m_Lua, nres) )
			{
				*va_arg(vl, double*)= lua_tonumber(m_Lua, nres);
			}
			break;
		case 'i':
			if ( lua_isnumber(m_Lua, nres) )
			{
				*va_arg(vl, int*) = static_cast<int>(lua_tointeger(m_Lua, nres));
			}
			break;
		case 's':
			if ( lua_isstring(m_Lua, nres) )
			{
				sprintf(va_arg(vl, char *), "%s", lua_tostring(m_Lua, nres));
			}
			break;
		default:
			break;
		}
		nres ++;
	}
	va_end(vl);

#ifdef _LUA_TEST_
	StatckDump();
#endif
	lua_pop(m_Lua, nresmem);

	return true;
}

inline void CLuaParser::ExpandNewTableList( CLuaNode* outList )
{
	int		type;
	int		typevar;
	int		top = lua_gettop(m_Lua);

	lua_pushnil( m_Lua );
	CLuaNode** pNode = &outList->_firstchild;

#ifdef _LUA_TEST_
	StatckDump();
#endif

	if ( top+2 >= LUA_MINSTACK)
	{
		return;
	}

	while (lua_next(m_Lua, top) != 0)
	{	
#ifdef _LUA_TEST_
		StatckDump();
#endif
		type = lua_type(m_Lua, -1);
		typevar = lua_type(m_Lua, -2);
		switch(type)
		{
		case LUA_TNIL:
		case LUA_TLIGHTUSERDATA:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
			{
				if (LUA_TSTRING == typevar)
				{
					(*pNode)		= new CLuaNode(); 
					(*pNode)->_type = type;
					(*pNode)->_var  = lua_tostring(m_Lua, -2);

					pNode			= &(*pNode)->_nextsibling;
				}
				else if (LUA_TNUMBER == typevar)
				{
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					char tmp[1024];
					sprintf(tmp, "%d", lua_tointeger(m_Lua, -2));
					(*pNode)->_var  = tmp;

					pNode			= &(*pNode)->_nextsibling;
				}
			}
			break;
		case LUA_TBOOLEAN:
			{
				if (LUA_TSTRING == typevar)
				{
					char tmp[1024];
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					(*pNode)->_var  = lua_tostring(m_Lua, -2);
					sprintf(tmp, "%d", lua_toboolean(m_Lua, -1));
					(*pNode)->_val  = tmp;

					pNode			= &(*pNode)->_nextsibling;
				}
				else if (LUA_TNUMBER == typevar)
				{
					char tmp[1024];
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					sprintf(tmp, "%d", lua_tointeger(m_Lua, -2));
					(*pNode)->_var  = tmp;
					sprintf(tmp, "%d", lua_toboolean(m_Lua, -1));
					(*pNode)->_val  = tmp;

					pNode			= &(*pNode)->_nextsibling;
				}
			}
			break;
		case LUA_TNUMBER:			
			{
				if (LUA_TSTRING == typevar)
				{
					char tmp[1024];
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					(*pNode)->_var  = lua_tostring(m_Lua, -2);
					sprintf(tmp, "%f", lua_tonumber(m_Lua, -1));
					(*pNode)->_val  = tmp;

					pNode			= &(*pNode)->_nextsibling;
				}
				else if (LUA_TNUMBER == typevar)
				{
					char tmp[1024];
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					sprintf(tmp, "%d", lua_tointeger(m_Lua, -2));
					(*pNode)->_var  = tmp;
					sprintf(tmp, "%f", lua_tonumber(m_Lua, -1));
					(*pNode)->_val  = tmp;

					pNode			= &(*pNode)->_nextsibling;
				}
			}
			break;
		case LUA_TSTRING:
			{
				if (LUA_TNUMBER == typevar)
				{
					char tmp[1024];
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					sprintf(tmp, "%d", lua_tointeger(m_Lua, -2));
					(*pNode)->_var  = tmp;
					(*pNode)->_val  = lua_tostring(m_Lua, -1);

					pNode			= &(*pNode)->_nextsibling;
				}
				else if (LUA_TSTRING == typevar)
				{
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					(*pNode)->_var  = lua_tostring(m_Lua, -2);
					(*pNode)->_val  = lua_tostring(m_Lua, -1);

					pNode			= &(*pNode)->_nextsibling;
				}
			}
			break;
		case LUA_TTABLE:
			{
				if (LUA_TSTRING == typevar)
				{
					(*pNode)		= new CLuaNode(); 
					(*pNode)->_type = type;
					(*pNode)->_var  = lua_tostring(m_Lua, -2);

					ExpandNewTableList((*pNode));
				}
				else if (LUA_TNUMBER == typevar)
				{
					(*pNode)		= new CLuaNode();
					(*pNode)->_type = type;
					char tmp[1024];
					sprintf(tmp, "%d", lua_tointeger(m_Lua, -2));
					(*pNode)->_var  = tmp;

					ExpandNewTableList((*pNode));
					pNode			= &(*pNode)->_nextsibling;
				}
#ifdef _LUA_TEST_
				StatckDump();
#endif
			}
			break;
		default:
			std::cout << "Lua GetList Err" << std::endl;
			break;
		}
		lua_pop(m_Lua, 1);
#ifdef _LUA_TEST_
		StatckDump();
#endif
	}
}

inline bool CLuaParser::GetLuaList( const std::string& inVar, CLuaNode& outHead)
{
	lua_getglobal(m_Lua, inVar.c_str());

	outHead.Release();

	outHead._var  = inVar;
	outHead._type = lua_type(m_Lua, -1);

	switch( outHead._type ) 
	{
	case LUA_TSTRING:
		{
			outHead._val = lua_tostring(m_Lua, -1);
		}
		break;
	case LUA_TBOOLEAN:
		{
			char tmp[1024];
			sprintf(tmp, "%d", lua_toboolean(m_Lua, -1));
			outHead._val = tmp;
		}
		break;
	case LUA_TNUMBER:
		{
			char tmp[1024];
			sprintf(tmp, "%f", lua_tonumber(m_Lua, -1));
			outHead._val = tmp;
		}
		break;
	case LUA_TTABLE:
		{
			ExpandNewTableList(&outHead);
		}
		break;
	case LUA_TNIL:
		break;
	case LUA_TLIGHTUSERDATA:
		break;
	case LUA_TFUNCTION:
		break;
	case LUA_TUSERDATA:
		break;
	case LUA_TTHREAD:
		break;
	default:
		{
			lua_pop(m_Lua, 1);
			return false;
		}
	}

	lua_pop(m_Lua, 1);
	return true;
}


inline bool CLuaParser::DivideString(const std::string& inString, char inDevided, std::vector<std::string>& outStrings)
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

inline bool CLuaParser::CheckFunctionSig( const char* sign )
{
	int type = 0;
	int nCnt[2];
	memset (nCnt, 0, sizeof(nCnt));

	char* sig = const_cast<char*>(sign);

	while (*sig)
	{
		if (*sig=='d' || *sig=='i' || *sig=='s')
		{
			nCnt[type] ++;
		}
		else if (*sig=='>')
		{
			type ++;
		}
		else
		{
			return false;
		}
		if (type >= 2)
		{
			break;
		}
		sig ++;
	}

	if (type>=2 || nCnt[0]+1>=LUA_MINSTACK || nCnt[1]+1>=LUA_MINSTACK)
	{
		return false;
	}
	
	return true;
}

