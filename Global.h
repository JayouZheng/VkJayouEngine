//
// Global.h
//

#pragma once

#include "LogicalDevice.h"

namespace Global
{
	VkInstance GetVkInstance();

	void SetVkInstance(const VkInstance& InInstance);

	LogicalDevice GetLogicalDevice();

	void SetLogicalDevice(const LogicalDevice& InLogicalDevice);

	VkDevice GetVkDevice();

	void SetVkDevice(const VkDevice& InDevice);

	void Decrease();

	void Advance();

	bool IsZero();
}
