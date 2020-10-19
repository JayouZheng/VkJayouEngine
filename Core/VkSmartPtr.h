//
// VkSmartPtr.h
// Note: Use #define VK_MANUAL_DESTROY_OBJECT to manage vulkan object manually.

#pragma once

#include "Global.h"
#include "LogSystem.h"

#pragma region VkSmartPtr

template<typename T>
class VkCounter;

template<typename T>
class VkSmartPtr
{

protected:

	VkCounter<T>* m_counter;

public: 

	T* MakeInstance()
	{
		m_counter->m_object    = new T;
		*(m_counter->m_object) = VK_NULL_HANDLE;

		return m_counter->m_object;
	}

public:

	VkSmartPtr(const char* type)
	{
		m_counter = new VkCounter<T>(nullptr);
		m_counter->SetType(type);
	}

	VkSmartPtr(const char* type, T* ptr)
	{
		m_counter = new VkCounter<T>(ptr);
		m_counter->SetType(type);
	}

	VkSmartPtr(const VkSmartPtr &other)
	{
		m_counter = other.m_counter;
		m_counter->m_count++;
	}

	virtual ~VkSmartPtr()
	{
		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;
	}

public:

	operator T*()
	{
		return m_counter->m_object;
	}

	T &operator*()
	{
		return *(m_counter->m_object);
	}

	T* operator->() const throw() // no exception
	{
		return m_counter->m_object;
	}

	T** operator&()
	{
		return &m_counter->m_object;
	}

	bool operator!=(const T* other_ptr) const
	{
		return m_counter->m_object != other_ptr;
	}

	bool operator==(const T* other_ptr) const
	{
		return m_counter->m_object == other_ptr;
	}

	VkSmartPtr& operator=(const VkSmartPtr& other)
	{
		if (this == &other)
			return *this;

		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;

		m_counter = other.m_counter;
		other.m_counter->m_count++;

		return *this;
	}

	VkSmartPtr& operator=(const T* other)
	{
		if (this->m_counter->m_object == other)
			return *this;

		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;

		m_counter = new VkCounter<T>(other);

		return *this;
	}
};

template<typename T>
class VkCounter
{
private:

	T*            m_object;
	uint32        m_count;
	const char*   m_type;
	static uint32 m_instanceRefs;

	template<typename T>
	friend class VkSmartPtr;

	void SetType(const char* type)
	{
		m_type = type;
	}

	VkCounter(T *ptr)
	{
		m_object = ptr;
		m_count = 1;

		m_instanceRefs++;
	}

	~VkCounter()
	{

#ifndef VK_MANUAL_DESTROY_OBJECT
		
#ifdef _vk_destroy
#undef _vk_destroy
#endif // _vk_destroy
#define _vk_destroy(object)                                                                        \
{                                                                                                  \
	if (_is_cstrlen_equal(_name_of(Vk##object), m_type) &&                                         \
		_is_cstr_equal(_name_of(Vk##object), m_type))                                              \
	{                                                                                              \
		vkDestroy##object(Global::GetVkDevice(), (Vk##object)*m_object, Global::GetVkAllocator()); \
		LogSystem::Log("_vk_destroy: " + _str_name_of(object), LogSystem::Category::VkSmartPtr);   \
	}                                                                                              \
}                                                                                                  \

		if (m_object != nullptr && *m_object != NULL)
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
			if (_is_cstr_equal(_name_of(VkSurfaceKHR), m_type))
			{
				vkDestroySurfaceKHR(Global::GetVkInstance(), (VkSurfaceKHR)*m_object, Global::GetVkAllocator());
				LogSystem::Log("_vk_destroy: " + _str_name_of(VkSurfaceKHR), LogSystem::Category::VkSmartPtr);
			}		
		}

		m_instanceRefs--;

		if (m_instanceRefs == 0)
		{
			vkDestroyDevice(Global::GetVkDevice(), Global::GetVkAllocator());
			vkDestroyInstance(Global::GetVkInstance(), Global::GetVkAllocator());
			Global::SafeFreeAllocator();
			Global::OnExit();
		}

#endif // VK_MANUAL_DESTROY_OBJECT
		
		delete m_object;
	}
};

template<typename T>
uint32 VkCounter<T>::m_instanceRefs = 0u;

#define _declare_vk_smart_ptr(type, var)  VkSmartPtr<type> var = VkSmartPtr<type>(_name_of(type));

#pragma endregion
