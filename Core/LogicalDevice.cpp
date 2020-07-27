//
// LogicalDevice.cpp
//

#include "LogicalDevice.h"
#include "CommandQueue.h"
#include "BaseAllocator.h"

LogicalDevice::LogicalDevice(const VkDevice& InDevice)
	: m_device(InDevice)
{
	
}

LogicalDevice::LogicalDevice(const void* Null)
{
	m_device = VK_NULL_HANDLE;
	m_allocator = nullptr;
}

LogicalDevice& LogicalDevice::operator=(const VkDevice& InDevice)
{
	m_device = InDevice;
	return *this;
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

void LogicalDevice::SetVkDevice(const VkDevice& InDevice)
{
	m_device = InDevice;
}

void LogicalDevice::SetAllocator(BaseAllocator* InAllocator)
{
	m_allocator = InAllocator;
}

bool LogicalDevice::IsNoneAllocator() const
{
	return m_allocator == nullptr;
}

VkCommandPool LogicalDevice::GetCmdPool()
{
	return *m_pCmdPool;
}

CommandQueue LogicalDevice::GetQueue(uint32 InQueueFamilyIndex, uint32 InQueueIndex /*= 0*/)
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


void LogicalDevice::CreateCommandPool(const VkCommandPoolCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateCommandPool(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), m_pCmdPool.MakeInstance()));
}

void LogicalDevice::CreateCommandPool(uint32 InQueueFamilyIndex, VkCommandPoolCreateFlags InFlags /*= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT*/)
{
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.flags = InFlags;
	cmdPoolCreateInfo.queueFamilyIndex = InQueueFamilyIndex;

	_vk_try(vkCreateCommandPool(m_device, &cmdPoolCreateInfo, m_allocator->GetVkAllocator(), m_pCmdPool.MakeInstance()));
}

void LogicalDevice::CreateSwapchainKHR(VkSwapchainKHR* OutSwapchain, const VkSwapchainCreateInfoKHR& InCreateInfo)
{
	_vk_try(vkCreateSwapchainKHR(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutSwapchain));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const VkShaderModuleCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateShaderModule(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const uint32* InCodes, size_t InCodeSize)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = InCodeSize;
	shaderModuleCreateInfo.pCode = InCodes;

	_vk_try(vkCreateShaderModule(m_device, &shaderModuleCreateInfo, m_allocator->GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateComputePipeline(VkPipeline* OutPipeline, const VkComputePipelineCreateInfo& InCreateInfo, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	_vk_try(vkCreateComputePipelines(m_device, InPipCache, _count_1, &InCreateInfo, m_allocator->GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const VkComputePipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	_vk_try(vkCreateComputePipelines(m_device, InPipCache, InCreateInfoCount, InCreateInfos, m_allocator->GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipeline(VkPipeline* OutPipeline, VkPipelineLayout InPipLayout, VkShaderModule InShaderModule, const char* InShaderEntryName /*= "main"*/, const VkSpecializationInfo* InSpecialConstInfo /*= nullptr*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	VkPipelineShaderStageCreateInfo pipSSCreateInfo = {};
	pipSSCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipSSCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	pipSSCreateInfo.module = InShaderModule;
	pipSSCreateInfo.pName = InShaderEntryName;
	pipSSCreateInfo.pSpecializationInfo = InSpecialConstInfo;

	VkComputePipelineCreateInfo pipCSCreateInfo = {};
	pipCSCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipCSCreateInfo.stage = pipSSCreateInfo;
	pipCSCreateInfo.layout = InPipLayout;
	pipCSCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipCSCreateInfo.basePipelineIndex = -1;

	_vk_try(vkCreateComputePipelines(m_device, InPipCache, _count_1, &pipCSCreateInfo, m_allocator->GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipeline(VkPipeline* OutPipeline, const SPipCSCreateDesc& InCreateDesc, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	VkPipelineShaderStageCreateInfo pipSSCreateInfo = {};
	pipSSCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipSSCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	pipSSCreateInfo.module = InCreateDesc.ShaderModule;
	pipSSCreateInfo.pName = InCreateDesc.EntryPoint.c_str();
	pipSSCreateInfo.pSpecializationInfo = InCreateDesc.pSpecialConstInfo;

	VkComputePipelineCreateInfo pipCSCreateInfo = {};
	pipCSCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipCSCreateInfo.stage = pipSSCreateInfo;
	pipCSCreateInfo.layout = InCreateDesc.PipLayout;
	pipCSCreateInfo.basePipelineHandle = InCreateDesc.BasePipelineHandle;
	pipCSCreateInfo.basePipelineIndex = InCreateDesc.BasePipelineIndex;

	_vk_try(vkCreateComputePipelines(m_device, InPipCache, _count_1, &pipCSCreateInfo, m_allocator->GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const SPipCSCreateDesc* InCreateDescs, uint32 InCreateDescCount, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	VkPipelineShaderStageCreateInfo pipSSCreateInfo = {};
	pipSSCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipSSCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;

	VkComputePipelineCreateInfo* pPipCSCreateInfos = new VkComputePipelineCreateInfo[InCreateDescCount];
	for (uint32 i = 0; i < InCreateDescCount; ++i)
	{
		pipSSCreateInfo.module = InCreateDescs[i].ShaderModule;
		pipSSCreateInfo.pName = InCreateDescs[i].EntryPoint.c_str();
		pipSSCreateInfo.pSpecializationInfo = InCreateDescs[i].pSpecialConstInfo;

		pPipCSCreateInfos[i].sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pPipCSCreateInfos[i].stage = pipSSCreateInfo;
		pPipCSCreateInfos[i].layout = InCreateDescs[i].PipLayout;
		pPipCSCreateInfos[i].basePipelineHandle = InCreateDescs[i].BasePipelineHandle;
		pPipCSCreateInfos[i].basePipelineIndex = InCreateDescs[i].BasePipelineIndex;
	}

	_vk_try(vkCreateComputePipelines(m_device, InPipCache, InCreateDescCount, pPipCSCreateInfos, m_allocator->GetVkAllocator(), OutPipeline));
	delete[] pPipCSCreateInfos;
}

void LogicalDevice::CreatePipelineCache(VkPipelineCache* OutPipCache, const VkPipelineCacheCreateInfo& InCreateInfo)
{
	_vk_try(vkCreatePipelineCache(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutPipCache));
}

void LogicalDevice::CreatePipelineCache(VkPipelineCache* OutPipCache, const VkPhysicalDeviceProperties& InPDProp)
{
	SPipCacheHeader pipCacheHeader = SPipCacheHeader(InPDProp);

	VkPipelineCacheCreateInfo pipCacheCreateInfo = {};
	pipCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipCacheCreateInfo.initialDataSize = pipCacheHeader.GetDataSize();
	pipCacheCreateInfo.pInitialData = pipCacheHeader.GetData();

	_vk_try(vkCreatePipelineCache(m_device, &pipCacheCreateInfo, m_allocator->GetVkAllocator(), OutPipCache));
}

size_t LogicalDevice::GetPipelineCacheDataSize(VkPipelineCache InPipCache)
{
	size_t dataSize = 0;
	_vk_try(vkGetPipelineCacheData(m_device, InPipCache, &dataSize, nullptr));
	return dataSize;
}

void LogicalDevice::GetPipelineCacheData(VkPipelineCache InPipCache, size_t InDataSize, void* OutData)
{
	_vk_try(vkGetPipelineCacheData(m_device, InPipCache, &InDataSize, OutData));
}

bool LogicalDevice::SavePipelineCacheToFile(VkPipelineCache InPipCache, const char* InPath)
{
#if 0
	FILE* pOutputFile;

	size_t dataSize = GetPipelineCacheDataSize(InPipCache);
	void* pData = malloc(dataSize);

	if (pData != nullptr)
	{
		GetPipelineCacheData(InPipCache, dataSize, pData);

		pOutputFile = fopen_s(InPath, "wb");

		if (pOutputFile != nullptr)
		{
			fwrite(pData, 1, dataSize, pOutputFile);

			fclose(pOutputFile);

			free(pData);
			return true;
		}
		else
		{
			free(pData);
			return false;
		}		
	}

	return false;
#endif

#if 1
	std::ofstream ofs;

	size_t dataSize = GetPipelineCacheDataSize(InPipCache);
	byte* pData = new byte[dataSize];

	if (pData != nullptr)
	{
		GetPipelineCacheData(InPipCache, dataSize, pData);

		try
		{
			ofs.open(InPath, std::ofstream::out | std::ofstream::binary);

			ofs.write((char*)pData, dataSize);

			ofs.close();

			delete[] pData;
			return true;
		}
		catch (const std::exception& e)
		{
			_cmd_print_line(e.what());

			delete[] pData;
			return false;
		}
	}

	return false;
#endif
}

void LogicalDevice::MergePipelineCaches(VkPipelineCache OutMergedPipCache, const VkPipelineCache* InPipCaches, uint32 InSrcPipCacheCount)
{
	_vk_try(vkMergePipelineCaches(m_device, OutMergedPipCache, InSrcPipCacheCount, InPipCaches));
}

void LogicalDevice::CreateDescriptorSetLayout(VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateDescriptorSetLayout(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateDescriptorSetLayout(VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutBinding* InBindings, uint32 InBindingCount)
{
	VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {};
	descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.bindingCount = InBindingCount;
	descSetLayoutCreateInfo.pBindings = InBindings;

	_vk_try(vkCreateDescriptorSetLayout(m_device, &descSetLayoutCreateInfo, m_allocator->GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateSingleDescriptorLayout(VkDescriptorSetLayout* OutLayout, VkDescriptorType InDescType, VkShaderStageFlags InShaderStage, const VkSampler* InImmutableSamplers /*= nullptr*/)
{
	VkDescriptorSetLayoutBinding descSetLayoutBinding = {};
	descSetLayoutBinding.binding = _index_0;
	descSetLayoutBinding.descriptorType = InDescType;
	descSetLayoutBinding.descriptorCount = _count_1;
	descSetLayoutBinding.stageFlags = InShaderStage;
	descSetLayoutBinding.pImmutableSamplers = InImmutableSamplers;

	VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {};
	descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.bindingCount = _count_1;
	descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

	_vk_try(vkCreateDescriptorSetLayout(m_device, &descSetLayoutCreateInfo, m_allocator->GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreatePipelineLayout(VkPipelineLayout* OutLayout, const VkPipelineLayoutCreateInfo& InCreateInfo)
{
	_vk_try(vkCreatePipelineLayout(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreatePipelineLayout(VkPipelineLayout* OutLayout, const VkDescriptorSetLayout* InDescSetLayouts, uint32 InSetCount /*= _count_1*/, const VkPushConstantRange* InPushConstants /*= nullptr*/, uint32 InConstCount /*= _count_0*/)
{
	VkPipelineLayoutCreateInfo pipLayoutCreateInfo = {};
	pipLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipLayoutCreateInfo.setLayoutCount = InSetCount;
	pipLayoutCreateInfo.pSetLayouts = InDescSetLayouts;
	pipLayoutCreateInfo.pushConstantRangeCount = ((InPushConstants != nullptr) && (InConstCount == 0)) ? _count_1 : InConstCount;
	pipLayoutCreateInfo.pPushConstantRanges = InPushConstants;

	_vk_try(vkCreatePipelineLayout(m_device, &pipLayoutCreateInfo, m_allocator->GetVkAllocator(), OutLayout));
}

void LogicalDevice::CreateDescriptorPool(const VkDescriptorPoolCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateDescriptorPool(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), m_pDescPool.MakeInstance()));
}

void LogicalDevice::CreateDescriptorPool(uint32 InMaxSets, const VkDescriptorPoolSize* InPerDescTypeCounts, uint32 InDescTypeCount)
{
	VkDescriptorPoolCreateInfo descPoolCreateInfo = {};
	descPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolCreateInfo.maxSets = InMaxSets;
	descPoolCreateInfo.poolSizeCount = InDescTypeCount;
	descPoolCreateInfo.pPoolSizes = InPerDescTypeCounts;

	_vk_try(vkCreateDescriptorPool(m_device, &descPoolCreateInfo, m_allocator->GetVkAllocator(), m_pDescPool.MakeInstance()));
}

void LogicalDevice::FlushAllQueue()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}

void LogicalDevice::ResetCmdPool()
{
	_vk_try(vkResetCommandPool(m_device, *m_pCmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
}
