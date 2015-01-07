

#ifndef __TEMPLATE_CALLBACK_H__
#define __TEMPLATE_CALLBACK_H__

#include <include/base/Base.h>
using namespace Communications;


// 抽象 
struct DelegationInterface 
{
public:
	DelegationInterface( void ) : m_ref( 1 ) { }

	virtual ~DelegationInterface() {};
	virtual void Action() = 0;

	void Retain( void ) { ++m_ref; }
	void Release( void )
	{
		if ( --m_ref==0 )
		{
			delete this;
		}
	}; 

private:
	int		m_ref;
};

// 成员函数没有参数
template
<
	class T, 
	class R
>
class DelegationImpl_0 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_0<T, R> >
{
public:
	typedef R (T::*ClassFunction)();
	
	DelegationImpl_0( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_0(T* object, ClassFunction function) 
		:_object(object), _function(function) 
	{ 
	}
	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(); 
	}

private:
	T* _object;
	ClassFunction _function;
};

template
<
	class R
>
class DelegationImpl_0_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_0_S<R> >
{
public:
	typedef R (*StaticFunction)();

	DelegationImpl_0_S( ) : _function(0)
	{
	}
	DelegationImpl_0_S(StaticFunction function) 
		:_function(function) 
	{ 
	}
	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(); 
	}

private:
	StaticFunction _function;
};


// 成员函数带一个参数
template
<
	class T, 
	class R,
	class P0
>
class DelegationImpl_1 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_1<T, R, P0> >	
{
public:
	typedef R (T::*ClassFunction)( P0 );

	DelegationImpl_1( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_1(T* object, ClassFunction function, P0 param0) 
		: _object(object), _function(function), _param0(param0)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
};

template
<
	class R,
	class P0
>
class DelegationImpl_1_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_1_S<R, P0> >	
{
public:
	typedef R (*StaticFunction)( P0 );

	DelegationImpl_1_S( ) : _function(0)
	{
	}
	DelegationImpl_1_S(StaticFunction function, P0 param0) 
		: _function(function), _param0(param0)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0); 
	}

private:
	StaticFunction _function;
	P0 _param0;
};

// 成员函数带两个参数
template
<
	class T, 
	class R,
	class P0,
	class P1
>
class DelegationImpl_2 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_2<T, R, P0, P1> >
{
public:
	typedef R (T::*ClassFunction)( P0, P1 );

	DelegationImpl_2( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_2(T* object, ClassFunction function, P0 param0, P1 param1) 
		: _object(object), _function(function), 
		_param0(param0), _param1(param1)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0, _param1); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
	P1 _param1;
};

template
<
	class R,
	class P0,
	class P1
>
class DelegationImpl_2_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_2_S<R, P0, P1> >
{
public:
	typedef R (*StaticFunction)( P0, P1 );

	DelegationImpl_2_S( ) : _function(0)
	{
	}
	DelegationImpl_2_S(StaticFunction function, P0 param0, P1 param1) 
		: _function(function), _param0(param0), _param1(param1)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0, _param1); 
	}

private:
	StaticFunction _function;
	P0 _param0;
	P1 _param1;
};

// 成员函数带三个参数
template
<
	class T, 
	class R,
	class P0,
	class P1,
	class P2
>
class DelegationImpl_3 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_3<T, R, P0, P1, P2> >
{
public:
	typedef R (T::*ClassFunction)( P0, P1, P2 );

	DelegationImpl_3( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_3(T* object, ClassFunction function, P0 param0, P1 param1, P2 param2) 
		: _object(object), _function(function), 
		_param0(param0), _param1(param1), _param2(param2)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0, _param1, _param2); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
};

template
<
	class R,
	class P0,
	class P1,
	class P2
>
class DelegationImpl_3_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_3_S<R, P0, P1, P2> >
{
public:
	typedef R (*StaticFunction)( P0, P1, P2 );

	DelegationImpl_3_S( ) : _function(0)
	{
	}
	DelegationImpl_3_S(StaticFunction function, P0 param0, P1 param1, P2 param2) 
		: _function(function), _param0(param0), _param1(param1), _param2(param2)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0, _param1, _param2); 
	}

private:
	StaticFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
};

// 成员函数带四个参数
template
<
	class T, 
	class R,
	class P0,
	class P1,
	class P2,
	class P3
>
class DelegationImpl_4 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_4<T, R, P0, P1, P2, P3> >
{
public:
	typedef R (T::*ClassFunction)( P0, P1, P2, P3 );

	DelegationImpl_4( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_4(T* object, ClassFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3) 
		: _object(object), _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0, _param1, _param2, _param3); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
};

template
<
	class R,
	class P0,
	class P1,
	class P2,
	class P3
>
class DelegationImpl_4_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_4_S<R, P0, P1, P2, P3> >
{
public:
	typedef R (*StaticFunction)( P0, P1, P2, P3 );

	DelegationImpl_4_S( ) :  _function(0)
	{
	}
	DelegationImpl_4_S(StaticFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3) 
		: _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0, _param1, _param2, _param3); 
	}

private:
	StaticFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
};

// 成员函数带五个参数
template
<
	class T, 
	class R,
	class P0,
	class P1,
	class P2,
	class P3,
	class P4
>
class DelegationImpl_5 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_5<T, R, P0, P1, P2, P3, P4> >
{
public:
	typedef R (T::*ClassFunction)( P0, P1, P2, P3, P4 );

	DelegationImpl_5( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_5(T* object, ClassFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3, P4 param4) 
		: _object(object), _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3),
		_param4(param4)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0, _param1, _param2, _param3, _param4); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
	P4 _param4;
};

template
<
	class R,
	class P0,
	class P1,
	class P2,
	class P3,
	class P4
>
class DelegationImpl_5_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_5_S<R, P0, P1, P2, P3, P4> >
{
public:
	typedef R (*StaticFunction)( P0, P1, P2, P3, P4 );

	DelegationImpl_5_S( ) : _function(0)
	{
	}
	DelegationImpl_5_S(StaticFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3, P4 param4) 
		: _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3),
		_param4(param4)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0, _param1, _param2, _param3, _param4); 
	}

private:
	StaticFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
	P4 _param4;
};

// 成员函数带六个参数
template
<
	class T, 
	class R,
	class P0,
	class P1,
	class P2,
	class P3,
	class P4,
	class P5
>
class DelegationImpl_6 : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_6<T, R, P0, P1, P2, P3, P4, P5> >
{
public:
	typedef R (T::*ClassFunction)( P0, P1, P2, P3, P4, P5 );

	DelegationImpl_6( ) : _object(0), _function(0)
	{
	}
	DelegationImpl_6(T* object, ClassFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3, P4 param4, P5 param5) 
		: _object(object), _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3),
		_param4(param4), _param5(param5)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _object && _function )
			(_object->*_function)(_param0, _param1, _param2, _param3, _param4, _param5); 
	}

private:
	T* _object;
	ClassFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
	P4 _param4;
	P5 _param5;
};

template
<
	class R,
	class P0,
	class P1,
	class P2,
	class P3,
	class P4,
	class P5
>
class DelegationImpl_6_S : public DelegationInterface, public CMemoryPool_Public< DelegationImpl_6_S<R, P0, P1, P2, P3, P4, P5> >
{
public:
	typedef R (*StaticFunction)( P0, P1, P2, P3, P4, P5 );

	DelegationImpl_6_S( ) : _function(0)
	{
	}
	DelegationImpl_6_S(StaticFunction function, P0 param0, 
		P1 param1, P2 param2, P3 param3, P4 param4, P5 param5) 
		: _function(function), 
		_param0(param0), _param1(param1), 
		_param2(param2), _param3(param3),
		_param4(param4), _param5(param5)
	{ 
	}

	virtual void Action( ) 
	{
		if ( _function )
			(*_function)(_param0, _param1, _param2, _param3, _param4, _param5); 
	}

private:
	StaticFunction _function;
	P0 _param0;
	P1 _param1;
	P2 _param2;
	P3 _param3;
	P4 _param4;
	P5 _param5;
};

// 单信号
class Signal
{
public:
	Signal( void ) :_PI(0)	{ }
	~Signal( void )			{ clear(); }

	Signal( const Signal& s )
	{
		_PI = s._PI;
		_PI->Retain();
	}

	// 连接到类的成员函数
	// 保证T的对象的持久性，
	// 保证参数支持copy构造
	template<class T, class R>
	Signal(T& Slot, R (T::*Sf)() )
	{
		_PI = new DelegationImpl_0<T, R>(&Slot, Sf);
	}

	template<class T, class R, class P0>
	Signal(T& Slot, R (T::*Sf)(P0), P0 p0)
	{
		_PI = new DelegationImpl_1<T, R, P0>(&Slot, Sf, p0);
	}

	template<class T, class R, class P0, class P1>
	Signal(T& Slot, R (T::*Sf)(P0, P1), P0 p0, P1 p1)
	{
		_PI = new DelegationImpl_2<T, R, P0, P1>(&Slot, Sf, p0, p1);
	}

	template<class T, class R, class P0, class P1, class P2>
	Signal(T& Slot, R (T::*Sf)(P0, P1, P2), P0 p0, P1 p1, P2 p2)
	{
		_PI = new DelegationImpl_3<T, R, P0, P1, P2>(&Slot, Sf, p0, p1, p2);
	}

	template<class T, class R, class P0, class P1, class P2, class P3>
	Signal(T& Slot, R (T::*Sf)(P0, P1, P2, P3), P0 p0, P1 p1, P2 p2, P3 p3)
	{
		_PI = new DelegationImpl_4<T, R, P0, P1, P2, P3>(&Slot, Sf, p0, p1, p2, p3);
	}

	template<class T, class R, class P0, class P1, class P2, class P3, class P4>
	Signal(T& Slot, R (T::*Sf)(P0, P1, P2, P3, P4), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		_PI = new DelegationImpl_5<T, R, P0, P1, P2, P3, P4>(&Slot, Sf, p0, p1, p2, p3, p4);
	}

	template<class T, class R, class P0, class P1, class P2, class P3, class P4, class P5>
	Signal(T& Slot, R (T::*Sf)(P0, P1, P2, P3, P4, P5), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		_PI = new DelegationImpl_6<T, R, P0, P1, P2, P3, P4, P5>(&Slot, Sf, p0, p1, p2, p3, p4, p5);
	}

	// 连接到静态函数
	template<class R>
	Signal(R (*Sf)() )
	{
		_PI = new DelegationImpl_0_S<R>(Sf);
	}

	template<class R, class P0>
	Signal(R (*Sf)(P0), P0 p0)
	{
		_PI = new DelegationImpl_1_S<R, P0>(Sf, p0);
	}

	template<class R, class P0, class P1>
	Signal(R (*Sf)(P0, P1), P0 p0, P1 p1)
	{
		_PI = new DelegationImpl_2_S<R, P0, P1>(Sf, p0, p1);
	}

	template<class R, class P0, class P1, class P2>
	Signal(R (*Sf)(P0, P1, P2), P0 p0, P1 p1, P2 p2)
	{
		_PI = new DelegationImpl_3_S<R, P0, P1, P2>(Sf, p0, p1, p2);
	}

	template<class R, class P0, class P1, class P2, class P3>
	Signal(R (*Sf)(P0, P1, P2, P3), P0 p0, P1 p1, P2 p2, P3 p3)
	{
		_PI = new DelegationImpl_4_S<R, P0, P1, P2, P3>(Sf, p0, p1, p2, p3);
	}

	template<class R, class P0, class P1, class P2, class P3, class P4>
	Signal(R (*Sf)(P0, P1, P2, P3, P4), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		_PI = new DelegationImpl_5_S<R, P0, P1, P2, P3, P4>(Sf, p0, p1, p2, p3, p4);
	}

	template<class R, class P0, class P1, class P2, class P3, class P4, class P5>
	Signal(R (*Sf)(P0, P1, P2, P3, P4, P5), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		_PI = new DelegationImpl_6_S<R, P0, P1, P2, P3, P4, P5>(Sf, p0, p1, p2, p3, p4, p5);
	}

	Signal& operator=( const Signal& s )
	{
		if ( this == &s || _PI==s._PI )
		{
			return *this;
		}

		if ( _PI )
		{
			_PI->Release();
		}

		_PI = s._PI;
		if ( _PI )
		{
			_PI->Retain();
		}

		return *this;
	}

	void operator()( void ) 
	{ 
		if(_PI) _PI->Action(); 
	}

	void clear( void )
	{
		if (_PI)
		{
			_PI->Release();
			_PI = 0;
		}
	}

	// 连接到类的成员函数
	// 保证T的对象的持久性，
	// 保证参数支持copy构造
	template<class T, class R>
	void ConnectSlot(T& Slot, R (T::*Sf)() )
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_0<T, R>(&Slot, Sf);
	}

	template<class T, class R, class P0>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0), P0 p0)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_1<T, R, P0>(&Slot, Sf, p0);
	}

	template<class T, class R, class P0, class P1>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0, P1), P0 p0, P1 p1)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_2<T, R, P0, P1>(&Slot, Sf, p0, p1);
	}

	template<class T, class R, class P0, class P1, class P2>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0, P1, P2), P0 p0, P1 p1, P2 p2)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_3<T, R, P0, P1, P2>(&Slot, Sf, p0, p1, p2);
	}

	template<class T, class R, class P0, class P1, class P2, class P3>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0, P1, P2, P3), P0 p0, P1 p1, P2 p2, P3 p3)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_4<T, R, P0, P1, P2, P3>(&Slot, Sf, p0, p1, p2, p3);
	}

	template<class T, class R, class P0, class P1, class P2, class P3, class P4>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0, P1, P2, P3, P4), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_5<T, R, P0, P1, P2, P3, P4>(&Slot, Sf, p0, p1, p2, p3, p4);
	}

	template<class T, class R, class P0, class P1, class P2, class P3, class P4, class P5>
	void ConnectSlot(T& Slot, R (T::*Sf)(P0, P1, P2, P3, P4, P5), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_6<T, R, P0, P1, P2, P3, P4, P5>(&Slot, Sf, p0, p1, p2, p3, p4, p5);
	}

	// 连接到静态函数
	template<class R>
	void ConnectSlot(R (*Sf)() )
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_0_S<R>(Sf);
	}

	template<class R, class P0>
	void ConnectSlot(R (*Sf)(P0), P0 p0)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_1_S<R, P0>(Sf, p0);
	}

	template<class R, class P0, class P1>
	void ConnectSlot(R (*Sf)(P0, P1), P0 p0, P1 p1)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_2_S<R, P0, P1>(Sf, p0, p1);
	}

	template<class R, class P0, class P1, class P2>
	void ConnectSlot(R (*Sf)(P0, P1, P2), P0 p0, P1 p1, P2 p2)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_3_S<R, P0, P1, P2>(Sf, p0, p1, p2);
	}

	template<class R, class P0, class P1, class P2, class P3>
	void ConnectSlot(R (*Sf)(P0, P1, P2, P3), P0 p0, P1 p1, P2 p2, P3 p3)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_4_S<R, P0, P1, P2, P3>(Sf, p0, p1, p2, p3);
	}

	template<class R, class P0, class P1, class P2, class P3, class P4>
	void ConnectSlot(R (*Sf)(P0, P1, P2, P3, P4), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_5_S<R, P0, P1, P2, P3, P4>(Sf, p0, p1, p2, p3, p4);
	}

	template<class R, class P0, class P1, class P2, class P3, class P4, class P5>
	void ConnectSlot(R (*Sf)(P0, P1, P2, P3, P4, P5), P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		if( _PI )	_PI->Release();
		_PI = new DelegationImpl_6_S<R, P0, P1, P2, P3, P4, P5>(Sf, p0, p1, p2, p3, p4, p5);
	}
private:
	DelegationInterface* _PI;
};

#endif //__TEMPLATE_CALLBACK_H__
