//
// vk_ptr.h
//

#pragma once

#include "vk_util.h"

template<typename T>
class vk_ptr : public SmartPtr<T>
{

private:

	VkDevice _device = nullptr;
	VkAllocationCallbacks* _allocation_callback = nullptr;

public:

	vk_ptr(VkDevice device)
	{
		_device = device;
	}

	~vk_ptr()
	{

#define _vk_destroy(object) if (std::is_same<Vk##object, T>::value) vkDestroy##object(_device, *(*this), _allocation_callback)

		_vk_destroy(Fence);
		_vk_destroy(Semaphore);
		_vk_destroy(Event);
		_vk_destroy(QueryPool);
		_vk_destroy(Buffer);
		_vk_destroy(BufferView);
		_vk_destroy(Image);
		_vk_destroy(ImageView);
		_vk_destroy(ShaderModule);
		_vk_destroy(PipelineCache);
		_vk_destroy(Pipeline);
		_vk_destroy(PipelineLayout);
		_vk_destroy(Sampler);
		_vk_destroy(DescriptorSetLayout);
		_vk_destroy(DescriptorPool);
		_vk_destroy(Framebuffer);
		_vk_destroy(RenderPass);
	}


	void vkSetAllocationCallbacks(VkAllocationCallbacks* allocCallback)
	{
		_allocation_callback = allocCallback;
	}
};