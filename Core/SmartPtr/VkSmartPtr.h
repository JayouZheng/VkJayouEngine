//
// VkSmartPtr.h
// Note: Use #define VK_MANUAL_DESTROY_OBJECT to manage vulkan object manually.

#pragma once

#include "Core/Global.h"
#include "Core/Utility/LogSystem.h"

#pragma region VkSmartPtr

VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkObjectHandler)

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
		m_counter->m_type = type;
	}

	VkSmartPtr(const char* type, T* ptr)
	{
		m_counter = new VkCounter<T>(ptr);
		m_counter->m_type = type;
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

	operator VkSmartPtr<VkObjectHandler>()
	{
		m_counter->m_bReleasedObjectOwnership = true;

		return VkSmartPtr<VkObjectHandler>(m_counter->m_type.data(), (VkObjectHandler*)m_counter->m_object);
	}

	operator T*()
	{
		return m_counter->m_object;
	}

	T &operator*()
	{
		return *(m_counter->m_object);
	}

	T* operator->() const
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

	VkSmartPtr& operator=(T* other)
	{
		if (this->m_counter->m_object == other)
			return *this;

		std::string type = m_counter->m_type;
		m_counter->m_count--;
		if (m_counter->m_count == 0)
			delete m_counter;

		m_counter = new VkCounter<T>(other);
		m_counter->m_type = type;

		return *this;
	}
};

template<typename T>
class VkCounter
{
private:

	T*            m_object;
	uint32        m_count;
	std::string   m_type;
	bool          m_bReleasedObjectOwnership;

	template<typename T>
	friend class VkSmartPtr;

	VkCounter(T *ptr)
	{
		m_object = ptr;
		m_count = 1;
		m_bReleasedObjectOwnership = false;

		Global::IncInstanceRef();
	}

	~VkCounter()
	{

#ifndef VK_MANUAL_DESTROY_OBJECT
		
#ifdef _vk_destroy
#undef _vk_destroy
#endif // _vk_destroy
#define _vk_destroy(object)                                                                          \
{                                                                                                    \
	if (m_type == _name_of(Vk##object))                                                              \
	{                                                                                                \
		vkDestroy##object(Global::GetVkDevice(), (Vk##object)*m_object, Global::GetVkAllocator());   \
		_log_common("_vk_destroy: " + _str_name_of(Vk##object), LogSystem::Category::VkSmartPtr); \
	}                                                                                                \
}                                                                                                    \

		if (m_object != nullptr && *m_object != NULL && !m_bReleasedObjectOwnership)
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
			if (m_type == _name_of(VkSurfaceKHR))
			{
				vkDestroySurfaceKHR(Global::GetVkInstance(), (VkSurfaceKHR)*m_object, Global::GetVkAllocator());
				_log_common("_vk_destroy: " + _str_name_of(VkSurfaceKHR), LogSystem::Category::VkSmartPtr);
			}

			Global::DecInstanceRef();

			if (Global::IsInstanceRefZero())
			{
				vkDestroyDevice(Global::GetVkDevice(), Global::GetVkAllocator());
				vkDestroyInstance(Global::GetVkInstance(), Global::GetVkAllocator());
				Global::SafeFreeAllocator();
			}

			delete m_object;
		}

#else
		delete m_object;

#endif // VK_MANUAL_DESTROY_OBJECT
	}
};

#define _declare_vk_smart_ptr(type, var)  VkSmartPtr<type> var = VkSmartPtr<type>(_name_of(type));

template<typename T>
inline VkSmartPtr<VkObjectHandler> VkCast(VkSmartPtr<T>& InObject)
{
	return (VkSmartPtr<VkObjectHandler>)InObject;
}

#pragma endregion
