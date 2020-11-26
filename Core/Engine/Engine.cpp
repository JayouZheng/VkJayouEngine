/*********************************************************************
 *  Engine.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Engine.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Base/ResourcePool.h"
#include <mutex>

namespace
{
    static Engine* g_engine;

    static std::mutex g_enRead;
    static std::mutex g_enWrite;

    struct InternalData
    {
        BaseLayer* BaseLayerRef;
    }g_data;

    static class EngineHandler
    {
    public:

        ~EngineHandler()
        {
            if (g_engine != nullptr)
                Engine::Get()->Exit();
        }
    }EngineHandlerInstance;
}

Engine::~Engine()
{
}

Engine*& Engine::Get()
{
    std::unique_lock<std::mutex> lock_w(g_enWrite);

    if (g_engine == nullptr)
        g_engine = new Engine;

    return g_engine;
}

void Engine::Init()
{
    g_data.BaseLayerRef = BaseLayer::Create(nullptr);
    ResourcePool::Get()->Push(g_data.BaseLayerRef);
    g_data.BaseLayerRef->Init();
}

void Engine::Exit()
{
    std::unique_lock<std::mutex> lock_r(g_enRead);
    std::unique_lock<std::mutex> lock_w(g_enWrite);

    delete g_engine;
    g_engine = nullptr;

    ResourcePool::Get()->Free();
}
