#pragma once
#include "alloc.h"
#include "construct.h"
#include <string>
#include "IteratoTraits.h"



template<class T, class Ptr, class Ref>
class VectorIterator
{
public:
	typedef T  ValueType;
	typedef VectorIterator<T, T*, T&> Self;

	VectorIterator(T* p = NULL)
		: _p(p)
	{}

	VectorIterator(const Self& s)
		: _p(s._p)
	{}

	Ref operator*()
	{
		return *_p;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_p++;
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_p++;
		return temp;
	}

	Self& operator--()
	{
		_p--;
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		--_p;
		return temp;
	}

	bool operator!=(const Self& it)
	{
		return _p != it._p;
	}

	bool operator==(const Self& it)
	{
		return _p == it._p;
	}
private:
	T* _p;
};



template<class T, class Alloc = _alloc>
class Vector
{
public:
	typedef T ValueType;
	typedef T* Iterator;
	typedef const T* ConstIterator;

	typedef RerveseIterator<ConstIterator> ConstRerveseIterator;
	typedef RerveseIterator<Iterator> RerveseIterator;

	//typedef VectorIterator<T, T*, T&> Iterator;
	typedef SimpleAlloc<T, Alloc> dataAlloc;
public:
	Vector()
		: _start(NULL)
		, _finish(NULL)
		, _endOfStorage(NULL)
	{}

	void UNInit_Fill(size_t n, const T& value)
	{
		_start = dataAlloc::Allocate(n);
		for (size_t idx = 0; idx < n; ++idx)
			Construct(_start + idx, value);
	}

	Vector(size_t n, const T& value = T())
	{
		UNInit_Fill(n, value);
	}

	void PushBack(const T& value)
	{
		_CheckCapacity();
		*_finish = value;
		_finish++;
	}

	void PopBack()
	{
		--_finish;
	}

	size_t Size()const
	{
		return _finish - _start;
	}

	size_t Capacity()const
	{
		return _endOfStorage - _start;
	}

	~Vector()
	{
		Destroy(_start, _endOfStorage);
		dataAlloc::DeAllocate(&*_start, Capacity());
	}

	/////////////////////////////////////////////////////////
	Iterator Begin()
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}


private:

	void _CheckCapacity()
	{
		if (_finish == _endOfStorage)
		{
			size_t capacity = Capacity();
			size_t newSize = 2 * capacity + 3;
			
			T* pTmep = dataAlloc::Allocate(newSize);
			T* p = (T*)malloc(sizeof(int)*3);
			for (size_t idx = 0; idx < newSize; ++idx)
				Construct(pTmep + idx, T());

			// 内置类型--->memcpy
			// 自定义类型---> =
			for (size_t idx = 0; idx < Size(); ++idx)
				pTmep[idx] = _start[idx];

			// 释放旧空间
			// vector<int>--->
			// vector<string>--> 销毁掉
			if (_start)
			{
				Destroy(_start, _finish);
				dataAlloc::DeAllocate(_start, capacity);
			}

			_start = pTmep;
			_finish = _start + capacity;
			_endOfStorage = _start + newSize;
		}
	}

private:
	Iterator _start;
	Iterator _finish;
	Iterator _endOfStorage;
};

#include <string>
#include <iostream>
using namespace std;
void Test1()
{
	Vector<std::string> s1;
	s1.PushBack("1111");
	s1.PushBack("2222");
	s1.PushBack("3333");
	s1.PushBack("4444");
	Vector<std::string>::Iterator it = s1.Begin();
	while (it != s1.End())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;
	cout << s1.Capacity() << endl;
	cout << s1.Size() << endl;
	cout << Distance(s1.Begin(), s1.End())<<endl;
}



void Test2()
{
	Vector<int> s1;
	s1.PushBack(1);
	s1.PushBack(2);
	s1.PushBack(3);
	s1.PushBack(4);
	Vector<int>::Iterator it = s1.Begin();
	while (it != s1.End())
	{
		cout << *it << " ";
		it++;
	}
	cout << endl;
	s1.PopBack();
	cout << s1.Capacity() << endl;
	cout << s1.Size() << endl;
	cout << Distance(s1.Begin(), s1.End());
}