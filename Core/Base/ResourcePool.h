/*********************************************************************
 *  ResourcePool.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A single instance, global access able, multithread safe resource pool.
 *********************************************************************/

#pragma once

#include "Core/SmartPtr/VkSmartPtr.h"

class IResourceHandler;

class ResourcePool
{
private:

    ResourcePool() = default;

public:

    ~ResourcePool();

    /**
     *  Get or create resouce pool instance.
     * 
     *  @return resource pool instance.
     */
    static ResourcePool*& Get();

    /**
     *  Add a new resource into pool.
     * 
     *  @param  InRef  the resource reference to add.
     */
    void Push(IResourceHandler* InRef);

    /**
     *  Add a new vulkan object smart pointer into pool.
     * 
     *  @param  InRef  the vulkan object to add.
     */
    void Push(VkSmartPtr<VkObjectHandler> InRef);
};