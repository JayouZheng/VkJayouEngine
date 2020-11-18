//
// IResourceHandler.h
//

#pragma once

#include <string>
#include <vector>

#include "Core/SmartPtr/SmartPtr.h"
#include "Core/SmartPtr/VkSmartPtr.h"

class IResourceHandler
{
public:

    virtual ~IResourceHandler()
    {
        for (auto& ref : m_resource.CommonRefs)
            if (ref != nullptr) delete ref;
    }

    void BindRef(IResourceHandler* InRef) { m_resource.CommonRefs.push_back(InRef); }
    void BindRef(VkSmartPtr<VkObjectHandler> InRef) { m_resource.VkSmartRefs.push_back(InRef); }

    std::string GetName() { return m_name; }
    void SetName(const std::string& InName) { m_name = InName; }

private:

    friend class ResourcePool;

    std::string m_name;

    struct InternalResource
    {
        std::vector<IResourceHandler*>           CommonRefs;
        std::vector<VkSmartPtr<VkObjectHandler>> VkSmartRefs;

    }m_resource;
};

#define _declare_create_interface(type)  public: static type* Create(IResourceHandler* InParent);
#define _impl_create_interface(type)     namespace { static uint32 type##ID = 0; } type* type::Create(IResourceHandler* InParent) { type* obj = new type; if (InParent != nullptr) InParent->BindRef(obj); return obj; }
#define _internal_init(type)             SetName(StringUtil::Printf("%_%", _name_of(type), type##ID++));
