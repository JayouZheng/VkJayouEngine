//
// VkSmartPtr.h
//

#pragma once

#include "Global.h"

#pragma region VkSmartPtr

template<typename T>
class VkCounter;

template<typename T>
class VkSmartPtr
{

protected:

	VkCounter<T> *_ptr_cnt;

public:

	void BindAllocationCallbacks(VkAllocationCallbacks* allocCallback)
	{
		_ptr_cnt->_allocation_callback = allocCallback;
	}

public:

	VkSmartPtr()
	{
		_ptr_cnt = new VkCounter<T>(new T);
	}

	VkSmartPtr(T *ptr)
	{
		_ptr_cnt = new VkCounter<T>(ptr);
	}

	VkSmartPtr(const VkSmartPtr &other)
	{
		_ptr_cnt = other._ptr_cnt;
		_ptr_cnt->_counter++;
	}

	VkSmartPtr &operator=(const VkSmartPtr &other)
	{
		if (this == &other)
			return *this;

		_ptr_cnt->_counter--;
		if (_ptr_cnt->_counter == 0)
			delete _ptr_cnt;

		_ptr_cnt = other._ptr_cnt;
		other._ptr_cnt->_counter++;

		return *this;
	}

	virtual ~VkSmartPtr()
	{
		_ptr_cnt->_counter--;
		if (_ptr_cnt->_counter == 0)
			delete _ptr_cnt;
	}

public:

	operator T*()
	{
		return _ptr_cnt->_ptr;
	}

	T &operator*()
	{
		return *(_ptr_cnt->_ptr);
	}

	T* operator->() const throw() // no exception
	{
		return _ptr_cnt->_ptr;
	}

	T** operator&()
	{
		return &_ptr_cnt->_ptr;
	}

	bool operator!=(const T* other_ptr) const
	{
		return _ptr_cnt->_ptr != other_ptr;
	}

	bool operator==(const T* other_ptr) const
	{
		return _ptr_cnt->_ptr == other_ptr;
	}
};

template<typename T>
class VkCounter
{
private:

	T *_ptr;
	int _counter;

	VkAllocationCallbacks* _allocation_callback = nullptr;

	template<typename T>
	friend class VkSmartPtr;

	VkCounter(T *ptr)
	{
		_ptr = ptr;
		_counter = 1;

		Global::Advance();
	}

	~VkCounter()
	{
		
#define _vk_destroy(object) if (std::is_same<Vk##object, T>::value) vkDestroy##object(Global::GetVkDevice(), (Vk##object)*_ptr, _allocation_callback)

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
		_vk_destroy(CommandPool);

		Global::Decrease();

		if (Global::IsZero())
		{
			vkDestroyDevice(Global::GetVkDevice(), nullptr);
			vkDestroyInstance(Global::GetVkInstance(), nullptr);
		}	

		delete _ptr;
	}
};

#pragma endregion
