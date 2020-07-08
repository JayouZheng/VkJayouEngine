//
// LogicalDevice.cpp
//

#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const VkDevice& InDevice)
	: m_device(InDevice)
{
	
}

LogicalDevice &LogicalDevice::operator=(const VkDevice &InDevice)
{
	m_device = InDevice;
	return *this;
}

LogicalDevice::operator VkDevice()
{
	return m_device;
}

LogicalDevice::operator VkDevice*()
{
	return &m_device;
}

bool LogicalDevice::operator==(const VkDevice InDevice) const
{
	return m_device == InDevice;
}

void LogicalDevice::SetVkDevice(const VkDevice& InDevice)
{
	m_device = InDevice;
}

void LogicalDevice::FlushAll()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}
