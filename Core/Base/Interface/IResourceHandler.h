/*********************************************************************
 *  IResourceHandler.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A resource base class to handle the lifetime of some child resource.
 *********************************************************************/

#pragma once

#include <string>
#include <vector>

#include "Core/Utilities/SmartPtr/SmartPtr.h"
#include "Core/Utilities/SmartPtr/VkSmartPtr.h"

class IResourceHandler
{
public:
    
    /**
     *  Bind child resource/instance to current resource/instance.
     * 
     *  @param  InRef  the child resource/instance.
     */
    void BindRef(IResourceHandler* InRef) { m_resource.CommonRefs.push_back(InRef); }

    /**
     *  Bind child vulkan smart object reference to current resource/instance.
     *
     *  @param  InRef  the child vulkan smart object reference.
     */
    void BindRef(VkSmartPtr<VkObjectHandler> InRef) { m_resource.VkSmartRefs.push_back(InRef); }

    /**
     *  Resolve the name of current resource/instance.
     * 
     *  @return the name of current resource/instance.
     */
    string GetName() { return m_name; }

    /**
     *  Set the name of current resource/instance.
     * 
     *  @param  InName  the name of current resource/instance.
     */
    void SetName(const string& InName) { m_name = InName; }

    virtual ~IResourceHandler()
    {
        for (auto& ref : m_resource.CommonRefs)
            if (ref != nullptr) delete ref;
    }

private:

    friend class ResourcePool;

    string m_name;

    struct InternalResource
    {
        std::vector<IResourceHandler*>           CommonRefs;    ///< Cache common resource references.
        std::vector<VkSmartPtr<VkObjectHandler>> VkSmartRefs;   ///< Cache vulkan smart object references.

    }m_resource;
};

/**
 *  Declare Create(...) interface for specific class type.
 * 
 *  @param  type  class type.
 */
#define _declare_create_interface(type)  public: static type* Create(IResourceHandler* InParent);

/**
 *  Implement Create(...) interface for specific class type.
 * 
 *  @param  type  class type.
 */
#define _impl_create_interface(type)     namespace { static uint32 type##ID = 0; } type* type::Create(IResourceHandler* InParent) { type* obj = new type; obj->SetName(StringUtil::Printf("%_%", _name_of(type), type##ID++)); if (InParent != nullptr) InParent->BindRef(obj); return obj; }
