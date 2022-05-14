/*********************************************************************
 *  List.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A simple linear, memory continuous list that support foreach.
 *********************************************************************/

#pragma once

#include "Core/Base/BaseType.h"
#include "../Math/Sequence.h"

template<typename T>
class List
{
public:

    List() : 
        m_data(nullptr) {}

    List(uint32 InSize) : 
        m_data(nullptr)
    {
        Resize(InSize);
    }

public:

    uint32 Length() const
    {
        return m_ids.Length();
    }

    void Clear()
    {
        delete[] m_data;
        m_data = nullptr;
    }

    void Resize(uint32 InSize)
    {
        if (m_data != nullptr)
            Clear();

        m_data = new T[InSize];
        m_ids.Resize(InSize);
    }

public:

    ~List()
    {
        Clear();
    }

    T& operator[](uint32 m_ids)
    {
        return m_data[m_ids];
    }

    T* begin()
    {
        return m_data;
    }

    T* end()
    {
        return (m_data + Length());
    }

protected:

    T* m_data;
    Math::Sequence::Natural m_ids;
};
