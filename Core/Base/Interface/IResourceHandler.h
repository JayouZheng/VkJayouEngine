//
// IResourceHandler.h
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Core/SmartPtr/SmartPtr.h"
#include "Core/SmartPtr/VkSmartPtr.h"

class IResourceHandler
{
public:

    virtual ~IResourceHandler() { }

protected:

    void Free() { delete this; }

    void BindRef(std::shared_ptr<IResourceHandler> InRef) { m_resource.SharedRef.push_back(InRef); }
    void BindRef(SmartPtr<IResourceHandler> InRef) { m_resource.SmartRef.push_back(InRef); }
    void BindRef(VkSmartPtr<VkObjectHandler> InRef) { m_resource.VkSmartRef.push_back(InRef); }

    std::string GetName() { return m_name; }

private:

    friend class ResourcePool;

    std::string m_name;
    

    struct InternalResource
    {
        std::vector<std::shared_ptr<IResourceHandler>> SharedRef;
        std::vector<SmartPtr<IResourceHandler>>        SmartRef;
        std::vector<VkSmartPtr<VkObjectHandler>>       VkSmartRef;

    }m_resource;
};
