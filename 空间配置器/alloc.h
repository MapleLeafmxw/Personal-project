#pragma once 

#include <new>
#include <string>

#define NULL 0
typedef void(*NewHandlleFunPtr)();

template<int inst>
class MallocAllocaTemplate
{
public:
	static void* Allocate(size_t n)
	{
		void* res = malloc(n);
		if (res == NULL)
			res = OOM_Malloc(n);
		return res;
	}

	static void* ReAllocate(void* p, size_t newsize)
	{
		void* res = realloc(p, newsize);
		if (res == NULL)
			res = OOM_Reallocate(p, newsize);
		return res;
	}

	static void DeAllocate(void* p)
	{
		free(p);
	}

	static void* OOM_Malloc(size_t n)
	{
		NewHandlleFunPtr temp;
		for (;;)
		{
			temp = pNewHandlleFun;
			if (temp == NULL)
				throw std::bad_alloc();
			(*temp)();
			void* res = malloc(n);
			if (res)
				return res;
		}
	}

	static void* OOM_ReAllocate(void* p, size_t newsize)
	{
		NewHandlleFunPtr temp;
		for (;;)
		{
			temp = pNewHandlleFun;
			if (temp == NULL)
				throw std::bad_alloc();
			(*temp)();
			void* res = realloc(p, newsize);
			if (res)
				return res;
		}
	}

private:
	static NewHandlleFunPtr pNewHandlleFun;
};

template<int inst>
NewHandlleFunPtr MallocAllocaTemplate<inst>::pNewHandlleFun = 0;

typedef MallocAllocaTemplate<0> MallocAlloc;

template<int inst>
class DefaultAllocateTemplate
{
public:

	static void* Allocate(size_t n)
	{
		if (n > __MAX_BYTES)
			return MallocAlloc::Allocate(n);

		size_t index = FREELIST_INDEX(n);
		Obj* res = _freeList[index];
		if (res == NULL)
			return Refill(ROUND_UP(n));
		_freeList[index] = res->_FreeListLink;
		return res;
	}

	static void DeAllocate(void* p, size_t n)
	{
		if (n > __MAX_BYTES)
			return MallocAlloc::DeAllocate(p);
		size_t index = FREELIST_INDEX(n);
		((Obj*)p)->_FreeListLink = _freeList[index];
		_freeList[index] = ((Obj*)p);
	}

	static void* Refill(size_t n)
	{
		size_t nObjs = 20;//ֱ�����ڴ��Ҫ20��С���ڴ�ռ�
		void* chunk = ChunkAllocate(n, nObjs);

		if (nObjs == 1)
			return chunk;

		//1<=nObjs<=20
		void* res = chunk;
		chunk = (char*)chunk + n;
		size_t index = FREELIST_INDEX(n);
		for (size_t idx = 1; idx < nObjs; ++idx)
		{
			((Obj*)chunk)->_FreeListLink = _freeList[index];
			_freeList[index] = ((Obj*)chunk);
			chunk = (char*)chunk + n;
		}
		return res;
	}

	static void* ChunkAllocate(size_t size, size_t& n)
	{
		size_t leftByte = _endFree - _startFree;
		size_t totalByte = n*size;

		if (leftByte >= totalByte)//�ڴ��㹻����20��С���ڴ�
		{
			void* res = _startFree;
			_startFree += totalByte;
			return res;
		}
		else if (leftByte >= size)//�ڴ治������20��С���ڴ棬���ǿ��Է�����ڵ���1С��20��С���ڴ�
		{
			n = leftByte / size;
			void* res = _startFree;
			_startFree += size*n;
			return res;
		}
		else	//һ�鶼���벻����
		{
			//��Ϊ����������ڴ�Ƚϴ���ڴ����ʣ��Ŀռ䲻�����䣬���Խ��ڴ���ʣ����ڴ��������������
			if (leftByte > 0)
			{
				size_t index = FREELIST_INDEX(leftByte);
				((Obj*)_startFree)->_FreeListLink = _freeList[index];
				_freeList[index] = ((Obj*)_startFree);
			}
			//�ڴ���û�пռ��ˣ���ϵͳ����ռ�
			size_t get2Byte = 2 * totalByte + (_heapSize >> 4);
			_startFree = (char*)malloc(get2Byte);
			if (_startFree == NULL)//��ϵͳ����ռ�ʧ��
			{
				//�������������Ҹ�����ڴ�
				size_t index = FREELIST_INDEX(size);
				for (; index < __NFREELISTS; index++)
				{
					if (_freeList[index] != NULL)
					{
						_startFree = (char*)_freeList[index];
						_endFree = _startFree + (index + 1)*__ALIGN;
						_freeList[index] = _freeList[index]->_FreeListLink;

						return ChunkAllocate(size, n);//�������������ҵ��˸�����ڴ�
					}
				}
				//������������Ҳû���ҵ�������ڴ棬����ֻ�ܿ��Լ��ռ��������е�OOM_Malloc
				_endFree = 0;
				_startFree = (char*)MallocAlloc::Allocate(get2Byte);
			}

			_endFree = _startFree + get2Byte;
			_heapSize = get2Byte;
			return ChunkAllocate(size, n);
		}

	}

private:

	static size_t FREELIST_INDEX(size_t bytes)
	{
		return ((bytes + __ALIGN - 1) / __ALIGN - 1);
	}
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + __ALIGN - 1)&~(__ALIGN - 1));
	}

private:
	enum{__ALIGN = 8};
	enum{__MAX_BYTES = 128};
	enum{__NFREELISTS = __MAX_BYTES/__ALIGN};

	union Obj{
		union Obj* _FreeListLink;   //��һ���ڴ��ĵ�ַ
		char _clientData[1];		  //�û�����
	};



	static Obj* _freeList[__NFREELISTS];
	static char* _startFree;
	static char* _endFree;
	static size_t _heapSize;
};

template<int inst>
char* DefaultAllocateTemplate<inst>::_startFree = NULL;

template<int inst>
char* DefaultAllocateTemplate<inst>::_endFree = NULL;

template<int inst>
size_t DefaultAllocateTemplate<inst>::_heapSize = 0;

template<int inst>
typename DefaultAllocateTemplate<inst>::Obj* DefaultAllocateTemplate<inst>::_freeList[__NFREELISTS] = { 0 };

#ifdef USE_MALLOC
typedef MallocAlloca _alloc;
#else
typedef DefaultAllocateTemplate<0> _alloc;
#endif

template<class T, class Alloc>
class SimpleAlloc
{
public:

	static T* Allocate(void)
	{
		T* p = (T*)Alloc::Allocate(sizeof(T));
		return p;
	}

	static T* Allocate(size_t n)
	{
		T* p = (T*)Alloc::Allocate(sizeof(T)*n);
		return p;
	}

	static void DeAllocate(T* p)
	{
		Alloc::DeAllocate(p, sizeof(T));
	}

	static void DeAllocate(T* p, size_t n)
	{
		Alloc::DeAllocate(p, sizeof(T)*n);
	}
};