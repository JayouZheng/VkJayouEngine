//
// LogicalDevice.cpp
//

#include "BaseLayer.h"

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

void LogicalDevice::SetBaseLayer(BaseLayer* InBaseLayer)
{
	m_baseLayer = InBaseLayer;
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

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const VkComputePipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
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

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const SPipCSCreateDesc* InCreateDescs, uint32 InCreateDescCount/*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
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

void LogicalDevice::AllocatorDescriptorSets(VkDescriptorSet* OutDescSet, const VkDescriptorSetAllocateInfo& InAllocateInfo)
{
	_vk_try(vkAllocateDescriptorSets(m_device, &InAllocateInfo, OutDescSet));
}

void LogicalDevice::AllocatorDescriptorSets(VkDescriptorSet* OutDescSet, const VkDescriptorSetLayout* InSetLayouts, uint32 InSetCount /*= _count_1*/)
{
	VkDescriptorSetAllocateInfo descSetAllocateInfo = {};
	descSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descSetAllocateInfo.descriptorPool = *m_pDescPool;
	descSetAllocateInfo.descriptorSetCount = InSetCount;
	descSetAllocateInfo.pSetLayouts = InSetLayouts;

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
		_return_log("Funs: " + _str_name_of(UpdateImageOfDescSet) + " expect image descriptor type!");

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet = InDescSet;
	writeDescSet.dstBinding = InBindingIndex;
	writeDescSet.dstArrayElement = InSetOffset;
	writeDescSet.descriptorCount = InImageCount;
	writeDescSet.descriptorType = InImageDescType;
	writeDescSet.pImageInfo = InImageInfos;
	writeDescSet.pBufferInfo = nullptr;
	writeDescSet.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);
}

void LogicalDevice::UpdateBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InBufferDescType, const VkDescriptorBufferInfo* InBufferInfos, uint32 InBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC))
		_return_log("Funs: " + _str_name_of(UpdateBufferOfDescSet) + " expect buffer descriptor type!");

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet = InDescSet;
	writeDescSet.dstBinding = InBindingIndex;
	writeDescSet.dstArrayElement = InSetOffset;
	writeDescSet.descriptorCount = InBufferCount;
	writeDescSet.descriptorType = InBufferDescType;
	writeDescSet.pImageInfo = nullptr;
	writeDescSet.pBufferInfo = InBufferInfos;
	writeDescSet.pTexelBufferView = nullptr;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);
}

void LogicalDevice::UpdateTexelBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InTBufferDescType, const VkBufferView* InTBufferViews, uint32 InTBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InTBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER) &&
		(InTBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER))
		_return_log("Funs: " + _str_name_of(UpdateTexelBufferOfDescSet) + " expect texel buffer descriptor type!");

	VkWriteDescriptorSet writeDescSet = {};
	writeDescSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSet.dstSet = InDescSet;
	writeDescSet.dstBinding = InBindingIndex;
	writeDescSet.dstArrayElement = InSetOffset;
	writeDescSet.descriptorCount = InTBufferCount;
	writeDescSet.descriptorType = InTBufferDescType;
	writeDescSet.pImageInfo = nullptr;
	writeDescSet.pBufferInfo = nullptr;
	writeDescSet.pTexelBufferView = InTBufferViews;

	vkUpdateDescriptorSets(m_device, _count_1, &writeDescSet, _count_0, nullptr);
}

void LogicalDevice::CopyDescriptorSets(const VkCopyDescriptorSet* InDescCopies, uint32 InCopySetCount /*= _count_1*/)
{
	vkUpdateDescriptorSets(m_device, _count_0, nullptr, InCopySetCount, InDescCopies);
}

void LogicalDevice::CopyDescriptorSet(VkDescriptorSet InSrcSet, uint32 InSrcBindingIndex, VkDescriptorSet InDstSet, uint32 InDstBindingIndex, uint32 InCopyDescCount, uint32 InSrcSetOffset /*= _offset_0*/, uint32 InDstSetOffset /*= _offset_0*/)
{
	VkCopyDescriptorSet copyDescSet = {};
	copyDescSet.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
	copyDescSet.srcSet = InSrcSet;
	copyDescSet.srcBinding = InSrcBindingIndex;
	copyDescSet.srcArrayElement = InSrcSetOffset;
	copyDescSet.dstSet = InDstSet;
	copyDescSet.dstBinding = InDstBindingIndex;
	copyDescSet.dstArrayElement = InDstSetOffset;
	copyDescSet.descriptorCount = InCopyDescCount;

	vkUpdateDescriptorSets(m_device, _count_0, nullptr, _count_1, &copyDescSet);
}

void LogicalDevice::CreateSampler(VkSampler* OutSampler, const VkSamplerCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateSampler(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreatePointWrapSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreatePointClampSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateLinearWrapSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateLinearClampSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateAnisotropicWrapSampler(VkSampler* OutSampler)
{
	if (m_baseLayer == nullptr)
		_return_log("Funs: " + _str_name_of(CreateAnisotropicWrapSampler) + " expect to Query Physical Device Limits!");

	float maxAnisotropy = std::min(GConfig::Sampler::MaxAnisotropy, m_baseLayer->GetMainPDLimits().maxSamplerAnisotropy);

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;                            // Anisotropic Filtering.
	samplerCreateInfo.maxAnisotropy = maxAnisotropy;                         // Anisotropic Filtering.
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateAnisotropicClampSampler(VkSampler* OutSampler)
{
	if (m_baseLayer == nullptr)
		_return_log("Funs: " + _str_name_of(CreateAnisotropicClampSampler) + " expect to Query Physical Device Limits!");

	float maxAnisotropy = std::min(GConfig::Sampler::MaxAnisotropy, m_baseLayer->GetMainPDLimits().maxSamplerAnisotropy);

	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;                            // Anisotropic Filtering.
	samplerCreateInfo.maxAnisotropy = maxAnisotropy;                         // Anisotropic Filtering.
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;                       // It does not matter.
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreatePCFSampler(VkSampler* OutSampler)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
	samplerCreateInfo.mipLodBias = 0;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;                                  // 16.0f ?
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, m_allocator->GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateRenderPass(VkRenderPass* OutRenderPass, const VkRenderPassCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateRenderPass(m_device, &InCreateInfo, m_allocator->GetVkAllocator(), OutRenderPass));
}

void LogicalDevice::CreateSingleRenderPass(VkRenderPass* OutRenderPass, VkFormat InColorFormat, VkFormat InDepthFormat)
{
	std::array<VkAttachmentDescription, 2> attachments{};
	// Color attachment.
	attachments[0].format = InColorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Depth attachment.
	attachments[1].format = InDepthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Subpasses.
	std::array<VkSubpassDescription, 1> subpassDescriptions{};

	// First subpass...
	// ----------------------------------------------------------------------------------------

	// std::array<VkAttachmentReference, 1> inputReferences{};
	std::array<VkAttachmentReference, 1> colorReferences{};
	// std::array<VkAttachmentReference, 1> resolveReferences{};
	std::array<VkAttachmentReference, 1> depthStencilReferences{};
	// std::array<VkAttachmentReference, 1> preserveAttachmentIndices{};

	colorReferences[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	depthStencilReferences[0] = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	subpassDescriptions[0].flags = _flag_none;
	subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescriptions[0].inputAttachmentCount = _count_0;
	subpassDescriptions[0].pInputAttachments = nullptr;
	subpassDescriptions[0].colorAttachmentCount = _count_1;
	subpassDescriptions[0].pColorAttachments = colorReferences.data();
	subpassDescriptions[0].pResolveAttachments = nullptr;
	subpassDescriptions[0].pDepthStencilAttachment = depthStencilReferences.data();
	subpassDescriptions[0].preserveAttachmentCount = _count_0;
	subpassDescriptions[0].pPreserveAttachments = nullptr;

	// Subpass dependencies for layout transitions
	std::array<VkSubpassDependency, 1> dependencies;

	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].dstSubpass = _index_0;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;      // FrameBuffer Local.

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
	renderPassInfo.pSubpasses = subpassDescriptions.data();
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();

	_vk_try(vkCreateRenderPass(m_device, &renderPassInfo, m_allocator->GetVkAllocator(), OutRenderPass));
}

void LogicalDevice::CreateFrameBuffer(VkFramebuffer* OutFrameBuffer, const VkFramebufferCreateInfo& InCreateInfo)
{
	if (m_baseLayer == nullptr)
		_return_log("Funs: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!");

	VkFramebufferCreateInfo frameBufferCreateInfo = InCreateInfo;

	_is_guaranteed_min(frameBufferCreateInfo.width,  4096, m_baseLayer->GetMainPDLimits().maxFramebufferWidth );
	_is_guaranteed_min(frameBufferCreateInfo.height, 4096, m_baseLayer->GetMainPDLimits().maxFramebufferHeight);
	_is_guaranteed_min(frameBufferCreateInfo.layers, 256,  m_baseLayer->GetMainPDLimits().maxFramebufferLayers);

	_vk_try(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, m_allocator->GetVkAllocator(), OutFrameBuffer));
}

void LogicalDevice::CreateFrameBuffer(VkFramebuffer* OutFrameBuffer, VkRenderPass InRenderPass, const VkImageView* InImageViews, uint32 InViewCount, VkExtent3D InSize)
{
	if (m_baseLayer == nullptr)
		_return_log("Funs: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!");

	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.renderPass = InRenderPass;
	frameBufferCreateInfo.attachmentCount = InViewCount;
	frameBufferCreateInfo.pAttachments = InImageViews;
	frameBufferCreateInfo.width = InSize.width;
	frameBufferCreateInfo.height = InSize.height;
	frameBufferCreateInfo.layers = InSize.depth;

	_is_guaranteed_min(frameBufferCreateInfo.width,  4096, m_baseLayer->GetMainPDLimits().maxFramebufferWidth );
	_is_guaranteed_min(frameBufferCreateInfo.height, 4096, m_baseLayer->GetMainPDLimits().maxFramebufferHeight);
	_is_guaranteed_min(frameBufferCreateInfo.layers, 256,  m_baseLayer->GetMainPDLimits().maxFramebufferLayers);

	_vk_try(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, m_allocator->GetVkAllocator(), OutFrameBuffer));
}

void LogicalDevice::FlushAllQueue()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}

void LogicalDevice::ResetCmdPool()
{
	_vk_try(vkResetCommandPool(m_device, *m_pCmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
}
