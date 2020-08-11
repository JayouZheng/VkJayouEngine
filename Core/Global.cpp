//
// Global.cpp
//

#include "Global.h"
#include "BaseAllocator.h"

namespace
{
	static VkInstance     g_instance         = VK_NULL_HANDLE;
	static VkDevice       g_device           = VK_NULL_HANDLE;

	static BaseAllocator* g_allocator        = nullptr;

	static uint32         g_count            = _count_0;
	static bool           g_bDestroyManually = false;


	static std::vector<std::string>          g_logs;
}

VkInstance Global::GetVkInstance()
{
	_exit_log(g_instance == VK_NULL_HANDLE, "return [g_instance] is null!");
	return g_instance;
}

void Global::SetVkInstance(const VkInstance& InInstance)
{
	g_instance = InInstance;
}

VkDevice Global::GetVkDevice()
{
	_exit_log(g_device == VK_NULL_HANDLE, "return [g_device] is null!");
	return g_device;
}

void Global::SetVkDevice(const VkDevice& InDevice)
{
	g_device = InDevice;
}

void Global::Decrease()
{
	g_count--;
}

void Global::Advance()
{
	g_count++;
}

bool Global::IsZero()
{
	return g_count == 0;
}

void Global::ApplicationDestroyManually(bool InFlag)
{
	g_bDestroyManually = InFlag;
}

bool Global::IsDestroyManually()
{
	return g_bDestroyManually;
}

void Global::CacheLog(const std::string& InLog)
{
	g_logs.push_back(InLog);
}

void Global::PrintLog()
{
	uint32 logID = 0;
	for (auto& log : g_logs)
	{
		_cmd_print_line("Global Cached " + std::to_string(g_logs.size()) + " Logs:");
		_cmd_print_line(std::to_string(logID) + ". " + log);
	}
}

BaseAllocator* Global::GetAllocator()
{
	return g_allocator;
}

VkAllocationCallbacks* Global::GetVkAllocator()
{
	return g_allocator->GetVkAllocator();
}

void Global::SetAllocator(BaseAllocator* InAllocator)
{
	g_allocator = InAllocator;
}

void Global::SafeFreeAllocator()
{
	if (g_allocator != nullptr)
	{
		delete g_allocator;
		g_allocator = nullptr;
	}
}

void Util::PrintArgs(const char* InFormat)
{
	std::cout << InFormat;
}

bool Util::ParseJson(const std::string& InPath, Json::Value& OutRoot)
{
	std::ifstream ifs;
	ifs.open(InPath);

	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;

	if (!parseFromStream(builder, ifs, &OutRoot, &errs))
	{
		_breturn_log(EXIT_FAILURE, errs);
	}

	return EXIT_SUCCESS;
}
