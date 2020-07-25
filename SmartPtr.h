//
// SmartPtr.h
//

#pragma once

#pragma region SmartPtr

#include "TypeDef.h"

template<typename T>
class Counter;

template<typename T>
class SmartPtr
{

protected:

	Counter<T> *_ptr_cnt;

public:

	SmartPtr()
	{
		_ptr_cnt = new Counter<T>(nullptr);
	}

	SmartPtr(T *ptr)
	{
		_ptr_cnt = new Counter<T>(ptr);
	}

	SmartPtr(const SmartPtr &other)
	{
		_ptr_cnt = other._ptr_cnt;
		_ptr_cnt->_counter++;
	}

	SmartPtr &operator=(const SmartPtr &other)
	{
		if (this == &other)
			return *this;

		_ptr_cnt->_counter--;
		if (_ptr_cnt->_counter == 0)
			delete _ptr_cnt;

		_ptr_cnt = other._ptr_cnt;
		other._ptr_cnt->_counter++;

		return *this;
	}

	virtual ~SmartPtr()
	{
		_ptr_cnt->_counter--;
		if (_ptr_cnt->_counter == 0)
			delete _ptr_cnt;
	}

public:

	operator T*()
	{
		return _ptr_cnt->_ptr;
	}

	T &operator*()
	{
		return *(_ptr_cnt->_ptr);
	}

	T* operator->() const throw() // no exception
	{
		return _ptr_cnt->_ptr;
	}

	T** operator&()
	{
		return &_ptr_cnt->_ptr;
	}

	bool operator!=(const T* other_ptr) const
	{
		return _ptr_cnt->_ptr != other_ptr;
	}

	bool operator==(const T* other_ptr) const
	{
		return _ptr_cnt->_ptr == other_ptr;
	}
};

template<typename T>
class Counter
{
private:
	T *_ptr;
	uint64 _counter;

	template<typename T>
	friend class SmartPtr;

	Counter(T *ptr)
	{
		_ptr = ptr;
		_counter = 1;
	}

	~Counter()
	{
		if (_ptr != nullptr)
		{
			delete _ptr;
		}		
	}
};

#pragma endregion