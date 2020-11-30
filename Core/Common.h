/*********************************************************************
 *  Common.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  Include most commonly used files.
 *********************************************************************/

#pragma once

#include "Core/Platform/Platform.h"

#include "vulkan/vulkan.hpp"
#pragma comment(lib, "vulkan-1.lib")

#include "Core/TypeDef.h"
#include "Core/Exception.h"
#include "Core/Utility/Log/LogSystem.h"
#include "Core/SmartPtr/SmartPtr.h"
#include "Core/SmartPtr/VkSmartPtr.h"
#include "Core/Utility/Parser/PathParser.h"
#include "Core/Base/Interface/IResourceHandler.h"
#include "Core/Base/MemoryLeakCheck.h"
