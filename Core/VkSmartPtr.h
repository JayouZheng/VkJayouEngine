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

	VkCounter<T>* _ptr_cnt;

public: 

	T* MakeInstance()
	{
		_ptr_cnt->_ptr = new T;
		*(_ptr_cnt->_ptr) = NULL;
		return _ptr_cnt->_ptr;
	}

public:

	VkSmartPtr(const char* type)
	{
		_ptr_cnt = new VkCounter<T>(nullptr);
		_ptr_cnt->SetType(type);
	}

	VkSmartPtr(const char* type, T* ptr)
	{
		_ptr_cnt = new VkCounter<T>(ptr);
		_ptr_cnt->SetType(type);
	}

	VkSmartPtr(const VkSmartPtr &other)
	{
		_ptr_cnt = other._ptr_cnt;
		_ptr_cnt->_counter++;
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

	VkSmartPtr& operator=(const VkSmartPtr& other)
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

	VkSmartPtr& operator=(const T* other)
	{
		if (this->_ptr_cnt->_ptr == other)
			return *this;

		_ptr_cnt->_counter--;
		if (_ptr_cnt->_counter == 0)
			delete _ptr_cnt;

		_ptr_cnt = new VkCounter<T>(other);

		return *this;
	}
};

template<typename T>
class VkCounter
{
private:

	T *_ptr;
	uint32 _counter;

	const char* _type;

	template<typename T>
	friend class VkSmartPtr;

	void SetType(const char* type)
	{
		_type = type;
	}

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
#define _vk_destroy(object)                                                                    \
{                                                                                              \
	if (_is_cstrlen_equal(_name_of(Vk##object), _type) &&                                      \
		_is_cstr_equal(_name_of(Vk##object), _type))                                           \
	{                                                                                          \
		vkDestroy##object(Global::GetVkDevice(), (Vk##object)*_ptr, Global::GetVkAllocator()); \
		Global::CacheLog("_vk_destroy: " + _str_name_of(object));                              \
	}                                                                                          \
}                                                                                              \

		if (_ptr != nullptr && *_ptr != NULL)
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
			if (_is_cstr_equal(_name_of(VkSurfaceKHR), _type))
			{
				vkDestroySurfaceKHR(Global::GetVkInstance(), (VkSurfaceKHR)*_ptr, Global::GetVkAllocator());
				Global::CacheLog("_vk_destroy: " + _str_name_of(VkSurfaceKHR));
			}		
		}

		delete _ptr;

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

#define _declare_vk_smart_ptr(type, var)  VkSmartPtr<type> var = VkSmartPtr<type>(_name_of(type));

#pragma endregion
