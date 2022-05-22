/*********************************************************************
 *  LogicalDevice.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Core/Base/BaseLayer.h"
#include "Core/Base/ResourcePool.h"
#include "Core/Base/BaseAllocator.h"
#include "Core/Platform/Windows/Window.h"
#include "Core/Render/GLSLCompiler.h"
#include "LogicalDevice.h"
#include "RenderBaseConfig.h"
#include "CommandQueue.h"
#include "Core/Engine/Engine.h"

#include "LogicalDevice.inl"

_impl_create_interface(LogicalDevice)

LogicalDevice::LogicalDevice() : 
	m_device     (VK_NULL_HANDLE),
	m_pBaseLayer (nullptr),
	m_pAllocator (nullptr)
{
	m_pCompiler = GLSLCompiler::Create(this);
	m_pCmdQueue = CommandQueue::Create(this);
}

VkAllocationCallbacks* LogicalDevice::GetVkAllocator() const
{
	return m_pAllocator != nullptr ? m_pAllocator->GetVkAllocator() : nullptr;
}

LogicalDevice& LogicalDevice::operator=(const VkDevice& InDevice)
{
	m_device = InDevice;
	return *this;
}

LogicalDevice::~LogicalDevice()
{

}

LogicalDevice::operator VkDevice() const
{
	return m_device;
}

VkDevice LogicalDevice::GetVkDevice() const
{
	return m_device;
}

VkDevice* LogicalDevice::GetAddressOfVkDevice()
{
	return &m_device;
}

bool LogicalDevice::operator==(const VkDevice& InDevice) const
{
	return m_device == InDevice;
}

void LogicalDevice::Init(BaseLayer* InBaseLayer)
{
	m_pBaseLayer = InBaseLayer;
	m_pAllocator = InBaseLayer->GetBaseAllocator();
}

bool LogicalDevice::IsNoneAllocator() const
{
	return m_pAllocator == nullptr;
}

VkCommandPool LogicalDevice::GetCmdPool()
{
	return *m_pCmdPool;
}

CommandQueue* LogicalDevice::GetCommandQueue()
{
	return m_pCmdQueue;
}

void LogicalDevice::SetViewport(VkViewport& OutViewport, VkRect2D& OutScissor, uint32 InWidth, uint32 InHeight)
{
	OutViewport.x = 0.0f;
	OutViewport.y = 0.0f;
	OutViewport.width  = (float)InWidth;
	OutViewport.height = (float)InHeight;
	OutViewport.minDepth = 0.0f;
	OutViewport.maxDepth = 1.0f;

	OutScissor.offset = { 0, 0 };
	OutScissor.extent = { InWidth, InHeight };
}

VkQueue LogicalDevice::GetVkQueue(uint32 InQueueFamilyIndex, uint32 InQueueIndex /*= 0*/)
{
	VkQueue vkQueue = VK_NULL_HANDLE;
	vkGetDeviceQueue(m_device, InQueueFamilyIndex, InQueueIndex, &vkQueue);
	return vkQueue;
}

void LogicalDevice::GetSwapchainImagesKHR(VkSwapchainKHR InSwapchain, uint32* InOutImageCount, VkImage* OutImages)
{
	_vk_try(vkGetSwapchainImagesKHR(m_device, InSwapchain, InOutImageCount, OutImages));
}

uint32 LogicalDevice::GetSwapchainNextImageKHR(VkSwapchainKHR InSwapchain, uint64 InTimeout, VkSemaphore InSemaphore, VkFence InFence)
{
	uint32 nextImageIndex = _index_0;
	_vk_try(vkAcquireNextImageKHR(m_device, InSwapchain, InTimeout, InSemaphore, InFence, &nextImageIndex));
	return nextImageIndex;
}

VkRenderPass LogicalDevice::GetRenderPass(const string& InName)
{
	auto found = m_renderPassNamePtrMap.find(InName);
	if (found != m_renderPassNamePtrMap.end())
		return *((*found).second);
	else
	{
		_log_warning(StringUtil::Printf("%: Specified renderpass name(%) is not exit!", _name_of(GetRenderPass), InName), LogSystem::Category::LogicalDevice);
		return VK_NULL_HANDLE;
	}
}

VkPipeline LogicalDevice::GetPipeline(const string& InName)
{
	auto found = m_pipelineNamePtrMap.find(InName);
	if (found != m_pipelineNamePtrMap.end())
		return *((*found).second);
	else
	{
		_log_warning(StringUtil::Printf("%: Specified pipeline name(%) is not exit!", _name_of(GetPipeline), InName), LogSystem::Category::LogicalDevice);
		return VK_NULL_HANDLE;
	}
}

void LogicalDevice::CreateCommandPool(const VkCommandPoolCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateCommandPool(m_device, &InCreateInfo, GetVkAllocator(), m_pCmdPool.MakeInstance()));

	this->BindRef(VkCast<VkCommandPool>(m_pCmdPool));
}

void LogicalDevice::CreateCommandPool(uint32 InQueueFamilyIndex, VkCommandPoolCreateFlags InFlags /*= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT*/)
{
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.flags = InFlags;
	cmdPoolCreateInfo.queueFamilyIndex = InQueueFamilyIndex;

	_vk_try(vkCreateCommandPool(m_device, &cmdPoolCreateInfo, GetVkAllocator(), m_pCmdPool.MakeInstance()));

	this->BindRef(VkCast<VkCommandPool>(m_pCmdPool));
}

void LogicalDevice::CreateSwapchainKHR(VkSwapchainKHR* OutSwapchain, const VkSwapchainCreateInfoKHR& InCreateInfo)
{
	_vk_try(vkCreateSwapchainKHR(m_device, &InCreateInfo, GetVkAllocator(), OutSwapchain));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const VkShaderModuleCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateShaderModule(m_device, &InCreateInfo, GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const uint32* InCodes, usize InCodeSize)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pCode = InCodes;
	shaderModuleCreateInfo.codeSize = InCodeSize;

	_vk_try(vkCreateShaderModule(m_device, &shaderModuleCreateInfo, GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const Path& InShaderPath, const char* InEntrypoint /*= "main"*/, VkShaderStageFlags* OutShaderStage /*= nullptr*/)
{
	string name, ext, dir;
	StringUtil::ExtractFilePath(InShaderPath.ToString(), &name, &ext, &dir);

	VkShaderStageFlags shaderStage;
	if (!GetShaderStage(ext, shaderStage))
		Engine::Get()->RequireExit(1);

	if (OutShaderStage != nullptr)
	{
		*OutShaderStage = shaderStage;
	}

	// TODO: Use .spv shader source direclty will not get the shader refection info.
	if (shaderStage != VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM)
	{
		const char* include_dirs[_count_1] = { dir.data() };

		GLSLCompiler::CompileInfo compileInfo;
		compileInfo.shader_type = StringUtil::ToLowerCase(ext) == "hlsl" ? GLSLCompiler::ShaderType::HLSL : GLSLCompiler::ShaderType::GLSL;
		compileInfo.entrypoint = InEntrypoint;
		compileInfo.includes_count = _count_1;
		compileInfo.includes = include_dirs;
		m_pCompiler->CompileShader(shaderStage, InShaderPath, &compileInfo);
		GLSLCompiler::SPVData* spvData = m_pCompiler->GetLastSPVData();

		if (spvData->result)
		{
			this->CreateShaderModule(OutShaderModule, spvData->spv_data, spvData->spv_length);
		}
		else
		{
			*OutShaderModule = VK_NULL_HANDLE;
			_log_error(spvData->log, LogSystem::Category::GLSLCompiler);
			_log_error(spvData->debug_log, LogSystem::Category::GLSLCompiler);

			_log_error(StringUtil::Printf("Compiling shader file \"%\" failed!", InShaderPath.ToString()), LogSystem::Category::GLSLCompiler);
			Engine::Get()->RequireExit(1);
		}
	}
	else
	{
		std::vector<uint8> shaderCode;
		if (!FileUtil::ReadBinary(InShaderPath, shaderCode))
			Engine::Get()->RequireExit(1);
		this->CreateShaderModule(OutShaderModule, (uint32*)shaderCode.data(), shaderCode.size());
	}
	
	//return true;
}

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const VkComputePipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	_vk_try(vkCreateComputePipelines(m_device, InPipCache, InCreateInfoCount, InCreateInfos, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipeline(VkPipeline* OutPipeline, VkPipelineLayout InPipLayout, VkShaderModule InShaderModule, const char* InShaderEntryName /*= "main"*/, const VkSpecializationInfo* InSpecialConstInfo /*= nullptr*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	VkPipelineShaderStageCreateInfo pipSSCreateInfo = {};
	pipSSCreateInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipSSCreateInfo.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
	pipSSCreateInfo.module = InShaderModule;
	pipSSCreateInfo.pName  = InShaderEntryName;
	pipSSCreateInfo.pSpecializationInfo = InSpecialConstInfo;

	VkComputePipelineCreateInfo pipCSCreateInfo = {};
	pipCSCreateInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipCSCreateInfo.stage  = pipSSCreateInfo;
	pipCSCreateInfo.layout = InPipLayout;
	pipCSCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipCSCreateInfo.basePipelineIndex  = -1;

	_vk_try(vkCreateComputePipelines(m_device, InPipCache, _count_1, &pipCSCreateInfo, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const PipelineComputeDesc* InDescs, uint32 InDescCount/*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	VkPipelineShaderStageCreateInfo pipSSCreateInfo = {};
	pipSSCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipSSCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;

	VkComputePipelineCreateInfo* pPipCSCreateInfos = new VkComputePipelineCreateInfo[InDescCount];
	for (uint32 i = 0; i < InDescCount; ++i)
	{
		pipSSCreateInfo.module = InDescs[i].ShaderModule;
		pipSSCreateInfo.pName  = InDescs[i].EntryPoint.c_str();
		pipSSCreateInfo.pSpecializationInfo = InDescs[i].pSpecialConstInfo;

		pPipCSCreateInfos[i].sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pPipCSCreateInfos[i].stage  = pipSSCreateInfo;
		pPipCSCreateInfos[i].layout = InDescs[i].PipLayout;
		pPipCSCreateInfos[i].basePipelineHandle = InDescs[i].BasePipelineHandle;
		pPipCSCreateInfos[i].basePipelineIndex  = InDescs[i].BasePipelineIndex;
	}

	_vk_try(vkCreateComputePipelines(m_device, InPipCache, InDescCount, pPipCSCreateInfos, GetVkAllocator(), OutPipeline));
	delete[] pPipCSCreateInfos;
}

void LogicalDevice::CreatePipelineCache(VkPipelineCache* OutPipCache, const VkPipelineCacheCreateInfo& InCreateInfo)
{
	_vk_try(vkCreatePipelineCache(m_device, &InCreateInfo, GetVkAllocator(), OutPipCache));
}

void LogicalDevice::CreatePipelineCache(VkPipelineCache* OutPipCache, const void* InData, usize InSize, VkPipelineCacheCreateFlags InFlags)
{
	VkPipelineCacheCreateInfo pipelineCacheInfo = {};
	pipelineCacheInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipelineCacheInfo.initialDataSize = InSize;
	pipelineCacheInfo.pInitialData    = InData;
	pipelineCacheInfo.flags           = InFlags;

	this->CreatePipelineCache(OutPipCache, pipelineCacheInfo);
}

void LogicalDevice::CreateEmptyPipelineCache(VkPipelineCache* OutPipCache)
{
	if (m_pBaseLayer == nullptr)
	{
		*OutPipCache = VK_NULL_HANDLE;

		_log_error("Func: " + _str_name_of(CreateEmptyPipelineCache) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	PipelineCacheHeader pipCacheHeader = PipelineCacheHeader(m_pBaseLayer->GetMainPDProps());

	VkPipelineCacheCreateInfo pipCacheCreateInfo = {};
	pipCacheCreateInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipCacheCreateInfo.initialDataSize = pipCacheHeader.GetDataSize();
	pipCacheCreateInfo.pInitialData    = pipCacheHeader.GetData();

	_vk_try(vkCreatePipelineCache(m_device, &pipCacheCreateInfo, GetVkAllocator(), OutPipCache));

	//return true;
}

void LogicalDevice::CreatePipelineCacheFromFile(VkPipelineCache* OutPipCache, const Path& InPath)
{
	if (m_pBaseLayer == nullptr)
	{
		*OutPipCache = VK_NULL_HANDLE;

		_log_error("Func: " + _str_name_of(CreatePipelineCacheFromFile) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	std::vector<uint8> cacheData;

	if (!FileUtil::ReadBinary(InPath, cacheData))
		Engine::Get()->RequireExit(1);

	if (!cacheData.empty())
	{
		// clang-format off
		//
		// Check for cache validity
		//
		// TODO: Update this as the spec evolves. The fields are not defined by the header.
		//
		// The code below supports SDK 0.10 Vulkan spec, which contains the following table:
		//
		// Offset	 Size            Meaning
		// ------    ------------    ------------------------------------------------------------------
		//      0               4    a device ID equal to VkPhysicalDeviceProperties::DeviceId written
		//                           as a stream of bytes, with the least significant byte first
		//
		//      4    VK_UUID_SIZE    a pipeline cache ID equal to VkPhysicalDeviceProperties::pipelineCacheUUID
		//
		//
		// The code must be updated for latest Vulkan spec, which contains the following table:
		//
		// Offset	 Size            Meaning
		// ------    ------------    ------------------------------------------------------------------
		//      0               4    length in bytes of the entire pipeline cache header written as a
		//                           stream of bytes, with the least significant byte first
		//      4               4    a VkPipelineCacheHeaderVersion value written as a stream of bytes,
		//                           with the least significant byte first
		//      8               4    a vendor ID equal to VkPhysicalDeviceProperties::vendorID written
		//                           as a stream of bytes, with the least significant byte first
		//     12               4    a device ID equal to VkPhysicalDeviceProperties::deviceID written
		//                           as a stream of bytes, with the least significant byte first
		//     16    VK_UUID_SIZE    a pipeline cache ID equal to VkPhysicalDeviceProperties::pipelineCacheUUID
		//
		// clang-format on
		PipelineCacheHeader pipCacheHearder = {};

		memcpy(&pipCacheHearder.Length,   cacheData.data() + 0,  4);
		memcpy(&pipCacheHearder.Version,  cacheData.data() + 4,  4);
		memcpy(&pipCacheHearder.VendorID, cacheData.data() + 8,  4);
		memcpy(&pipCacheHearder.DeviceID, cacheData.data() + 12, 4);
		memcpy( pipCacheHearder.UUID,     cacheData.data() + 16, VK_UUID_SIZE);

		// Check each field and report bad values before freeing existing cache.
		bool badCache = false;

		if (pipCacheHearder.Length <= 0)
		{
			badCache = true;
			_log_error(StringUtil::Printf("Bad header length in %, value is %", InPath.ToString(), pipCacheHearder.Length), LogSystem::Category::LogicalDevice);
		}

		if (pipCacheHearder.Version != VK_PIPELINE_CACHE_HEADER_VERSION_ONE)
		{
			badCache = true;
			_log_error(StringUtil::Printf("Unsupported cache header version in %, value is %", InPath.ToString(), pipCacheHearder.Version), LogSystem::Category::LogicalDevice);
		}

		if (pipCacheHearder.VendorID != m_pBaseLayer->GetMainPDProps().vendorID)
		{
			badCache = true;
			_log_error(StringUtil::Printf("Vendor ID mismatch in %, value is %, Driver expects: %", InPath.ToString(), pipCacheHearder.VendorID, m_pBaseLayer->GetMainPDProps().vendorID), LogSystem::Category::LogicalDevice);
		}

		if (pipCacheHearder.DeviceID != m_pBaseLayer->GetMainPDProps().deviceID)
		{
			badCache = true;
			_log_error(StringUtil::Printf("Device ID mismatch in %, value is %, Driver expects: %", InPath.ToString(), pipCacheHearder.DeviceID, m_pBaseLayer->GetMainPDProps().deviceID), LogSystem::Category::LogicalDevice);
		}

		if (memcmp(pipCacheHearder.UUID, m_pBaseLayer->GetMainPDProps().pipelineCacheUUID, sizeof(pipCacheHearder.UUID)) != 0)
		{
			badCache = true;
			_log_error(StringUtil::Printf("UUID ID mismatch in %, value is %, Driver expects: %", InPath.ToString(), StringUtil::UUIDToString(pipCacheHearder.UUID), StringUtil::UUIDToString(m_pBaseLayer->GetMainPDProps().pipelineCacheUUID)), LogSystem::Category::LogicalDevice);
		}

		if (badCache)
		{
			_log_error(StringUtil::Printf("Deleting cache entry % to repopulate.", InPath.ToString()), LogSystem::Category::LogicalDevice);
			if (remove(InPath.ToCString()) != 0)
				_log_error("Deleting error", LogSystem::Category::IO);
			Engine::Get()->RequireExit(1);
		}
	}

	this->CreatePipelineCache(OutPipCache, cacheData.data(), cacheData.size());

	//return true;
}

usize LogicalDevice::GetPipelineCacheDataSize(VkPipelineCache InPipCache)
{
	usize dataSize = 0;
	_vk_try(vkGetPipelineCacheData(m_device, InPipCache, &dataSize, nullptr));
	return dataSize;
}

void LogicalDevice::GetPipelineCacheData(VkPipelineCache InPipCache, usize InDataSize, void* OutData)
{
	_vk_try(vkGetPipelineCacheData(m_device, InPipCache, &InDataSize, OutData));
}

void LogicalDevice::GetPipelineCacheData(VkPipelineCache InPipCache, std::vector<uint8>& OutData)
{
	OutData.resize(this->GetPipelineCacheDataSize(InPipCache));
	this->GetPipelineCacheData(InPipCache, OutData.size(), OutData.data());
}

void LogicalDevice::SavePipelineCacheToFile(const Path& InPath)
{
	_declare_vk_smart_ptr(VkPipelineCache, pMergedPipCache);

	this->CreateEmptyPipelineCache(pMergedPipCache.MakeInstance());
	
	// Merge old pipeline caches into new one.
	if (!m_pipelineCaches.empty())
	{
		this->MergePipelineCaches(*pMergedPipCache, m_pipelineCaches.data(), (uint32)m_pipelineCaches.size());
		m_pipelineCaches.clear();
		m_pipelineCachePtrs.clear();
	}

	m_pipelineCaches.push_back(*pMergedPipCache);
	m_pipelineCachePtrs.push_back(pMergedPipCache);

	std::vector<uint8> cacheData;
	this->GetPipelineCacheData(*pMergedPipCache, cacheData);

	if (!FileUtil::WriteBinary(InPath, cacheData))
		_log_error("Save PipelineCache to file failed!", LogSystem::Category::LogicalDevice);
}

void LogicalDevice::MergePipelineCaches(VkPipelineCache OutMergedPipCache, const VkPipelineCache* InPipCaches, uint32 InSrcPipCacheCount)
{
	_vk_try(vkMergePipelineCaches(m_device, OutMergedPipCache, InSrcPipCacheCount, InPipCaches));
}

void LogicalDevice::CreateDescriptorSetLayout(VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateDescriptorSetLayout(m_device, &InCreateInfo, GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateDescriptorSetLayout(VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutBinding* InBindings, uint32 InBindingCount)
{
	VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {};
	descSetLayoutCreateInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.bindingCount = InBindingCount;
	descSetLayoutCreateInfo.pBindings    = InBindings;

	_vk_try(vkCreateDescriptorSetLayout(m_device, &descSetLayoutCreateInfo, GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateSingleDescriptorLayout(VkDescriptorSetLayout* OutLayout, VkDescriptorType InDescType, VkShaderStageFlags InShaderStage, const VkSampler* InImmutableSamplers /*= nullptr*/)
{
	VkDescriptorSetLayoutBinding descSetLayoutBinding = {};
	descSetLayoutBinding.binding            = _index_0;
	descSetLayoutBinding.descriptorType     = InDescType;
	descSetLayoutBinding.descriptorCount    = _count_1;
	descSetLayoutBinding.stageFlags         = InShaderStage;
	descSetLayoutBinding.pImmutableSamplers = InImmutableSamplers;

	VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {};
	descSetLayoutCreateInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.bindingCount = _count_1;
	descSetLayoutCreateInfo.pBindings    = &descSetLayoutBinding;

	_vk_try(vkCreateDescriptorSetLayout(m_device, &descSetLayoutCreateInfo, GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreatePipelineLayout(VkPipelineLayout* OutLayout, const VkPipelineLayoutCreateInfo& InCreateInfo)
{
	_vk_try(vkCreatePipelineLayout(m_device, &InCreateInfo, GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreatePipelineLayout(VkPipelineLayout* OutLayout, const VkDescriptorSetLayout* InDescSetLayouts, uint32 InSetCount /*= _count_1*/, const VkPushConstantRange* InPushConstants /*= nullptr*/, uint32 InConstCount /*= _count_0*/)
{
	VkPipelineLayoutCreateInfo pipLayoutCreateInfo = {};
	pipLayoutCreateInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipLayoutCreateInfo.setLayoutCount         = InSetCount;
	pipLayoutCreateInfo.pSetLayouts            = InDescSetLayouts;
	pipLayoutCreateInfo.pushConstantRangeCount = ((InPushConstants != nullptr) && (InConstCount == 0)) ? _count_1 : InConstCount;
	pipLayoutCreateInfo.pPushConstantRanges    = InConstCount == 0 ? nullptr : InPushConstants;

	_vk_try(vkCreatePipelineLayout(m_device, &pipLayoutCreateInfo, GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateDescriptorPool(const VkDescriptorPoolCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateDescriptorPool(m_device, &InCreateInfo, GetVkAllocator(), m_pDescPool.MakeInstance()));
}

void LogicalDevice::CreateDescriptorPool(uint32 InMaxSets, const VkDescriptorPoolSize* InPerDescTypeCounts, uint32 InDescTypeCount)
{
	VkDescriptorPoolCreateInfo descPoolCreateInfo = {};
	descPoolCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolCreateInfo.maxSets       = InMaxSets;
	descPoolCreateInfo.poolSizeCount = InDescTypeCount;
	descPoolCreateInfo.pPoolSizes    = InPerDescTypeCounts;

	_vk_try(vkCreateDescriptorPool(m_device, &descPoolCreateInfo, GetVkAllocator(), m_pDescPool.MakeInstance()));
}

void LogicalDevice::AllocatorDescriptorSets(VkDescriptorSet* OutDescSet, const VkDescriptorSetAllocateInfo& InAllocateInfo)
{
	_vk_try(vkAllocateDescriptorSets(m_device, &InAllocateInfo, OutDescSet));
}

void LogicalDevice::AllocatorDescriptorSets(VkDescriptorSet* OutDescSet, const VkDescriptorSetLayout* InSetLayouts, uint32 InSetCount /*= _count_1*/)
{
	VkDescriptorSetAllocateInfo descSetAllocateInfo = {};
	descSetAllocateInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descSetAllocateInfo.descriptorPool     = *m_pDescPool;
	descSetAllocateInfo.descriptorSetCount = InSetCount;
	descSetAllocateInfo.pSetLayouts        = InSetLayouts;

	_vk_try(vkAllocateDescriptorSets(m_device, &descSetAllocateInfo, OutDescSet));
}

void LogicalDevice::FreeDescriptorSets(const VkDescriptorSet* InDescSets, uint32 InSetCount /*= _count_1*/)
{
	_vk_try(vkFreeDescriptorSets(m_device, *m_pDescPool, InSetCount, InDescSets));
}

void LogicalDevice::ResetDescriptorPool(VkDescriptorPoolResetFlags InFlags /*= _flag_none*/)
{
	_vk_try(vkResetDescriptorPool(m_device, *m_pDescPool, InFlags));
}

void LogicalDevice::UpdateDescriptorSets(const VkWriteDescriptorSet* InDescWrites, uint32 InWriteSetCount /*= _count_1*/, const VkCopyDescriptorSet* InDescCopies /*= nullptr*/, uint32 InCopySetCount /*= _count_0*/)
{
	uint32 copySetCount = ((InDescCopies != nullptr) && (InCopySetCount == 0)) ? _count_1 : InCopySetCount;
	vkUpdateDescriptorSets(m_device, InWriteSetCount, InDescWrites, copySetCount, InDescCopies);
}

void LogicalDevice::UpdateImageOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InImageDescType, const VkDescriptorImageInfo* InImageInfos, uint32 InImageCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InImageDescType != VK_DESCRIPTOR_TYPE_SAMPLER) &&
		(InImageDescType != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) &&
		(InImageDescType != VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) &&
		(InImageDescType != VK_DESCRIPTOR_TYPE_STORAGE_IMAGE) &&
		(InImageDescType != VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT))
	{
		_log_error("Func: " + _str_name_of(UpdateImageOfDescSet) + " expect image descriptor type!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet           = InDescSet;
	writeDescSet.dstBinding       = InBindingIndex;
	writeDescSet.dstArrayElement  = InSetOffset;
	writeDescSet.descriptorCount  = InImageCount;
	writeDescSet.descriptorType   = InImageDescType;
	writeDescSet.pImageInfo       = InImageInfos;
	writeDescSet.pBufferInfo      = nullptr;
	writeDescSet.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);
	
	//return true;
}

void LogicalDevice::UpdateBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InBufferDescType, const VkDescriptorBufferInfo* InBufferInfos, uint32 InBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC))
	{
		_log_error("Func: " + _str_name_of(UpdateBufferOfDescSet) + " expect buffer descriptor type!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet           = InDescSet;
	writeDescSet.dstBinding       = InBindingIndex;
	writeDescSet.dstArrayElement  = InSetOffset;
	writeDescSet.descriptorCount  = InBufferCount;
	writeDescSet.descriptorType   = InBufferDescType;
	writeDescSet.pImageInfo       = nullptr;
	writeDescSet.pBufferInfo      = InBufferInfos;
	writeDescSet.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);

	//return true;
}

void LogicalDevice::UpdateTexelBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InTBufferDescType, const VkBufferView* InTBufferViews, uint32 InTBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InTBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER) &&
		(InTBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER))
	{
		_log_error("Func: " + _str_name_of(UpdateTexelBufferOfDescSet) + " expect texel buffer descriptor type!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet           = InDescSet;
	writeDescSet.dstBinding       = InBindingIndex;
	writeDescSet.dstArrayElement  = InSetOffset;
	writeDescSet.descriptorCount  = InTBufferCount;
	writeDescSet.descriptorType   = InTBufferDescType;
	writeDescSet.pImageInfo       = nullptr;
	writeDescSet.pBufferInfo      = nullptr;
	writeDescSet.pTexelBufferView = InTBufferViews;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);

	//return true;
}

void LogicalDevice::CopyDescriptorSets(const VkCopyDescriptorSet* InDescCopies, uint32 InCopySetCount /*= _count_1*/)
{
	vkUpdateDescriptorSets(m_device, _count_0, nullptr, InCopySetCount, InDescCopies);
}

void LogicalDevice::CopyDescriptorSet(VkDescriptorSet InSrcSet, uint32 InSrcBindingIndex, VkDescriptorSet InDstSet, uint32 InDstBindingIndex, uint32 InCopyDescCount, uint32 InSrcSetOffset /*= _offset_0*/, uint32 InDstSetOffset /*= _offset_0*/)
{
	VkCopyDescriptorSet copyDescSet = {};
	copyDescSet.sType           = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
	copyDescSet.srcSet          = InSrcSet;
	copyDescSet.srcBinding      = InSrcBindingIndex;
	copyDescSet.srcArrayElement = InSrcSetOffset;
	copyDescSet.dstSet          = InDstSet;
	copyDescSet.dstBinding      = InDstBindingIndex;
	copyDescSet.dstArrayElement = InDstSetOffset;
	copyDescSet.descriptorCount = InCopyDescCount;

	vkUpdateDescriptorSets(m_device, _count_0, nullptr, _count_1, &copyDescSet);
}

void LogicalDevice::CreateSampler(VkSampler* OutSampler, const VkSamplerCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateSampler(m_device, &InCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreatePointWrapSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_FALSE;
	samplerCreateInfo.maxAnisotropy           = 1.0f;
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreatePointClampSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_FALSE;
	samplerCreateInfo.maxAnisotropy           = 1.0f;
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateLinearWrapSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_FALSE;
	samplerCreateInfo.maxAnisotropy           = 1.0f;
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateLinearClampSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_FALSE;
	samplerCreateInfo.maxAnisotropy           = 1.0f;
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateAnisotropicWrapSampler(VkSampler* OutSampler)
{
	if (m_pBaseLayer == nullptr)
	{
		OutSampler = VK_NULL_HANDLE;

		_log_error("Func: " + _str_name_of(CreateAnisotropicWrapSampler) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	float maxAnisotropy = std::min(RenderBaseConfig::Sampler::MaxAnisotropy, m_pBaseLayer->GetMainPDLimits().maxSamplerAnisotropy);

	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_TRUE;                                 // Anisotropic Filtering.
	samplerCreateInfo.maxAnisotropy           = maxAnisotropy;                           // Anisotropic Filtering.
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));

	//return true;
}

void LogicalDevice::CreateAnisotropicClampSampler(VkSampler* OutSampler)
{
	if (m_pBaseLayer == nullptr)
	{
		OutSampler = VK_NULL_HANDLE;

		_log_error("Func: " + _str_name_of(CreateAnisotropicClampSampler) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}	

	float maxAnisotropy = std::min(RenderBaseConfig::Sampler::MaxAnisotropy, m_pBaseLayer->GetMainPDLimits().maxSamplerAnisotropy);

	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter               = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_TRUE;                                 // Anisotropic Filtering.
	samplerCreateInfo.maxAnisotropy           = maxAnisotropy;                           // Anisotropic Filtering.
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_NEVER;                     // It does not matter.
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));

	//return true;
}

void LogicalDevice::CreatePCFSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo     = {};
	samplerCreateInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter               = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.mipLodBias              = 0;
	samplerCreateInfo.anisotropyEnable        = VK_FALSE;
	samplerCreateInfo.maxAnisotropy           = 1.0f; // 16.0f ?
	samplerCreateInfo.compareEnable           = VK_FALSE;
	samplerCreateInfo.compareOp               = VK_COMPARE_OP_LESS_OR_EQUAL;
	samplerCreateInfo.minLod                  = 0.0f;
	samplerCreateInfo.maxLod                  = RenderBaseConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateSampler(VkSampler* OutSampler, Render::Sampler InSamplerType)
{
	switch (InSamplerType)
	{
	case Render::Sampler::PointWrap:
		this->CreatePointWrapSampler(OutSampler);
		break;
	case Render::Sampler::PointClamp:
		this->CreatePointClampSampler(OutSampler);
		break;
	case Render::Sampler::LinearWrap:
		this->CreateLinearWrapSampler(OutSampler);
		break;
	case Render::Sampler::LinearClamp:
		this->CreateLinearClampSampler(OutSampler);
		break;
	case Render::Sampler::AnisotropicWrap:
		this->CreateAnisotropicWrapSampler(OutSampler);
		break;
	case Render::Sampler::AnisotropicClamp:
		this->CreateAnisotropicClampSampler(OutSampler);
		break;
	case Render::Sampler::PCF:
		this->CreatePCFSampler(OutSampler);
		break;
	default:
		this->CreatePointWrapSampler(OutSampler);
		break;
	}
}

void LogicalDevice::CreateRenderPass(VkRenderPass* OutRenderPass, const VkRenderPassCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateRenderPass(m_device, &InCreateInfo, GetVkAllocator(), OutRenderPass));
}

void LogicalDevice::CreateRenderPass(const string& InJsonPath)
{
	_log_common("Begin creating renderpass with " + InJsonPath, LogSystem::Category::RenderPass);

	// RenderPass.
	{
		std::vector<VkAttachmentDescription>             renderPassAttachmentDescs;
		std::vector<VkSubpassDescription>                renderPassSubpassDescs;
		std::vector<std::vector<VkAttachmentReference>>  renderPassSubpassInputAttachments;
		std::vector<std::vector<VkAttachmentReference>>  renderPassSubpassColorAttachments;
		std::vector<std::vector<VkAttachmentReference>>  renderPassSubpassResolveAttachments;
		std::vector<std::vector<uint32>>                 renderPassSubpassPreserveAttachments;
		std::vector<VkAttachmentReference>               renderPassSubpassDepthAttachments;
		std::vector<VkSubpassDependency>                 renderPassSubpassDependency;

		std::unordered_map<string, uint32>               attachmentNameIDMap;

		VkRenderPassCreateInfo renderPassCreateInfo = {};

		Json::Value renderPassRoot;
		bool        bIsArray;

		if (!JsonParser::Parse(InJsonPath, renderPassRoot))
		{
			_log_error("JsonParser failed at file: " + InJsonPath, LogSystem::Category::LogicalDevice);
			Engine::Get()->RequireExit(1);
		}

		auto& renderPassInfo = renderPassRoot[_text_mapper(vk_renderpass_info)];

		if (renderPassInfo == Json::nullValue)
		{
			_log_error("json file: [renderpass_info] can not be null!", LogSystem::Category::JsonParser);
			Engine::Get()->RequireExit(1);
		}

		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.flags = JsonParser::GetUInt32(renderPassInfo[_text_mapper(vk_flags)]);

		string renderPassName = JsonParser::GetString(renderPassInfo[_text_mapper(vk_name)]);

		// Attachment.
		bIsArray = renderPassInfo[_text_mapper(vk_attachment_descriptions)].isArray();
		uint32 numAttachDesc = bIsArray ? renderPassInfo[_text_mapper(vk_attachment_descriptions)].size() : _count_1;

		renderPassAttachmentDescs.resize(numAttachDesc);

		renderPassCreateInfo.attachmentCount = numAttachDesc;
		renderPassCreateInfo.pAttachments    = renderPassAttachmentDescs.data();

		for (uint32 j = 0; j < numAttachDesc; j++)
		{
			auto& attachment = bIsArray ? renderPassInfo[_text_mapper(vk_attachment_descriptions)][j] : renderPassInfo[_text_mapper(vk_attachment_descriptions)];

			attachmentNameIDMap.emplace(JsonParser::GetString(attachment[_text_mapper(vk_name)]), j);

			renderPassAttachmentDescs[j].flags          = JsonParser::GetUInt32(attachment[_text_mapper(vk_flags)]);
			renderPassAttachmentDescs[j].format         = GetVkFormat(JsonParser::GetString(attachment[_text_mapper(vk_format)]));
			renderPassAttachmentDescs[j].samples        = (VkSampleCountFlagBits)GetMultisampleCount(JsonParser::GetUInt32(attachment[_text_mapper(vk_sample_count)]));
			renderPassAttachmentDescs[j].loadOp         = GetVkAttachmentLoadOp(JsonParser::GetString(attachment[_text_mapper(vk_load_op)]));
			renderPassAttachmentDescs[j].storeOp        = GetVkAttachmentStoreOp(JsonParser::GetString(attachment[_text_mapper(vk_store_op)]));
			renderPassAttachmentDescs[j].stencilLoadOp  = GetVkAttachmentLoadOp(JsonParser::GetString(attachment[_text_mapper(vk_stencil_load_op)]));
			renderPassAttachmentDescs[j].stencilStoreOp = GetVkAttachmentStoreOp(JsonParser::GetString(attachment[_text_mapper(vk_stencil_store_op)]));
			renderPassAttachmentDescs[j].initialLayout  = GetVkImageLayout(JsonParser::GetString(attachment[_text_mapper(vk_in_state)]));
			renderPassAttachmentDescs[j].finalLayout    = GetVkImageLayout(JsonParser::GetString(attachment[_text_mapper(vk_out_state)]));
		}

		// Subpass.
		bIsArray = renderPassInfo[_text_mapper(vk_subpass_descriptions)].isArray();
		uint32 numSubpassDesc = bIsArray ? renderPassInfo[_text_mapper(vk_subpass_descriptions)].size() : _count_1;

		renderPassSubpassDescs.resize(numSubpassDesc);
		renderPassSubpassInputAttachments.resize(numSubpassDesc);
		renderPassSubpassColorAttachments.resize(numSubpassDesc);
		renderPassSubpassResolveAttachments.resize(numSubpassDesc);
		renderPassSubpassPreserveAttachments.resize(numSubpassDesc);
		renderPassSubpassDepthAttachments.resize(numSubpassDesc);

		renderPassCreateInfo.subpassCount = numSubpassDesc;
		renderPassCreateInfo.pSubpasses = renderPassSubpassDescs.data();

		std::unordered_map<string, uint32> subpassNameIDMap;

		for (uint32 j = 0; j < numSubpassDesc; j++)
		{
			bIsArray = renderPassInfo[_text_mapper(vk_subpass_descriptions)].isArray();
			auto& subpass = bIsArray ? renderPassInfo[_text_mapper(vk_subpass_descriptions)][j] : renderPassInfo[_text_mapper(vk_subpass_descriptions)];

			subpassNameIDMap.emplace(JsonParser::GetString(subpass[_text_mapper(vk_name)]), j);

			renderPassSubpassDescs[j].flags = JsonParser::GetUInt32(subpass[_text_mapper(vk_flags)]);
			renderPassSubpassDescs[j].pipelineBindPoint = GetVkPipelineBindPoint(JsonParser::GetString(subpass[_text_mapper(vk_pipeline_bind_point)]));

			// Input Attachment Reference.
			if (subpass[_text_mapper(vk_input_attachments)] != Json::nullValue)
			{
				bIsArray = subpass[_text_mapper(vk_input_attachments)].isArray();
				uint32 numInputAttach = bIsArray ? subpass[_text_mapper(vk_input_attachments)].size() : _count_1;

				renderPassSubpassInputAttachments[j].resize(numInputAttach);

				renderPassSubpassDescs[j].inputAttachmentCount = numInputAttach;
				renderPassSubpassDescs[j].pInputAttachments    = renderPassSubpassInputAttachments[j].data();

				for (uint32 k = 0; k < numInputAttach; k++)
				{
					auto& inputAttach = bIsArray ? subpass[_text_mapper(vk_input_attachments)][k] : subpass[_text_mapper(vk_input_attachments)];

					string name = JsonParser::GetString(inputAttach[_text_mapper(vk_attachment_name)]);

					auto found = attachmentNameIDMap.find(name);
					if (found != attachmentNameIDMap.end())
					{
						renderPassSubpassInputAttachments[j][k].attachment = (*found).second;
						renderPassSubpassInputAttachments[j][k].layout     = GetVkImageLayout(JsonParser::GetString(inputAttach[_text_mapper(vk_state)]));
					}
					else
					{
						_log_error(StringUtil::Printf("Specified attachment name \"%\" was not found!", name), LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
				}
			}
			else
			{
				renderPassSubpassDescs[j].inputAttachmentCount = _count_0;
				renderPassSubpassDescs[j].pInputAttachments    = nullptr;

				//_log_warning("Input Attachment Reference is NULL!", LogSystem::Category::RenderPass);
			}

			// Color Attachment Reference.
			if (subpass[_text_mapper(vk_color_attachments)] != Json::nullValue)
			{
				bIsArray = subpass[_text_mapper(vk_color_attachments)].isArray();
				uint32 numColorAttach = bIsArray ? subpass[_text_mapper(vk_color_attachments)].size() : _count_1;

				renderPassSubpassColorAttachments[j].resize(numColorAttach);

				renderPassSubpassDescs[j].colorAttachmentCount = numColorAttach;
				renderPassSubpassDescs[j].pColorAttachments    = renderPassSubpassColorAttachments[j].data();

				for (uint32 k = 0; k < numColorAttach; k++)
				{
					auto& colorAttach = bIsArray ? subpass[_text_mapper(vk_color_attachments)][k] : subpass[_text_mapper(vk_color_attachments)];

					string name = JsonParser::GetString(colorAttach[_text_mapper(vk_attachment_name)]);

					auto found = attachmentNameIDMap.find(name);
					if (found != attachmentNameIDMap.end())
					{
						renderPassSubpassColorAttachments[j][k].attachment = (*found).second;
						renderPassSubpassColorAttachments[j][k].layout     = GetVkImageLayout(JsonParser::GetString(colorAttach[_text_mapper(vk_state)]));
					}
					else
					{
						_log_error(StringUtil::Printf("Specified attachment name \"%\" was not found!", name), LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
				}
			}
			else
			{
				renderPassSubpassDescs[j].colorAttachmentCount = _count_0;
				renderPassSubpassDescs[j].pColorAttachments    = nullptr;

				_log_warning("Color Attachment Reference is NULL!", LogSystem::Category::RenderPass);
			}

			// Resolve Attachment Reference.
			if (subpass[_text_mapper(vk_resolve_attachments)] != Json::nullValue)
			{
				bIsArray = subpass[_text_mapper(vk_resolve_attachments)].isArray();
				uint32 numResolveAttach = bIsArray ? subpass[_text_mapper(vk_resolve_attachments)].size() : _count_1;

				renderPassSubpassResolveAttachments[j].resize(numResolveAttach);

				renderPassSubpassDescs[j].pResolveAttachments = renderPassSubpassResolveAttachments[j].data();

				for (uint32 k = 0; k < numResolveAttach; k++)
				{
					auto& resolveAttach = bIsArray ? subpass[_text_mapper(vk_resolve_attachments)][k] : subpass[_text_mapper(vk_resolve_attachments)];

					string name = JsonParser::GetString(resolveAttach[_text_mapper(vk_attachment_name)]);

					auto found = attachmentNameIDMap.find(name);
					if (found != attachmentNameIDMap.end())
					{
						renderPassSubpassResolveAttachments[j][k].attachment = (*found).second;
						renderPassSubpassResolveAttachments[j][k].layout     = GetVkImageLayout(JsonParser::GetString(resolveAttach[_text_mapper(vk_state)]));
					}
					else
					{
						_log_error(StringUtil::Printf("Specified attachment name \"%\" was not found!", name), LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
				}
			}
			else renderPassSubpassDescs[j].pResolveAttachments = nullptr;

			// Preserve Attachment ID.
			if (subpass[_text_mapper(vk_preserve_attachment_names)] != Json::nullValue)
			{
				bIsArray = subpass[_text_mapper(vk_preserve_attachment_names)].isArray();
				uint32 numPreserveAttach = bIsArray ? subpass[_text_mapper(vk_preserve_attachment_names)].size() : _count_1;

				renderPassSubpassPreserveAttachments[j].resize(numPreserveAttach);

				renderPassSubpassDescs[j].pPreserveAttachments = renderPassSubpassPreserveAttachments[j].data();

				for (uint32 k = 0; k < numPreserveAttach; k++)
				{
					auto& preserveAttach = bIsArray ? subpass[_text_mapper(vk_preserve_attachment_names)][k] : subpass[_text_mapper(vk_preserve_attachment_names)];

					string name = JsonParser::GetString(preserveAttach);

					auto found = attachmentNameIDMap.find(name);
					if (found != attachmentNameIDMap.end())
						renderPassSubpassPreserveAttachments[j][k] = (*found).second;
					else
					{
						_log_error(StringUtil::Printf("Specified attachment name \"%\" was not found!", name), LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
				}
			}
			else
			{
				renderPassSubpassDescs[j].preserveAttachmentCount = _count_0;
				renderPassSubpassDescs[j].pPreserveAttachments = nullptr;
			}

			// Depth Attachment Reference.
			if (subpass[_text_mapper(vk_depth_attachment)] != Json::nullValue)
			{
				renderPassSubpassDescs[j].pDepthStencilAttachment = &renderPassSubpassDepthAttachments[j];

				auto& depthAttach = subpass[_text_mapper(vk_depth_attachment)];

				string name = JsonParser::GetString(depthAttach[_text_mapper(vk_attachment_name)]);

				auto found = attachmentNameIDMap.find(name);
				if (found != attachmentNameIDMap.end())
				{
					renderPassSubpassDepthAttachments[j].attachment = (*found).second;
					renderPassSubpassDepthAttachments[j].layout     = GetVkImageLayout(JsonParser::GetString(depthAttach[_text_mapper(vk_state)]));
				}
				else
				{
					_log_error(StringUtil::Printf("Specified attachment name \"%\" was not found!", name), LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}
			}
			else
			{
				renderPassSubpassDescs[j].pDepthStencilAttachment = nullptr;

				_log_warning("Depth Attachment Reference is NULL!", LogSystem::Category::RenderPass);
			}
		}

		m_renderPassNameMapsubpassNameIDMap.emplace(renderPassName, subpassNameIDMap);

		// Dependency.
		bIsArray = renderPassInfo[_text_mapper(vk_subpass_dependencies)].isArray();
		uint32 numDependency = bIsArray ? renderPassInfo[_text_mapper(vk_subpass_dependencies)].size() : _count_1;

		renderPassSubpassDependency.resize(numDependency);
		renderPassCreateInfo.dependencyCount = numDependency;
		renderPassCreateInfo.pDependencies   = renderPassSubpassDependency.data();

		for (uint32 j = 0; j < numDependency; j++)
		{
			bIsArray = renderPassInfo[_text_mapper(vk_subpass_dependencies)].isArray();
			auto& dependency = bIsArray ? renderPassInfo[_text_mapper(vk_subpass_dependencies)][j] : renderPassInfo[_text_mapper(vk_subpass_dependencies)];

			string name = JsonParser::GetString(dependency[_text_mapper(vk_src_subpass_name)]);

			if (name != _str_null)
			{
				auto found_src = subpassNameIDMap.find(name);
				if (found_src != subpassNameIDMap.end())
					renderPassSubpassDependency[j].srcSubpass = (*found_src).second;
				else
				{
					_log_error(StringUtil::Printf("Specified subpass name \"%\" was not found!", name), LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}
			}
			else
			{
				renderPassSubpassDependency[j].srcSubpass = VK_SUBPASS_EXTERNAL;
			}

			name = JsonParser::GetString(dependency[_text_mapper(vk_dst_subpass_name)]);

			if (name != _str_null)
			{
				auto found_dst = subpassNameIDMap.find(name);
				if (found_dst != subpassNameIDMap.end())
					renderPassSubpassDependency[j].dstSubpass = (*found_dst).second;
				else
				{
					_log_error(StringUtil::Printf("Specified subpass name \"%\" was not found!", name), LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}
			}
			else
			{
				renderPassSubpassDependency[j].dstSubpass = VK_SUBPASS_EXTERNAL;
			}			

			renderPassSubpassDependency[j].srcStageMask = GetVkPipelineStageFlags(JsonParser::GetString(dependency[_text_mapper(vk_src_stage_mask)]));
			renderPassSubpassDependency[j].dstStageMask = GetVkPipelineStageFlags(JsonParser::GetString(dependency[_text_mapper(vk_dst_stage_mask)]));

			// Deal with Multi-Access-Flags.
			{
				bIsArray = dependency[_text_mapper(vk_src_access_mask)].isArray();
				uint32 numSrcAccessMask = bIsArray ? dependency[_text_mapper(vk_src_access_mask)].size() : _count_1;

				VkAccessFlags srcAccessMask = 0;
				for (uint32 k = 0; k < numSrcAccessMask; k++)
				{
					auto& mask = bIsArray ? dependency[_text_mapper(vk_src_access_mask)][k] : dependency[_text_mapper(vk_src_access_mask)];
					srcAccessMask |= GetVkAccessFlags(JsonParser::GetString(mask));
				}

				bIsArray = dependency[_text_mapper(vk_dst_access_mask)].isArray();
				uint32 numDstAccessMask = bIsArray ? dependency[_text_mapper(vk_dst_access_mask)].size() : _count_1;

				VkAccessFlags dstAccessMask = 0;
				for (uint32 k = 0; k < numDstAccessMask; k++)
				{
					auto& mask = bIsArray ? dependency[_text_mapper(vk_dst_access_mask)][k] : dependency[_text_mapper(vk_dst_access_mask)];
					dstAccessMask |= GetVkAccessFlags(JsonParser::GetString(mask));;
				}

				renderPassSubpassDependency[j].srcAccessMask = srcAccessMask;
				renderPassSubpassDependency[j].dstAccessMask = dstAccessMask;
			}

			renderPassSubpassDependency[j].dependencyFlags = GetVkDependencyFlags(JsonParser::GetString(dependency[_text_mapper(vk_dependency_flags)]));
		}

		_declare_vk_smart_ptr(VkRenderPass, pRenderPass);
		this->CreateRenderPass(pRenderPass.MakeInstance(), renderPassCreateInfo);
	
		m_renderPassNamePtrMap.emplace(renderPassName, pRenderPass);
	}

	_log_common("End creating renderpass with " + InJsonPath, LogSystem::Category::RenderPass);

	//return true;
}

void LogicalDevice::CreateSingleRenderPass(VkRenderPass* OutRenderPass, VkFormat InColorFormat, VkFormat InDepthFormat)
{
	std::array<VkAttachmentDescription, 2> attachments{};
	// Color attachment.
	attachments[0].format         = InColorFormat;
	attachments[0].samples        = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Depth attachment.
	attachments[1].format         = InDepthFormat;
	attachments[1].samples        = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Subpasses.
	std::array<VkSubpassDescription, 1> subpassDescriptions{};

	// First subpass...
	// ----------------------------------------------------------------------------------------

	// std::array<VkAttachmentReference, 1> inputReferences{};
	std::array<VkAttachmentReference, 1> colorReferences{};
	// std::array<VkAttachmentReference, 1> resolveReferences{};
	std::array<VkAttachmentReference, 1> depthStencilReferences{};
	// std::array<VkAttachmentReference, 1> preserveAttachmentIndices{};

	colorReferences[0]        = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL         };
	depthStencilReferences[0] = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	subpassDescriptions[0].flags                   = _flag_none;
	subpassDescriptions[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].inputAttachmentCount    = _count_0;
	subpassDescriptions[0].pInputAttachments       = nullptr;
	subpassDescriptions[0].colorAttachmentCount    = _count_1;
	subpassDescriptions[0].pColorAttachments       = colorReferences.data();
	subpassDescriptions[0].pResolveAttachments     = nullptr;
	subpassDescriptions[0].pDepthStencilAttachment = depthStencilReferences.data();
	subpassDescriptions[0].preserveAttachmentCount = _count_0;
	subpassDescriptions[0].pPreserveAttachments    = nullptr;

	// Subpass dependencies for layout transitions
	std::array<VkSubpassDependency, 1> dependencies;

	dependencies[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass      = _index_0;
	dependencies[0].srcStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;      // FrameBuffer Local.

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments    = attachments.data();
	renderPassInfo.subpassCount    = static_cast<uint32_t>(subpassDescriptions.size());
	renderPassInfo.pSubpasses      = subpassDescriptions.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies   = dependencies.data();

	_vk_try(vkCreateRenderPass(m_device, &renderPassInfo, GetVkAllocator(), OutRenderPass));
}

void LogicalDevice::CreateFrameBuffer(VkFramebuffer* OutFrameBuffer, const VkFramebufferCreateInfo& InCreateInfo)
{
	if (m_pBaseLayer == nullptr)
	{
		OutFrameBuffer = VK_NULL_HANDLE;

		_log_error("Func: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}
		
	VkFramebufferCreateInfo frameBufferCreateInfo = InCreateInfo;

	if (InCreateInfo.width  > m_pBaseLayer->GetMainPDLimits().maxFramebufferWidth  ||
		InCreateInfo.height > m_pBaseLayer->GetMainPDLimits().maxFramebufferHeight ||
		InCreateInfo.layers > m_pBaseLayer->GetMainPDLimits().maxFramebufferLayers)
	{
		_log_error(StringUtil::Printf("Buffer size exceeds physical device limit at: %", _name_of(CreateFrameBuffer)), LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	_vk_try(vkCreateFramebuffer(m_device, &InCreateInfo, GetVkAllocator(), OutFrameBuffer));

	//return true;
}

void LogicalDevice::CreateFrameBuffer(VkFramebuffer* OutFrameBuffer, VkRenderPass InRenderPass, const VkImageView* InImageViews, uint32 InViewCount, VkExtent3D InSize)
{
	if (m_pBaseLayer == nullptr)
	{
		OutFrameBuffer = VK_NULL_HANDLE;
		
		_log_error("Func: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}
		
	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.renderPass      = InRenderPass;
	frameBufferCreateInfo.attachmentCount = InViewCount;
	frameBufferCreateInfo.pAttachments    = InImageViews;
	frameBufferCreateInfo.width           = InSize.width;
	frameBufferCreateInfo.height          = InSize.height;
	frameBufferCreateInfo.layers          = InSize.depth;

	if (frameBufferCreateInfo.width  > m_pBaseLayer->GetMainPDLimits().maxFramebufferWidth  ||
		frameBufferCreateInfo.height > m_pBaseLayer->GetMainPDLimits().maxFramebufferHeight ||
		frameBufferCreateInfo.layers > m_pBaseLayer->GetMainPDLimits().maxFramebufferLayers)
	{
		_log_error(StringUtil::Printf("Buffer size exceeds physical device limit at: %", _name_of(CreateFrameBuffer)), LogSystem::Category::LogicalDevice);
		Engine::Get()->RequireExit(1);
	}

	_vk_try(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, GetVkAllocator(), OutFrameBuffer));

	//return true;
}

void LogicalDevice::CreateGraphicPipelines(VkPipeline* OutPipeline, const VkGraphicsPipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	_vk_try(vkCreateGraphicsPipelines(m_device, InPipCache, InCreateInfoCount, InCreateInfos, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateGraphicPipelines(VkPipeline* OutPipeline, const PipelineGraphicDesc* InDescs, uint32 InDescCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	const PipelineGraphicDesc& InDesc = InDescs[0];

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
		nullptr,                                             // pNext
		0,                                                   // flags
		VK_SHADER_STAGE_VERTEX_BIT,                          // stage
		InDesc.ShaderModule,                                 // module
		InDesc.EntryPoint.c_str(),                           // pName
		InDesc.pSpecialConstInfo                             // pSpecializationInfo
	};

	static const VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO, // sType
		nullptr,                                                   // pNext
		0,                                                         // flags
		0,                                                         // vertexBindingDescriptionCount
		nullptr,                                                   // pVertexBindingDescriptions
		0,                                                         // vertexAttributeDescriptionCount
		nullptr                                                    // pVertexAttributeDescriptions
	};

	static const VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, // sType
		nullptr,                                                     // pNext
		0,                                                           // flags
		VK_PRIMITIVE_TOPOLOGY_POINT_LIST,                            // topology
		VK_FALSE                                                     // primitiveRestartEnable
	};

	static const VkViewport dummyViewport = 
	{ 
		0.0f, 0.0f,                 // x, y
		1.0f, 1.0f,                 // width, height
		0.1f, 1000.0f               // minDepth, maxDepth
	};

	static const VkRect2D dummyScissor = 
	{
		{ 0, 0 },                   // offset
		{ 1, 1 }                    // extent
	};

	static const VkPipelineViewportStateCreateInfo viewportStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, // sType
		nullptr,                                               // pNext
		0,                                                     // flags
		1,                                                     // viewportCount
		&dummyViewport,                                        // pViewports
		1,                                                     // scissorCount
		&dummyScissor                                          // pScissors
	};

	static const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO, // sType
		nullptr,                                                    // pNext
		0,                                                          // flags
		VK_FALSE,                                                   // depthClampEnable
		VK_TRUE,                                                    // rasterizerDiscardEnable
		VK_POLYGON_MODE_FILL,                                       // polygonMode
		VK_CULL_MODE_NONE,                                          // cullMode
		VK_FRONT_FACE_COUNTER_CLOCKWISE,                            // frontFace
		VK_FALSE,                                                   // depthBiasEnable
		0.0f,                                                       // depthBiasConstantFactor
		0.0f,                                                       // depthBiasClamp
		0.0f,                                                       // depthBiasSlopeFactor
		0.0f                                                        // lineWidth
	};

	static const VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO, // sType
		nullptr,                                         // pNext
		0,                                               // flags
		1,                                               // stageCount
		&shaderStageCreateInfo,                          // pStages
		&vertexInputStateCreateInfo,                     // pVertexInputState
		&inputAssemblyStateCreateInfo,                   // pInputAssemblyState
		nullptr,                                         // pTessellationState
		&viewportStateCreateInfo,                        // pViewportState
		&rasterizationStateCreateInfo,                   // pRasterizationState
		nullptr,                                         // pMultisampleState
		nullptr,                                         // pDepthStencilState
		nullptr,                                         // pColorBlendState
		nullptr,                                         // pDynamicState
		InDesc.PipLayout,                                // layout
		InDesc.RenderPass,                               // renderPass
		0,                                               // subpass
		InDesc.BasePipelineHandle,                       // basePipelineHandle
		InDesc.BasePipelineIndex                         // basePipelineIndex
	};

	_vk_try(vkCreateGraphicsPipelines(m_device, InPipCache, _count_1, &graphicsPipelineCreateInfo, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateGraphicPipelines(const string& InJsonPath, VkPipelineCache InPipCache)
{
	{
		_log_common("Begin creating graphic pipeline with " + InJsonPath, LogSystem::Category::LogicalDevice);

		TimerUtil::PerformanceScope scope(_name_of(CreateGraphicPipelines));

		if (m_pBaseLayer == nullptr)
		{
			_log_error("Func: " + _str_name_of(CreateGraphicPipelines) + " expect to Query Physical Device Limits!", LogSystem::Category::LogicalDevice);
			Engine::Get()->RequireExit(1);
		}

		Json::Value root;

		if (!JsonParser::Parse(InJsonPath, root))
		{
			_log_error("Func: " + _str_name_of(CreateGraphicPipelines) + " failed! not a valid json file path!", LogSystem::Category::LogicalDevice);
			Engine::Get()->RequireExit(1);
		}

		if (root[_text_mapper(vk_graphic_pipeline_infos)] == Json::nullValue)
		{
			_log_error("json file: [graphic_pipeline_infos] can not be null!", LogSystem::Category::JsonParser);
			Engine::Get()->RequireExit(1);
		}

		bool   bIsArray = root[_text_mapper(vk_graphic_pipeline_infos)].isArray();
		uint32 numGInfo = bIsArray ? root[_text_mapper(vk_graphic_pipeline_infos)].size() : _count_1;

		WindowDesc windowDesc = Engine::Get()->GetWindowDesc();

		VkViewport currentViewport = {};
		VkRect2D   currentScissor  = {};
		this->SetViewport(currentViewport, currentScissor, windowDesc.Width, windowDesc.Height);

		LocalResourcePool localResPool;

		std::vector<VkGraphicsPipelineCreateInfo>                     graphicInfos;
		std::vector<std::vector<VkPipelineShaderStageCreateInfo>>     shaderInfos;
		std::vector<string>                                           shaderEntrypoints;
		std::vector<std::vector<VkSpecializationMapEntry>>            specMaps;
		std::vector<std::vector<uint32>>                              specData;
		std::vector<VkSpecializationInfo>                             specInfos;
		std::vector<std::vector<VkSampleMask>>                        sampleMasks;
		std::vector<VkPipelineVertexInputStateCreateInfo>             vertexInputStateInfos;
		std::vector<std::vector<VkVertexInputBindingDescription>>     vertexInputBindings;
		std::vector<std::vector<VkVertexInputAttributeDescription>>   vertexInputAttributes;
		std::vector<VkPipelineInputAssemblyStateCreateInfo>           pipelineIAStateInfos;
		std::vector<VkPipelineTessellationStateCreateInfo>            pipelineTessStateInfos;
		std::vector<VkPipelineViewportStateCreateInfo>                pipelineViewportStateInfos;
		std::vector<VkPipelineRasterizationStateCreateInfo>           pipelineRSStateInfos;
		std::vector<VkPipelineMultisampleStateCreateInfo>             pipelineMultisampleStateInfos;
		std::vector<VkPipelineDepthStencilStateCreateInfo>            pipelineDepthStencilStateInfos;
		std::vector<VkPipelineColorBlendStateCreateInfo>              pipelineColorBlendStateInfos;
		std::vector<VkPipelineDynamicStateCreateInfo>                 pipelineDynamicStateInfos;
		std::vector<std::vector<VkPipelineColorBlendAttachmentState>> colorBlendAttachmentStates;
		std::vector<std::vector<VkDynamicState>>                      dynamicStates;
		std::unordered_map<string, int32>                             basePipelineNameIDMap;

		graphicInfos.resize(numGInfo);
		shaderInfos.resize(numGInfo);
		specMaps.resize(numGInfo);
		specData.resize(numGInfo);
		specInfos.resize(numGInfo);
		sampleMasks.resize(numGInfo);
		vertexInputStateInfos.resize(numGInfo);
		vertexInputBindings.resize(numGInfo);
		vertexInputAttributes.resize(numGInfo);
		pipelineIAStateInfos.resize(numGInfo);
		pipelineTessStateInfos.resize(numGInfo);
		pipelineViewportStateInfos.resize(numGInfo);
		pipelineRSStateInfos.resize(numGInfo);
		pipelineMultisampleStateInfos.resize(numGInfo);
		pipelineDepthStencilStateInfos.resize(numGInfo);
		pipelineColorBlendStateInfos.resize(numGInfo);
		pipelineDynamicStateInfos.resize(numGInfo);
		colorBlendAttachmentStates.resize(numGInfo);
		dynamicStates.resize(numGInfo);

		for (uint32 i = 0; i < numGInfo; i++)
		{
			pipelineIAStateInfos[i] = RenderBaseConfig::Pipeline::DefaultInputAssemblyStateInfo;
			pipelineTessStateInfos[i] = RenderBaseConfig::Pipeline::DefaultTessellationStateInfo;
			pipelineViewportStateInfos[i] = RenderBaseConfig::Pipeline::DefaultViewportStateInfo;
			pipelineRSStateInfos[i] = RenderBaseConfig::Pipeline::DefaultRasterizationStateInfo;
			pipelineMultisampleStateInfos[i] = RenderBaseConfig::Pipeline::DefaultMultisampleStateInfo;
			pipelineDepthStencilStateInfos[i] = RenderBaseConfig::Pipeline::DefaultDepthStencilStateInfo;
			pipelineColorBlendStateInfos[i] = RenderBaseConfig::Pipeline::DefaultColorBlendStateInfo;
			pipelineDynamicStateInfos[i] = RenderBaseConfig::Pipeline::DefaultDynamicStateInfo;

			bIsArray = root[_text_mapper(vk_graphic_pipeline_infos)].isArray();
			auto& graphicInfo = bIsArray ? root[_text_mapper(vk_graphic_pipeline_infos)][i] : root[_text_mapper(vk_graphic_pipeline_infos)];

			basePipelineNameIDMap.emplace(JsonParser::GetString(graphicInfo[_text_mapper(vk_name)]), i);

			graphicInfos[i].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			graphicInfos[i].pNext = nullptr;
			graphicInfos[i].flags = JsonParser::GetUInt32(graphicInfo[_text_mapper(vk_flags)]);

			// Pipeline Stage.
			if (graphicInfo[_text_mapper(vk_pipeline_stages_infos)] == Json::nullValue)
			{
				_log_error("json file: [pipeline_stages_infos] can not be null!", LogSystem::Category::JsonParser);
				Engine::Get()->RequireExit(1);
			}

			bIsArray = graphicInfo[_text_mapper(vk_pipeline_stages_infos)].isArray();
			uint32 numStageInfo = bIsArray ? graphicInfo[_text_mapper(vk_pipeline_stages_infos)].size() : _count_1;

			shaderInfos[i].resize(numStageInfo);
			shaderEntrypoints.resize(numGInfo * numStageInfo);

			graphicInfos[i].stageCount = numStageInfo;
			graphicInfos[i].pStages = shaderInfos[i].data();

			for (uint32 j = 0; j < numStageInfo; j++)
			{
				bIsArray = graphicInfo[_text_mapper(vk_pipeline_stages_infos)].isArray();
				auto& shaderInfo = bIsArray ? graphicInfo[_text_mapper(vk_pipeline_stages_infos)][j] : graphicInfo[_text_mapper(vk_pipeline_stages_infos)];

				string shaderPath = JsonParser::GetString(shaderInfo[_text_mapper(vk_stage_code_path)]);
				if (shaderPath == _str_null)
				{
					_log_error("json file: [stage_code_path] can not be null!", LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}

				shaderEntrypoints[i * numStageInfo + j] = JsonParser::GetString(shaderInfo[_text_mapper(vk_entrypoint)], "main");

				_declare_vk_smart_ptr(VkShaderModule, pShaderModule);
				VkShaderStageFlags currentShaderStage, userDefinedShaderStage;
				this->CreateShaderModule(pShaderModule.MakeInstance(), Path(shaderPath), shaderEntrypoints[i * numStageInfo + j].c_str(), &currentShaderStage);

				localResPool.Push(VkCast<VkShaderModule>(pShaderModule));

				shaderInfos[i][j].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderInfos[i][j].pNext = nullptr;
				shaderInfos[i][j].flags = JsonParser::GetUInt32(shaderInfo[_text_mapper(vk_flags)]);
				shaderInfos[i][j].stage = (VkShaderStageFlagBits)((shaderInfo[_text_mapper(vk_stage_type)] != Json::nullValue) ? (GetShaderStage(JsonParser::GetString(shaderInfo[_text_mapper(vk_stage_type)]), userDefinedShaderStage) ? userDefinedShaderStage : currentShaderStage) : currentShaderStage);
				shaderInfos[i][j].module = *pShaderModule;
				shaderInfos[i][j].pName = shaderEntrypoints[i * numStageInfo + j].c_str();
				shaderInfos[i][j].pSpecializationInfo = &specInfos[i];

				if (shaderInfo[_text_mapper(vk_specialization_constants)] != Json::nullValue)
				{
					bIsArray = shaderInfo[_text_mapper(vk_specialization_constants)].isArray();
					uint32 numSpecConst = bIsArray ? shaderInfo[_text_mapper(vk_specialization_constants)].size() : _count_1;

					specMaps[i].resize(numSpecConst);
					specData[i].resize(numSpecConst);

					specInfos[i].mapEntryCount = numSpecConst;
					specInfos[i].pMapEntries = specMaps[i].data();
					specInfos[i].dataSize = numSpecConst * 4; // 4 byte per const, 32 bit value.
					specInfos[i].pData = specData[i].data();

					for (uint32 k = 0; k < numSpecConst; k++)
					{
						auto& value = bIsArray ? shaderInfo[_text_mapper(vk_specialization_constants)][k] : shaderInfo[_text_mapper(vk_specialization_constants)];

						specMaps[i][k].constantID = k;
						specMaps[i][k].offset = k * 4; // 4 byte per const, 32 bit value.
						specMaps[i][k].size = 4;     // 4 byte per const, 32 bit value.

						//////////////////////////////////////////////////////////////
						// json value reinterpretation.
						switch (value.type())
						{
							case Json::ValueType::intValue:     specMaps[i][k].size = sizeof(int32);    _reinterpret_data(specData[i][k], value.asInt());   break;
							case Json::ValueType::uintValue:    specMaps[i][k].size = sizeof(uint32);   _reinterpret_data(specData[i][k], value.asUInt());  break;
							case Json::ValueType::realValue:    specMaps[i][k].size = sizeof(float);    _reinterpret_data(specData[i][k], value.asFloat()); break;
							case Json::ValueType::booleanValue: specMaps[i][k].size = sizeof(VkBool32); _reinterpret_data(specData[i][k], value.asBool());  break;
							default:
							{
								_log_error("json file: not support [specialization_constants] value type!", LogSystem::Category::JsonParser);
								Engine::Get()->RequireExit(1);
							}
						}
						//////////////////////////////////////////////////////////////
					}
				}
				else
				{
					shaderInfos[i][j].pSpecializationInfo = nullptr;
				}
			}

			// Pipeline Layout.
			std::vector<VkPushConstantRange> pushConstantRanges;
			std::vector<std::vector<VkDescriptorSetLayoutBinding>> descSets;

			if (!m_pCompiler->CheckAndParseSPVData(m_pBaseLayer->GetMainPDLimits().maxBoundDescriptorSets, pushConstantRanges, descSets))
				Engine::Get()->RequireExit(1);

			m_pCompiler->FlushSPVData();

			// Vertex Input State.
			graphicInfos[i].pVertexInputState = &vertexInputStateInfos[i];
			if (graphicInfo[_text_mapper(vk_vertex_input_attributes)] == Json::nullValue)
			{
				_log_error("json file: [vertex_input_attributes] can not be null!", LogSystem::Category::JsonParser);
				Engine::Get()->RequireExit(1);
			}

			// Bindings.
			bIsArray = graphicInfo[_text_mapper(vk_vertex_input_attributes)].isArray();
			uint32 numBinding = bIsArray ? graphicInfo[_text_mapper(vk_vertex_input_attributes)].size() : _count_1;

			vertexInputBindings[i].resize(numBinding);

			for (uint32 j = 0; j < numBinding; j++)
			{
				bIsArray = graphicInfo[_text_mapper(vk_vertex_input_attributes)].isArray();
				auto& binding = bIsArray ? graphicInfo[_text_mapper(vk_vertex_input_attributes)][j] : graphicInfo[_text_mapper(vk_vertex_input_attributes)];

				uint32 bindingID = binding[_text_mapper(vk_binding_id)] != Json::nullValue ? binding[_text_mapper(vk_binding_id)].asUInt() : j;

				if (j >= 16u)
				{
					_log_warning("current app vertex input binding number is limit to 16!", LogSystem::Category::LogicalDevice);
					break;
				}

				if (bindingID >= 16u)
				{
					_log_warning("Current app vertex input binding number is limit to 16!", LogSystem::Category::LogicalDevice);
					continue;
				}

				// Vertex Attributes.
				bIsArray = binding[_text_mapper(vk_attributes)].isArray();
				uint32 numAttribute = bIsArray ? binding[_text_mapper(vk_attributes)].size() : _count_1;

				vertexInputAttributes[i].resize(numAttribute);

				uint32  attributeOffset = 0;
				uint32& allAttributeSize = attributeOffset;
				for (uint32 k = 0; k < numAttribute; k++)
				{
					string attribute = bIsArray ? binding[_text_mapper(vk_attributes)][k].asString() : binding[_text_mapper(vk_attributes)].asString();

					vertexInputAttributes[i][k].binding = _index_0;
					vertexInputAttributes[i][k].location = k;
					vertexInputAttributes[i][k].format = GetVertexAttributeVkFormat(attribute);
					vertexInputAttributes[i][k].offset = attributeOffset;

					attributeOffset += GetVertexAttributeSize(attribute);
				}

				vertexInputBindings[i][j].binding = bindingID;
				vertexInputBindings[i][j].stride = allAttributeSize;
				vertexInputBindings[i][j].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				vertexInputStateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vertexInputStateInfos[i].vertexBindingDescriptionCount = numBinding;
				vertexInputStateInfos[i].pVertexBindingDescriptions = vertexInputBindings[i].data();
				vertexInputStateInfos[i].vertexAttributeDescriptionCount = numAttribute;
				vertexInputStateInfos[i].pVertexAttributeDescriptions = vertexInputAttributes[i].data();
			}

			// IA State.
			graphicInfos[i].pInputAssemblyState = &pipelineIAStateInfos[i];
			auto& inputAssemblyInfo = graphicInfo[_text_mapper(vk_pipeline_input_assembly)];
			if (inputAssemblyInfo != Json::nullValue)
			{
				pipelineIAStateInfos[i].flags = JsonParser::GetUInt32(inputAssemblyInfo[_text_mapper(vk_flags)]);
				pipelineIAStateInfos[i].topology = GetVkPrimitiveTopology(JsonParser::GetString(inputAssemblyInfo[_text_mapper(vk_primitive_topology)]));
				pipelineIAStateInfos[i].primitiveRestartEnable = JsonParser::GetUInt32(inputAssemblyInfo[_text_mapper(vk_primitive_restart_enable)]);
			}

			// Tessellation State.
			auto& tessellationInfo = graphicInfo[_text_mapper(vk_tessellation_state)];
			if (tessellationInfo != Json::nullValue)
			{
				graphicInfos[i].pTessellationState = &pipelineTessStateInfos[i];
				pipelineTessStateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
				pipelineTessStateInfos[i].flags = JsonParser::GetUInt32(tessellationInfo[_text_mapper(vk_flags)]);
				pipelineTessStateInfos[i].patchControlPoints = JsonParser::GetUInt32(tessellationInfo[_text_mapper(vk_patch_control_points_count)]);
			}
			else
			{
				graphicInfos[i].pTessellationState = nullptr;
			}

			// Viewport State.
			graphicInfos[i].pViewportState = &pipelineViewportStateInfos[i];
			auto& viewportInfo = graphicInfo[_text_mapper(vk_viewport_state)];
			if (viewportInfo != Json::nullValue)
			{
				bIsArray = viewportInfo[_text_mapper(vk_viewports)].isArray();
				uint32 numViewport = bIsArray ? viewportInfo[_text_mapper(vk_viewports)].size() : _count_1;

				pipelineViewportStateInfos[i].flags = JsonParser::GetUInt32(viewportInfo[_text_mapper(vk_flags)]);
				pipelineViewportStateInfos[i].viewportCount = numViewport;
				pipelineViewportStateInfos[i].scissorCount = numViewport;
				pipelineViewportStateInfos[i].pViewports = &currentViewport;
				pipelineViewportStateInfos[i].pScissors = &currentScissor;

				for (uint32 j = 0; j < numViewport; j++)
				{
					auto& viewport = bIsArray ? viewportInfo[_text_mapper(vk_viewports)][j] : viewportInfo[_text_mapper(vk_viewports)];
					auto& scissor = viewportInfo[_text_mapper(vk_scissor_rectangles)].isArray() ? viewportInfo[_text_mapper(vk_scissor_rectangles)][j] : viewportInfo[_text_mapper(vk_scissor_rectangles)];

					if (!viewport[_text_mapper(vk_position)].isArray())
					{
						_log_error("json file: viewport [position] must be an array [ first, second ]!", LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
					if (!viewport[_text_mapper(vk_size)].isArray())
					{
						_log_error("json file: viewport [size] must be an array [ first, second ]!", LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
					if (!viewport[_text_mapper(vk_depth_range)].isArray())
					{
						_log_error("json file: viewport [depth_range] must be an array [ first, second ]!", LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
					if (!scissor[_text_mapper(vk_offset)].isArray())
					{
						_log_error("json file: scissor [offset] must be an array [ first, second ]!", LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}
					if (!scissor[_text_mapper(vk_size)].isArray())
					{
						_log_error("json file: scissor [size] must be an array [ first, second ]!", LogSystem::Category::JsonParser);
						Engine::Get()->RequireExit(1);
					}

					currentViewport.x = JsonParser::GetFloat(viewport[_text_mapper(vk_position)][0]);
					currentViewport.y = JsonParser::GetFloat(viewport[_text_mapper(vk_position)][1]);
					currentViewport.width = JsonParser::GetString(viewport[_text_mapper(vk_size)][0]) == "auto" ? (float)windowDesc.Width : JsonParser::GetFloat(viewport[_text_mapper(vk_size)][0]);
					currentViewport.height = JsonParser::GetString(viewport[_text_mapper(vk_size)][1]) == "auto" ? (float)windowDesc.Height : JsonParser::GetFloat(viewport[_text_mapper(vk_size)][1]);
					currentViewport.minDepth = JsonParser::GetFloat(viewport[_text_mapper(vk_depth_range)][0]);
					currentViewport.maxDepth = JsonParser::GetFloat(viewport[_text_mapper(vk_depth_range)][1]);

					currentScissor.offset.x = JsonParser::GetInt32(scissor[_text_mapper(vk_offset)][0]);
					currentScissor.offset.y = JsonParser::GetInt32(scissor[_text_mapper(vk_offset)][1]);
					currentScissor.extent.width = JsonParser::GetString(scissor[_text_mapper(vk_size)][0]) == "auto" ? windowDesc.Width : JsonParser::GetUInt32(scissor[_text_mapper(vk_size)][0]);
					currentScissor.extent.height = JsonParser::GetString(scissor[_text_mapper(vk_size)][1]) == "auto" ? windowDesc.Height : JsonParser::GetUInt32(scissor[_text_mapper(vk_size)][1]);
				}
			}

			// RS State.
			graphicInfos[i].pRasterizationState = &pipelineRSStateInfos[i];
			auto& rasterizationInfo = graphicInfo[_text_mapper(vk_rasterization_state)];
			if (rasterizationInfo != Json::nullValue)
			{
				pipelineRSStateInfos[i].flags = JsonParser::GetUInt32(rasterizationInfo[_text_mapper(vk_flags)]);
				pipelineRSStateInfos[i].depthClampEnable = JsonParser::GetUInt32(rasterizationInfo[_text_mapper(vk_depth_clamp_enable)]);
				pipelineRSStateInfos[i].rasterizerDiscardEnable = JsonParser::GetUInt32(rasterizationInfo[_text_mapper(vk_rasterizer_discard_enable)]);
				pipelineRSStateInfos[i].polygonMode = GetVkPolygonMode(JsonParser::GetString(rasterizationInfo[_text_mapper(vk_polygon_mode)]));
				pipelineRSStateInfos[i].cullMode = GetVkCullModeFlags(JsonParser::GetString(rasterizationInfo[_text_mapper(vk_cull_mode)]));
				pipelineRSStateInfos[i].frontFace = GetVkFrontFace(JsonParser::GetString(rasterizationInfo[_text_mapper(vk_front_face)]));
				pipelineRSStateInfos[i].depthBiasEnable = JsonParser::GetUInt32(rasterizationInfo[_text_mapper(vk_depth_bias_enable)]);
				pipelineRSStateInfos[i].depthBiasConstantFactor = JsonParser::GetFloat(rasterizationInfo[_text_mapper(vk_depth_bias_constant_factor)]);
				pipelineRSStateInfos[i].depthBiasClamp = JsonParser::GetFloat(rasterizationInfo[_text_mapper(vk_depth_bias_clamp)]);
				pipelineRSStateInfos[i].depthBiasSlopeFactor = JsonParser::GetFloat(rasterizationInfo[_text_mapper(vk_depth_bias_slope_factor)]);
				pipelineRSStateInfos[i].lineWidth = JsonParser::GetFloat(rasterizationInfo[_text_mapper(vk_line_width)]);
			}

			// Multisample State.
			graphicInfos[i].pMultisampleState = &pipelineMultisampleStateInfos[i];
			auto& multisampleInfo = graphicInfo[_text_mapper(vk_multisample_state)];
			if (multisampleInfo != Json::nullValue)
			{
				bIsArray = multisampleInfo[_text_mapper(vk_sample_masks)].isArray();
				uint32 numSampleMask = bIsArray ? multisampleInfo[_text_mapper(vk_sample_masks)].size() : _count_1;

				sampleMasks[i].resize(numSampleMask);

				for (uint32 j = 0; j < numSampleMask; j++)
					sampleMasks[i][j] = bIsArray ? multisampleInfo[_text_mapper(vk_sample_masks)][j].asUInt() : multisampleInfo[_text_mapper(vk_sample_masks)].asUInt();

				pipelineMultisampleStateInfos[i].flags = JsonParser::GetUInt32(multisampleInfo[_text_mapper(vk_flags)]);
				pipelineMultisampleStateInfos[i].rasterizationSamples = (VkSampleCountFlagBits)GetMultisampleCount(JsonParser::GetUInt32(multisampleInfo[_text_mapper(vk_multisample_count)]));
				pipelineMultisampleStateInfos[i].sampleShadingEnable = JsonParser::GetUInt32(multisampleInfo[_text_mapper(vk_sample_shading_enable)]);
				pipelineMultisampleStateInfos[i].minSampleShading = JsonParser::GetFloat(multisampleInfo[_text_mapper(vk_min_sample_shading_factor)]);
				pipelineMultisampleStateInfos[i].pSampleMask = sampleMasks[i].data();
				pipelineMultisampleStateInfos[i].alphaToCoverageEnable = JsonParser::GetUInt32(multisampleInfo[_text_mapper(vk_alpha_to_coverage_enable)]);
				pipelineMultisampleStateInfos[i].alphaToOneEnable = JsonParser::GetUInt32(multisampleInfo[_text_mapper(vk_alpha_to_one_enable)]);
			}

			// Depth Stencil State.
			graphicInfos[i].pDepthStencilState = &pipelineDepthStencilStateInfos[i];
			auto& depthStencilInfo = graphicInfo[_text_mapper(vk_depth_stencil_state)];
			if (depthStencilInfo != Json::nullValue)
			{
				pipelineDepthStencilStateInfos[i].flags = JsonParser::GetUInt32(depthStencilInfo[_text_mapper(vk_flags)]);
				pipelineDepthStencilStateInfos[i].depthTestEnable = JsonParser::GetUInt32(depthStencilInfo[_text_mapper(vk_depth_test_enable)]);
				pipelineDepthStencilStateInfos[i].depthWriteEnable = JsonParser::GetUInt32(depthStencilInfo[_text_mapper(vk_depth_write_enable)]);
				pipelineDepthStencilStateInfos[i].depthCompareOp = GetVkCompareOp(JsonParser::GetString(depthStencilInfo[_text_mapper(vk_depth_compare_op)]));
				pipelineDepthStencilStateInfos[i].depthBoundsTestEnable = JsonParser::GetUInt32(depthStencilInfo[_text_mapper(vk_depth_bounds_test_enable)]);
				pipelineDepthStencilStateInfos[i].stencilTestEnable = JsonParser::GetUInt32(depthStencilInfo[_text_mapper(vk_stencil_test_enable)]);
				pipelineDepthStencilStateInfos[i].minDepthBounds = JsonParser::GetFloat(depthStencilInfo[_text_mapper(vk_min_depth_bounds)]);
				pipelineDepthStencilStateInfos[i].maxDepthBounds = JsonParser::GetFloat(depthStencilInfo[_text_mapper(vk_max_depth_bounds)]);

				auto& stencilInfo = depthStencilInfo[_text_mapper(vk_stencil_test_state)];
				if (stencilInfo != Json::nullValue)
				{
					if (JsonParser::GetString(stencilInfo[_text_mapper(vk_front)]) != "auto")
					{
						pipelineDepthStencilStateInfos[i].front.failOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_fail_op)]));
						pipelineDepthStencilStateInfos[i].front.passOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_pass_op)]));
						pipelineDepthStencilStateInfos[i].front.depthFailOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_depth_fail_op)]));
						pipelineDepthStencilStateInfos[i].front.compareOp = GetVkCompareOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_compare_op)]));
						pipelineDepthStencilStateInfos[i].front.compareMask = StringUtil::StrHexToNumeric(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_compare_mask)], "0x00"));
						pipelineDepthStencilStateInfos[i].front.writeMask = StringUtil::StrHexToNumeric(JsonParser::GetString(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_write_mask)], "0x00"));
						pipelineDepthStencilStateInfos[i].front.reference = JsonParser::GetUInt32(stencilInfo[_text_mapper(vk_front)][_text_mapper(vk_reference)]);
					}

					if (JsonParser::GetString(stencilInfo[_text_mapper(vk_back)]) != "auto")
					{
						pipelineDepthStencilStateInfos[i].back.failOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_fail_op)]));
						pipelineDepthStencilStateInfos[i].back.passOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_pass_op)]));
						pipelineDepthStencilStateInfos[i].back.depthFailOp = GetVkStencilOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_depth_fail_op)]));
						pipelineDepthStencilStateInfos[i].back.compareOp = GetVkCompareOp(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_compare_op)]));
						pipelineDepthStencilStateInfos[i].back.compareMask = StringUtil::StrHexToNumeric(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_compare_mask)], "0x00"));
						pipelineDepthStencilStateInfos[i].back.writeMask = StringUtil::StrHexToNumeric(JsonParser::GetString(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_write_mask)], "0x00"));
						pipelineDepthStencilStateInfos[i].back.reference = JsonParser::GetUInt32(stencilInfo[_text_mapper(vk_back)][_text_mapper(vk_reference)]);
					}

					if (JsonParser::GetString(stencilInfo[_text_mapper(vk_front)]) == "auto")
						pipelineDepthStencilStateInfos[i].front = pipelineDepthStencilStateInfos[i].back;
					if (JsonParser::GetString(stencilInfo[_text_mapper(vk_back)]) == "auto")
						pipelineDepthStencilStateInfos[i].back = pipelineDepthStencilStateInfos[i].front;
				}
			}

			// Color Blend State.
			graphicInfos[i].pColorBlendState = &pipelineColorBlendStateInfos[i];
			auto& colorBlendInfo = graphicInfo[_text_mapper(vk_color_blend_state)];
			if (colorBlendInfo != Json::nullValue && colorBlendInfo[_text_mapper(vk_attachments)] != Json::nullValue)
			{
				bIsArray = colorBlendInfo[_text_mapper(vk_attachments)].isArray();
				uint32 numAttachment = bIsArray ? colorBlendInfo[_text_mapper(vk_attachments)].size() : _count_1;

				colorBlendAttachmentStates[i].resize(numAttachment);

				for (uint32 j = 0; j < numAttachment; j++)
				{
					auto& attachment = bIsArray ? colorBlendInfo[_text_mapper(vk_attachments)][j] : colorBlendInfo[_text_mapper(vk_attachments)];

					colorBlendAttachmentStates[i][j].blendEnable = JsonParser::GetUInt32(attachment[_text_mapper(vk_blend_enable)]);
					colorBlendAttachmentStates[i][j].srcColorBlendFactor = attachment[_text_mapper(vk_src_color_factor)].isUInt() ? static_cast<VkBlendFactor>(JsonParser::GetUInt32(attachment[_text_mapper(vk_src_color_factor)])) : GetVkBlendFactor(JsonParser::GetString(attachment[_text_mapper(vk_src_color_factor)]));
					colorBlendAttachmentStates[i][j].dstColorBlendFactor = attachment[_text_mapper(vk_dst_color_factor)].isUInt() ? static_cast<VkBlendFactor>(JsonParser::GetUInt32(attachment[_text_mapper(vk_dst_color_factor)])) : GetVkBlendFactor(JsonParser::GetString(attachment[_text_mapper(vk_dst_color_factor)]));
					colorBlendAttachmentStates[i][j].colorBlendOp = GetVkBlendOp(JsonParser::GetString(attachment[_text_mapper(vk_color_blend_op)]));
					colorBlendAttachmentStates[i][j].srcAlphaBlendFactor = attachment[_text_mapper(vk_src_alpha_factor)].isUInt() ? static_cast<VkBlendFactor>(JsonParser::GetUInt32(attachment[_text_mapper(vk_src_alpha_factor)])) : GetVkBlendFactor(JsonParser::GetString(attachment[_text_mapper(vk_src_alpha_factor)]));
					colorBlendAttachmentStates[i][j].dstAlphaBlendFactor = attachment[_text_mapper(vk_dst_alpha_factor)].isUInt() ? static_cast<VkBlendFactor>(JsonParser::GetUInt32(attachment[_text_mapper(vk_dst_alpha_factor)])) : GetVkBlendFactor(JsonParser::GetString(attachment[_text_mapper(vk_dst_alpha_factor)]));
					colorBlendAttachmentStates[i][j].alphaBlendOp = GetVkBlendOp(JsonParser::GetString(attachment[_text_mapper(vk_alpha_blend_op)]));
					colorBlendAttachmentStates[i][j].colorWriteMask = GetColorComponentMask(JsonParser::GetString(attachment[_text_mapper(vk_component_mask)]));
				}

				pipelineColorBlendStateInfos[i].flags = JsonParser::GetUInt32(colorBlendInfo[_text_mapper(vk_flags)]);
				pipelineColorBlendStateInfos[i].logicOpEnable = JsonParser::GetUInt32(colorBlendInfo[_text_mapper(vk_logic_op_enable)]);
				pipelineColorBlendStateInfos[i].logicOp = GetVkLogicOp(JsonParser::GetString(colorBlendInfo[_text_mapper(vk_logic_op)]));
				pipelineColorBlendStateInfos[i].attachmentCount = numAttachment;
				pipelineColorBlendStateInfos[i].pAttachments = colorBlendAttachmentStates[i].data();

				bIsArray = colorBlendInfo[_text_mapper(vk_blend_constants)].isArray();
				uint32 numConstant = bIsArray ? colorBlendInfo[_text_mapper(vk_blend_constants)].size() : _count_1;
				for (uint32 j = 0; j < numConstant; j++)
				{
					if (j >= 4) break;
					pipelineColorBlendStateInfos[i].blendConstants[j] = JsonParser::GetFloat(bIsArray ? colorBlendInfo[_text_mapper(vk_blend_constants)][j] : colorBlendInfo[_text_mapper(vk_blend_constants)]);
				}
			}

			// Dynamic State.
			graphicInfos[i].pDynamicState = &pipelineDynamicStateInfos[i];
			auto& dynamicStateInfo = graphicInfo[_text_mapper(vk_dynamic_state)];
			if (dynamicStateInfo != Json::nullValue && dynamicStateInfo[_text_mapper(vk_state)] != Json::nullValue)
			{
				bIsArray = dynamicStateInfo[_text_mapper(vk_state)].isArray();
				uint32 numDynamicState = bIsArray ? dynamicStateInfo[_text_mapper(vk_state)].size() : _count_1;

				dynamicStates[i].resize(numDynamicState);

				for (uint32 j = 0; j < numDynamicState; j++)
					dynamicStates[i][j] = GetVkDynamicState(JsonParser::GetString(bIsArray ? dynamicStateInfo[_text_mapper(vk_state)][j] : dynamicStateInfo[_text_mapper(vk_state)]));

				pipelineDynamicStateInfos[i].flags = JsonParser::GetUInt32(dynamicStateInfo[_text_mapper(vk_flags)]);
				pipelineDynamicStateInfos[i].dynamicStateCount = numDynamicState;
				pipelineDynamicStateInfos[i].pDynamicStates = dynamicStates[i].data();
			}

			// Pipeline Layout.
			std::vector<VkDescriptorSetLayout> descSetLayouts;

			for (auto& bindings : descSets)
			{
				_declare_vk_smart_ptr(VkDescriptorSetLayout, pDescSetLayout);

				this->CreateDescriptorSetLayout(pDescSetLayout.MakeInstance(), bindings.data(), (uint32)bindings.size());
				descSetLayouts.push_back(*pDescSetLayout);
				localResPool.Push(VkCast<VkDescriptorSetLayout>(pDescSetLayout));
			}

			_declare_vk_smart_ptr(VkPipelineLayout, pPipelineLayout);

			this->CreatePipelineLayout(pPipelineLayout.MakeInstance(), descSetLayouts.data(), (uint32)descSetLayouts.size(), pushConstantRanges.data(), (uint32)pushConstantRanges.size());
			localResPool.Push(VkCast<VkPipelineLayout>(pPipelineLayout));

			graphicInfos[i].layout = *pPipelineLayout;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// RenderPass.
			{
				string renderPassPath = JsonParser::GetString(graphicInfo[_text_mapper(vk_renderpass_path)]);
				if (renderPassPath == _str_null)
				{
					_log_error("json file: [renderpass_path] can not be null!", LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}

				string renderPassJson = PathParser::Parse(renderPassPath);
				this->CreateRenderPass(renderPassJson);

				string renderPassName = JsonParser::GetString(graphicInfo[_text_mapper(vk_renderpass)]);

				graphicInfos[i].renderPass = this->GetRenderPass(renderPassName);

				// Subpass ID.
				string name = JsonParser::GetString(graphicInfo[_text_mapper(vk_subpass)]);

				auto& subpassNameIDMap = m_renderPassNameMapsubpassNameIDMap[renderPassName];

				auto found = subpassNameIDMap.find(name);
				if (found != subpassNameIDMap.end())
					graphicInfos[i].subpass = (*found).second;
				else
				{
					_log_error(StringUtil::Printf("Specified subpass name \"%\" was not found!", name), LogSystem::Category::JsonParser);
					Engine::Get()->RequireExit(1);
				}
			}
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// Pipeline Derivative.
			{
				string name = JsonParser::GetString(graphicInfo[_text_mapper(vk_base_pipeline)]);

				auto found = basePipelineNameIDMap.find(name);
				if (found != basePipelineNameIDMap.end())
				{
					graphicInfos[i].basePipelineHandle = VK_NULL_HANDLE;
					graphicInfos[i].basePipelineIndex = (*found).second;
				}
				else
				{
					_log_warning(StringUtil::Printf("Specified base_pipeline name \"%\" was not found in previous created pipeline, current pipeline will try to find a such named pipeline as the base pipeline in memory!", name), LogSystem::Category::JsonParser);

					graphicInfos[i].basePipelineHandle = this->GetPipeline(name);
					graphicInfos[i].basePipelineIndex = -1;
				}
			}
		}

		VkPipeline* pPipelines = new VkPipeline[numGInfo];
		this->CreateGraphicPipelines(pPipelines, graphicInfos.data(), (uint32)graphicInfos.size(), InPipCache);

		for (auto& pipeline : basePipelineNameIDMap)
		{
			_declare_vk_smart_ptr(VkPipeline, pPipeline);
			pPipeline = pPipelines + pipeline.second;
			m_pipelineNamePtrMap.emplace(pipeline.first, pPipeline);
		}

		_log_common("End creating graphic pipeline with " + InJsonPath, LogSystem::Category::LogicalDevice);
	}

	// test...
	// Engine::Get()->RequireExit(1);

	//return true;
}

void LogicalDevice::FlushAllQueue()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}

void LogicalDevice::ResetCmdPool()
{
	_vk_try(vkResetCommandPool(m_device, *m_pCmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
}
