//
// LogicalDevice.h
//

#pragma once

#include "vk_util.h"

class LogicalDevice
{

protected:

	VkDevice m_device = VK_NULL_HANDLE;

public:

	LogicalDevice() {}
	LogicalDevice(const VkDevice& InDevice);
	LogicalDevice &operator=(const VkDevice &InDevice);

	virtual ~LogicalDevice() {}

public:

	operator VkDevice();
	operator VkDevice*();

	bool operator==(const VkDevice InDevice) const;

public:

	void SetVkDevice(const VkDevice& InDevice);

public:

	void FlushAll();

};