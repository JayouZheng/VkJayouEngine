/*********************************************************************
 *  VkSmartPtr.cpp
 *  Copyright (C) 2021 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#include "VkSmartPtr.h"

int32 VkSmartPtr_Private::m_instanceRefs = 0;

void VkSmartPtr_Private::IncInstanceRef()
{
	m_instanceRefs++;
}

void VkSmartPtr_Private::DecInstanceRef()
{
	m_instanceRefs--;
}

bool VkSmartPtr_Private::IsInstanceRefZero()
{
	return m_instanceRefs == 0;
}
