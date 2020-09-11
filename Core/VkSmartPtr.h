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

	T* MakeInstance()
	{
		_ptr_cnt->_ptr = new T;
		return _ptr_cnt->_ptr;
	}

public:

	VkSmartPtr()
	{
		_ptr_cnt = new VkCounter<T>(nullptr);
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
	uint64 _counter;

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

#ifdef _vk_destroy
#undef _vk_destroy
#endif
#define _vk_destroy(object) if (std::is_same<Vk##object, T>::value) { vkDestroy##object(Global::GetVkDevice(), (Vk##object)*_ptr, Global::GetVkAllocator()); Global::CacheLog("_vk_destroy: " + _str_name_of(object)); }

		if (_ptr != nullptr)
		{
			_vk_destroy(Fence);
			_vk_destroy(Semaphore); // Should Wait for all reference Object freed...
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

			// Using Semaphore...
			_vk_destroy(SwapchainKHR);
			
			// Using VkInstance...
			if (std::is_same<VkSurfaceKHR, T>::value) 
			{
				vkDestroySurfaceKHR(Global::GetVkInstance(), (VkSurfaceKHR)*_ptr, Global::GetVkAllocator());
				Global::CacheLog("_vk_destroy: " + _str_name_of(VkSurfaceKHR));
			}

			delete _ptr;
		}

		Global::Decrease();

		if (Global::IsZero())
		{
			vkDestroyDevice(Global::GetVkDevice(), Global::GetVkAllocator());
			vkDestroyInstance(Global::GetVkInstance(), Global::GetVkAllocator());
			Global::SafeFreeAllocator();
			Global::PrintLog();
		}
		
	}
};

#pragma endregion
