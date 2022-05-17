/*********************************************************************
 *  Engine.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Engine.h"
#include "Core/Base/ResourcePool.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Platform/Windows/Window.h"
#include "Core/Scene/Scene.h"
#include <mutex>

namespace
{
    Engine* g_engine = nullptr;

    std::mutex g_enRead;
    std::mutex g_enWrite;

    struct InternalData
    {
        // Global resource pointer.
        BaseLayer*           pBaseLayer = nullptr;
        Window*              pWindow    = nullptr;
        Scene*               pScene     = nullptr;

        TimerUtil::GameTimer gameTimer;
        Engine::ModuleInfo   moduleInfo;

        bool bIsEngineInitialized = false;
    }g_data;

    class EngineHandler
    {
    public:

        void Free()
        {
            std::unique_lock<std::mutex> lock_r(g_enRead);
            std::unique_lock<std::mutex> lock_w(g_enWrite);

            if (g_engine != nullptr)
            {
                delete g_engine;
                g_engine = nullptr;
            }
        }

        ~EngineHandler()
        {
            Free();
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
    g_data.pBaseLayer = BaseLayer::Create(nullptr);
    g_data.pBaseLayer->PreInit();

    ResourcePool::Get()->Push(g_data.pBaseLayer);

    g_data.pWindow = Window::Create(g_data.pBaseLayer);
    g_data.pWindow->Init();

    g_data.pBaseLayer->PostInit();

    g_data.pScene = Scene::Create(g_data.pBaseLayer);
    g_data.pScene->Init();
  
    g_data.moduleInfo = { _str_null, _str_null };

    g_data.bIsEngineInitialized = true;
}

void Engine::Run()
{
    if (!g_data.bIsEngineInitialized)
    {
        _log_error("The Engine is uninitialized!", LogSystem::Category::Engine);
        RequireExit(1);
    }

    g_data.pWindow->Show();
}

void Engine::Tick()
{
    // Engine Loop.
    if (!g_data.bIsEngineInitialized)
    {
        _log_error("The Engine is uninitialized!", LogSystem::Category::Engine);
        RequireExit(1);
    }

    g_data.gameTimer.Tick([&]()
    {
        g_data.pScene->Update(g_data.gameTimer);
        g_data.pScene->Render(g_data.gameTimer);
    });
}

void Engine::RequireExit(int32 InCode)
{
    _log_common(StringUtil::Printf("Engine::RequireExit with Code %\n ------>Suddenly exit will confuse memory laek check, never mind.", InCode), LogSystem::Category::Engine);
    exit(InCode);
}

void Engine::CacheModuleInfo(const ModuleInfo& InModuleInfo)
{
    static bool bDoOnce = false;
    if (!bDoOnce)
    {
        g_data.moduleInfo = InModuleInfo;
        bDoOnce = true;
    }
}

BaseLayer* Engine::GetBaseLayer()
{
    return g_data.pBaseLayer;
}

Window* Engine::GetWindow()
{
    return g_data.pWindow;
}

Scene* Engine::GetScene()
{
    return g_data.pScene;
}

string Engine::GetModulePath() const
{
    return g_data.moduleInfo.Path;
}

string Engine::GetModuleName() const
{
    return g_data.moduleInfo.Name;
}

WindowDesc Engine::GetWindowDesc() const
{
    if (g_data.pWindow != nullptr)
    {
        return g_data.pWindow->GetWindowDesc();
    }
    else
    {
        return WindowDesc();
    }
}
