/*********************************************************************
 *  VkSmartPtr.cpp
 *  Copyright (C) 2021 Jayou. All Rights Reserved.
 * 
 *  .
 *********************************************************************/

#include "VkSmartPtr.h"

namespace
{
	static int32 g_instanceRefs = 0;
}

void VkSmartPtr_Private::IncInstanceRef()
{
	g_instanceRefs++;
}

void VkSmartPtr_Private::DecInstanceRef()
{
	g_instanceRefs--;
}

bool VkSmartPtr_Private::IsInstanceRefZero()
{
	return g_instanceRefs == 0;
}
