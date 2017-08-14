#pragma once 

#include "IteratoTraits.h"
#include "TypeTraits.h"

template<class T>
void Destroy(T* p)
{
	p->~T();
}

template<class T1, class T2>
void Construct(T1* p, const T2& value)
{
	new(p)T1(value);
}


//vector<string> 要先析构然后释放
//vector<int>    直接释放
//类型萃取

template<class ForwardIterator>
inline void _Destroy(ForwardIterator first, ForwardIterator last, FalseType)
{
	while (first != last)
	{
		Destroy(&(*first));
		first++;
	}
}

template<class ForwardIterator>
inline void _Destroy(ForwardIterator first, ForwardIterator last, TrueType)
{}
	

template<class ForwardIterator>
inline void Destroy(ForwardIterator first, ForwardIterator last)
{
	_Destroy(first, last, TypeTraits<IteratorTraits<ForwardIterator>::ValueType>::has_trivial_destructor());
}