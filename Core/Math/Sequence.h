/*********************************************************************
 *  Sequence.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of common used sequence.
 *********************************************************************/

#pragma once

#include "Core/BaseType.h"

namespace Math
{
    namespace Sequence
    {
        class Natural
        {
        public:

            Natural() : 
                m_length(0u),
                m_sequence(nullptr) {}

            Natural(uint32 InSize) :
                m_length(0u),
                m_sequence(nullptr) 
            {
                Resize(InSize);
            }

        public:

            uint32 Length() const
            {
                return m_length;
            }

            void Clear()
            {
                delete[] m_sequence;
                m_sequence = nullptr;
            }

            void Resize(uint32 InSize)
            {
                m_length = InSize;

                if (m_sequence != nullptr)
                    Clear();

                m_sequence = new int[m_length];
                for (int i = 0; i < m_length; i++)
                    m_sequence[i] = i;
            }

        public:

            ~Natural()
            {
                Clear();
            }

            const uint32* begin()
            {
                return m_sequence;
            }

            const uint32* end()
            {
                return (m_sequence + m_length);
            }

        private:

            uint32  m_length;
            uint32* m_sequence;
        };
    }
}