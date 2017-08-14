#pragma once

struct InputIteratorTag
{};

struct OutputIteratorTag
{};

struct ForwardIteratorTag : public InputIteratorTag
{};

struct BidirectionalIteratorTag : public OutputIteratorTag
{};

struct RandomAccessIteratorTag : public BidirectionalIteratorTag
{};


template<class Iterator>
struct IteratorTraits
{
	typename typedef Iterator::IteratorCategory IteratorCategory;
	typename typedef Iterator::ValueType ValueType;
	typename typedef Iterator::Pointer Pointer;
	typename typedef Iterator::Reference Reference;
	typename typedef Iterator::DifferenceType DifferenceType;
};

//原生态指针
template<class T>
struct IteratorTraits<T*>
{
	typename typedef RandomAccessIteratorTag IteratorCategory;
	typename typedef T ValueType;
	typename typedef T* Pointer;
	typename typedef T& Reference;
	typename typedef int DifferenceType;
};

//原生态指针
template<class T>
struct IteratorTraits<const T*>
{
	typename typedef RandomAccessIteratorTag IteratorCategory;
	typename typedef T ValueType;
	typename typedef const T* Pointer;
	typename typedef const T& Reference;
	typename typedef int DifferenceType;
};


template<class Iterator>
typename IteratorTraits<Iterator>::DifferenceType _Distance(Iterator first, Iterator last, RandomAccessIteratorTag)
{
	return first - last;
}

template<class Iterator>
typename IteratorTraits<Iterator>::DifferenceType _Distance(Iterator first, Iterator lase, InputIteratorTag)
{
	IteratorTraits<Iterator>::DifferenceType n = 0;
	while (first != last)
	{
		++first;
		++n;
	}
	return n;
}

template<class Iterator>
typename IteratorTraits<Iterator>::DifferenceType Distance(Iterator first, Iterator last)
{
	return _Distance(first, last, IteratorTraits<Iterator>::IteratorCategory());
}

template<class Iterator>
class RerveseIterator
{
public:
	typename typedef IteratorTraits<Iterator>::IteratorCategory IteratorCategory;
	typename typedef IteratorTraits<Iterator>::ValueType ValueType;
	typename typedef IteratorTraits<Iterator>::Pointer Pointer;
	typename typedef IteratorTraits<Iterator>::Reference Reference;
	typename typedef IteratorTraits<Iterator>::DifferenceType DifferenceType;

	typedef RerveseIterator<Iterator> Self;
public:
	RerveseIterator()
		:_cur()
	{}
	RerveseIterator(Iterator cur)
		:_cur(cur)
	{}
	RerveseIterator(const Self& s)
		:_cur(s._cur)
	{}
	Self& operator++()
	{
		--_cur;
		return *this;
	}
	Self operator++(int)
	{
		Iterator temp = _cur;
		--_cur;
		return temp;
	}

	Self& operator--()
	{
		++_cur;
		return *this;
	}
	Self operator--(int)
	{
		Iterator temp = _cur;
		++_cur;
		return temp;
	}

	Reference operator*()
	{
		Iterator temp(_cur);
		return *(--temp);
	}
	Reference operator->()
	{	
		return &(*(_cur));
	}
	bool operator==(const Self& s)
	{
		return _cur == s._cur;
	}
	bool operator!=(const Self& s)
	{
		return _cur != s._cur;
	}

private:
	Iterator _cur;
};