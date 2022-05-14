/*********************************************************************
 *  EnumToString.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  Convert enum value to string for display usage.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"
#include "vulkan/vulkan.hpp"

class EnumToString
{

public:

	//---------------------------------------------------------------------------
	// Below is a table of api that convert vulkan enum to string.
	//---------------------------------------------------------------------------

	static string  VkShaderStageToString     (VkShaderStageFlags  InStage   );
	static string  VkDescriptorTypeToString  (VkDescriptorType    InDescType);
};