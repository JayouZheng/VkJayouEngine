/*********************************************************************
 *  SmartPtr.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#pragma once

#pragma region SmartPtr

#include "Core/TypeDef.h"

template<typename T>
class Counter;

template<typename T>
class SmartPtr
{

protected:

	Counter<T>* m_counter;

public:

	SmartPtr()
	{
		m_counter = new Counter<T>(nullptr);
	}

	SmartPtr(T *ptr)
	{
		m_counter = new Counter<T>(ptr);
	}

	SmartPtr(const SmartPtr &other)
	{
		m_counter = other.m_counter;
		m_counter->m_count++;
	}

	SmartPtr &operator=(const SmartPtr &other)
	{
		if (this == &other)
			return *this;

		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;

		m_counter = other.m_counter;
		other.m_counter->m_count++;

		return *this;
	}

	virtual ~SmartPtr()
	{
		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;
	}

public:

	operator T*()
	{
		return m_counter->m_object;
	}

	T &operator*()
	{
		return *(m_counter->m_object);
	}

	T* operator->() const throw() // no exception
	{
		return m_counter->m_object;
	}

	T** operator&()
	{
		return &m_counter->m_object;
	}

	bool operator!=(const T* other_ptr) const
	{
		return m_counter->m_object != other_ptr;
	}

	bool operator==(const T* other_ptr) const
	{
		return m_counter->m_object == other_ptr;
	}
};

template<typename T>
class Counter
{
private:
	T*     m_object;
	uint64 m_count;

	template<typename T>
	friend class SmartPtr;

	Counter(T *ptr)
	{
		m_object = ptr;
		m_count  = 1;
	}

	~Counter()
	{
		if (m_object != nullptr)
		{
			delete m_object;
		}		
	}
};

#pragma endregion