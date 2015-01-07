
#pragma once

//#ifndef __COMMON_CIRCLEQUEUE_H__
//#define __COMMON_CIRCLEQUEUE_H__


// ע�����⣺
// ����֤�̰߳�ȫ
// �����Ա�֤ һ���̶߳���һ���߳�д �ǰ�ȫ��

namespace Communications
{
	/************************************************************************/
	/*	                                                                    */
	/*					ѭ������											*/
	/*			���TΪĳ��ָ�룬��ô��Ҫ����Clear����Pop��pop�����е�����	*/
	/*			���е��ܳ���Ϊ1<<SIZE_BIT									*/
	/*																		*/
	/************************************************************************/
	template<typename T, unsigned long SIZE_BIT=10 >
	class CCircleQueue
	{
	public:
		CCircleQueue( void )
		{
			assert(SIZE_BIT>0);
			_queuemask	= ((1<<SIZE_BIT)-1);
			_queue		= new T[(1<<SIZE_BIT)];
			_queuehead	= 0;
			_queuetail	= 0;
		}

		~CCircleQueue( void )
		{
			delete [] _queue;
		}

		// ������ڲ���������
		void Clear( void )
		{
			_queuehead	= 0;
			_queuetail	= 0;
		}

		// ����һ�������ڶ���β
		bool Push( const T& data )
		{
			unsigned long tmptail = ((_queuetail+1)&_queuemask);
			if ( tmptail==_queuehead )		return false;

			_queuetail = tmptail;
			_queue[tmptail] = data;

			return true;
		}

		// ȡ��һ�����ݴӶ���ͷ
		bool Pop( T& data )
		{
			if (_queuehead == _queuetail)	return false;

			unsigned long tmphead = ((_queuehead+1)&_queuemask);
			_queuehead = tmphead;
			data = _queue[tmphead];

			return true;
		}

		// �����Ƿ�Ϊ��
		bool IsEmpty( void ) const
		{
			if (_queuehead == _queuetail)	return true;

			return false;
		}

		// ���е��ܳ���
		unsigned long TotalSize( void ) const
		{
			return (_queuemask+1);
		}

		// ���е�ǰ����
		unsigned long Size( void ) const
		{
			if ( _queuehead == _queuetail )		return 0;

			return ((_queuetail-_queuehead+_queuemask+1)&_queuemask);
		}

	private:
		T*						_queue;
		unsigned long			_queuemask;

		volatile unsigned long	_queuehead;
		volatile unsigned long	_queuetail;

	private:
		CCircleQueue( const CCircleQueue& );
		CCircleQueue& operator=( const CCircleQueue& );
	};

}

//#endif // __COMMON_CIRCLEQUEUE_H__
