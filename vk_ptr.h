//
// vk_ptr.h
//

#pragma once

#include "SmartPtr.h"
#include "vulkan/vulkan.hpp"

template<typename T>
class vk_ptr : public SmartPtr<T>
{

public:

	vk_ptr()
	{
		
	}

	vk_ptr(T* ptr) :SmartPtr(ptr)
	{
		
	}

	~vk_ptr()
	{
		vkDestroyInstance(*(*this), nullptr);
	}
};

template<typename T>
SmartPtr<T> make_vk_ptr()
{
	
}