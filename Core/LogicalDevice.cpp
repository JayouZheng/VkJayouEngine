//
// LogicalDevice.cpp
//

#include "BaseLayer.h"
#include "JsonParser.h"
#include "GLSLCompiler.h"

#include "StringManager.h"

VkAllocationCallbacks* LogicalDevice::GetVkAllocator() const
{
	return m_allocator != nullptr ? m_allocator->GetVkAllocator() : nullptr;
}

LogicalDevice::LogicalDevice(const VkDevice& InDevice)
	: m_device(InDevice)
{

}

LogicalDevice::LogicalDevice(const void* Null)
{
	m_device    = VK_NULL_HANDLE;
	m_baseLayer = nullptr;
	m_allocator = nullptr;
	m_window    = nullptr;
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

void LogicalDevice::SetWindow(Window* InWindow)
{
	m_window = InWindow;
}

bool LogicalDevice::IsNoneAllocator() const
{
	return m_allocator == nullptr;
}

VkCommandPool LogicalDevice::GetCmdPool()
{
	return *m_pCmdPool;
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
	_vk_try(vkCreateCommandPool(m_device, &InCreateInfo, GetVkAllocator(), m_pCmdPool.MakeInstance()));
}

void LogicalDevice::CreateCommandPool(uint32 InQueueFamilyIndex, VkCommandPoolCreateFlags InFlags /*= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT*/)
{
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.flags = InFlags;
	cmdPoolCreateInfo.queueFamilyIndex = InQueueFamilyIndex;

	_vk_try(vkCreateCommandPool(m_device, &cmdPoolCreateInfo, GetVkAllocator(), m_pCmdPool.MakeInstance()));
}

void LogicalDevice::CreateSwapchainKHR(VkSwapchainKHR* OutSwapchain, const VkSwapchainCreateInfoKHR& InCreateInfo)
{
	_vk_try(vkCreateSwapchainKHR(m_device, &InCreateInfo, GetVkAllocator(), OutSwapchain));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const VkShaderModuleCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateShaderModule(m_device, &InCreateInfo, GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const uint32* InCodes, size_t InCodeSize)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = InCodeSize;
	shaderModuleCreateInfo.pCode = InCodes;

	_vk_try(vkCreateShaderModule(m_device, &shaderModuleCreateInfo, GetVkAllocator(), OutShaderModule));
}

void LogicalDevice::CreateShaderModule(VkShaderModule* OutShaderModule, const char* InShaderPath)
{
	std::ifstream is(InShaderPath, std::ios::binary | std::ios::in | std::ios::ate);

	if (is.is_open())
	{
		size_t size = is.tellg();
		_breturn_log(size == -1, _str_name_of(CreateShaderModule) + ", file size go to -1(at std::istream::tellg)!");

		is.seekg(0, std::ios::beg);
		char* shaderCode = new char[size];
		is.read(shaderCode, size);
		is.close();

		GLSLCompiler compiler;
		compiler.CompileShader(Util::GetShaderStage("vertex"), InShaderPath);
		GLSLCompiler::SPVData spvData = compiler.GetDuplicatedSPVData();

		std::string name, ext, dir;
		StringUtil::ExtractFilePath(std::string(InShaderPath), &name, &ext, &dir);

		this->CreateShaderModule(OutShaderModule, (uint32*)shaderCode, size);

		delete[] shaderCode;
	}
	else
	{
		*OutShaderModule = VK_NULL_HANDLE;
		_return_log("Error: Could not open shader file \"" + std::string(InShaderPath) + "\"");
	}
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

void LogicalDevice::CreateComputePipelines(VkPipeline* OutPipeline, const SPipelineComputeDesc* InDescs, uint32 InDescCount/*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
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

void LogicalDevice::CreatePipelineCache(VkPipelineCache* OutPipCache, const VkPhysicalDeviceProperties& InPDProp)
{
	SPipelineCacheHeader pipCacheHeader = SPipelineCacheHeader(InPDProp);

	VkPipelineCacheCreateInfo pipCacheCreateInfo = {};
	pipCacheCreateInfo.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipCacheCreateInfo.initialDataSize = pipCacheHeader.GetDataSize();
	pipCacheCreateInfo.pInitialData    = pipCacheHeader.GetData();

	_vk_try(vkCreatePipelineCache(m_device, &pipCacheCreateInfo, GetVkAllocator(), OutPipCache));
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
	pipLayoutCreateInfo.pPushConstantRanges    = InPushConstants;

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
		_return_log("Func: " + _str_name_of(UpdateImageOfDescSet) + " expect image descriptor type!");

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
}

void LogicalDevice::UpdateBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InBufferDescType, const VkDescriptorBufferInfo* InBufferInfos, uint32 InBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) &&
		(InBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC))
		_return_log("Func: " + _str_name_of(UpdateBufferOfDescSet) + " expect buffer descriptor type!");

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
}

void LogicalDevice::UpdateTexelBufferOfDescSet(VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InTBufferDescType, const VkBufferView* InTBufferViews, uint32 InTBufferCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/)
{
	if ((InTBufferDescType != VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER) &&
		(InTBufferDescType != VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER))
		_return_log("Func: " + _str_name_of(UpdateTexelBufferOfDescSet) + " expect texel buffer descriptor type!");

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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateAnisotropicWrapSampler(VkSampler* OutSampler)
{
	if (m_baseLayer == nullptr)
	{
		OutSampler = VK_NULL_HANDLE;
		_return_log("Func: " + _str_name_of(CreateAnisotropicWrapSampler) + " expect to Query Physical Device Limits!");
	}

	float maxAnisotropy = std::min(GConfig::Sampler::MaxAnisotropy, m_baseLayer->GetMainPDLimits().maxSamplerAnisotropy);

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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateAnisotropicClampSampler(VkSampler* OutSampler)
{
	if (m_baseLayer == nullptr)
	{
		OutSampler = VK_NULL_HANDLE;
		_return_log("Func: " + _str_name_of(CreateAnisotropicClampSampler) + " expect to Query Physical Device Limits!");
	}	

	float maxAnisotropy = std::min(GConfig::Sampler::MaxAnisotropy, m_baseLayer->GetMainPDLimits().maxSamplerAnisotropy);

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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK; // It does not matter.
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
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
	samplerCreateInfo.maxLod                  = GConfig::Sampler::SamplerMaxLod;
	samplerCreateInfo.borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	_vk_try(vkCreateSampler(m_device, &samplerCreateInfo, GetVkAllocator(), OutSampler));
}

void LogicalDevice::CreateRenderPass(VkRenderPass* OutRenderPass, const VkRenderPassCreateInfo& InCreateInfo)
{
	_vk_try(vkCreateRenderPass(m_device, &InCreateInfo, GetVkAllocator(), OutRenderPass));
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
	if (m_baseLayer == nullptr)
	{
		OutFrameBuffer = VK_NULL_HANDLE;
		_return_log("Func: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!");
	}
		
	VkFramebufferCreateInfo frameBufferCreateInfo = InCreateInfo;

	_is_guaranteed_min(frameBufferCreateInfo.width,  4096, m_baseLayer->GetMainPDLimits().maxFramebufferWidth );
	_is_guaranteed_min(frameBufferCreateInfo.height, 4096, m_baseLayer->GetMainPDLimits().maxFramebufferHeight);
	_is_guaranteed_min(frameBufferCreateInfo.layers, 256,  m_baseLayer->GetMainPDLimits().maxFramebufferLayers);

	_vk_try(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, GetVkAllocator(), OutFrameBuffer));
}

void LogicalDevice::CreateFrameBuffer(VkFramebuffer* OutFrameBuffer, VkRenderPass InRenderPass, const VkImageView* InImageViews, uint32 InViewCount, VkExtent3D InSize)
{
	if (m_baseLayer == nullptr)
	{
		OutFrameBuffer = VK_NULL_HANDLE;
		_return_log("Func: " + _str_name_of(CreateFrameBuffer) + " expect to Query Physical Device Limits!");
	}
		
	VkFramebufferCreateInfo frameBufferCreateInfo = {};
	frameBufferCreateInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.renderPass      = InRenderPass;
	frameBufferCreateInfo.attachmentCount = InViewCount;
	frameBufferCreateInfo.pAttachments    = InImageViews;
	frameBufferCreateInfo.width           = InSize.width;
	frameBufferCreateInfo.height          = InSize.height;
	frameBufferCreateInfo.layers          = InSize.depth;

	_is_guaranteed_min(frameBufferCreateInfo.width,  4096, m_baseLayer->GetMainPDLimits().maxFramebufferWidth );
	_is_guaranteed_min(frameBufferCreateInfo.height, 4096, m_baseLayer->GetMainPDLimits().maxFramebufferHeight);
	_is_guaranteed_min(frameBufferCreateInfo.layers, 256,  m_baseLayer->GetMainPDLimits().maxFramebufferLayers);

	_vk_try(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, GetVkAllocator(), OutFrameBuffer));
}

void LogicalDevice::CreateGraphicPipelines(VkPipeline* OutPipeline, const VkGraphicsPipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	_vk_try(vkCreateGraphicsPipelines(m_device, InPipCache, InCreateInfoCount, InCreateInfos, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateGraphicPipelines(VkPipeline* OutPipeline, const SPipelineGraphicDesc* InDescs, uint32 InDescCount /*= _count_1*/, VkPipelineCache InPipCache /*= VK_NULL_HANDLE*/)
{
	const SPipelineGraphicDesc& InDesc = InDescs[0];

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		VK_SHADER_STAGE_VERTEX_BIT,// stage
		InDesc.ShaderModule,// module
		InDesc.EntryPoint.c_str(),// pName
		InDesc.pSpecialConstInfo// pSpecializationInfo
	};

	static const VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		0,//vertexBindingDescriptionCount
		nullptr,// pVertexBindingDescriptions
		0,//vertexAttributeDescriptionCount
		nullptr// pVertexAttributeDescriptions
	};

	static const VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		VK_PRIMITIVE_TOPOLOGY_POINT_LIST,// topology
		VK_FALSE// primitiveRestartEnable
	};

	static const VkViewport dummyViewport = 
	{ 
		0.0f, 0.0f,// x, y
		1.0f, 1.0f,// width, height
		0.1f, 1000.0f// minDepth, maxDepth
	};

	static const VkRect2D dummyScissor = 
	{
		{ 0, 0 },// offset
		{ 1, 1 }// extent
	};

	static const VkPipelineViewportStateCreateInfo viewportStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		1,// viewportCount
		&dummyViewport,// pViewports
		1,// scissorCount
		&dummyScissor// pScissors
	};

	static const VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		VK_FALSE,// depthClampEnable
		VK_TRUE,// rasterizerDiscardEnable
		VK_POLYGON_MODE_FILL,// polygonMode
		VK_CULL_MODE_NONE,// cullMode
		VK_FRONT_FACE_COUNTER_CLOCKWISE,// frontFace
		VK_FALSE,// depthBiasEnable
		0.0f,// depthBiasConstantFactor
		0.0f,// depthBiasClamp
		0.0f,// depthBiasSlopeFactor
		0.0f// lineWidth
	};

	static const VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = 
	{ 
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,// sType
		nullptr,// pNext
		0,// flags
		1,// stageCount
		&shaderStageCreateInfo,// pStages
		&vertexInputStateCreateInfo,// pVertexInputState
		&inputAssemblyStateCreateInfo,// pInputAssemblyState
		nullptr,// pTessellationState
		&viewportStateCreateInfo,// pViewportState
		&rasterizationStateCreateInfo,// pRasterizationState
		nullptr,// pMultisampleState
		nullptr,// pDepthStencilState
		nullptr,// pColorBlendState
		nullptr,// pDynamicState
		InDesc.PipLayout,// layout
		InDesc.RenderPass,// renderPass
		0,// subpass
		InDesc.BasePipelineHandle,// basePipelineHandle
		InDesc.BasePipelineIndex// basePipelineIndex
	};

	_vk_try(vkCreateGraphicsPipelines(m_device, InPipCache, _count_1, &graphicsPipelineCreateInfo, GetVkAllocator(), OutPipeline));
}

void LogicalDevice::CreateGraphicPipelines(VkPipeline* OutPipeline, const std::string& InJsonPath, VkPipelineCache InPipCache)
{
	if (OutPipeline != nullptr) *OutPipeline = VK_NULL_HANDLE;
	Json::Value root;
	_breturn_log(!Util::ParseJson(InJsonPath, root), _str_name_of(CreateGraphicPipelines) + " Failed! Not Valid File Path!");

	_jverify_return_log(root["graphic_pipeline_infos"], "json file: [graphic_pipeline_infos] can not be null!");
	
	bool bIsArray   = root["graphic_pipeline_infos"].isArray();
	uint32 numGInfo = bIsArray ? root["graphic_pipeline_infos"].size() : _count_1;

	VkGraphicsPipelineCreateInfo*    pGraphicInfos = _safe_new(VkGraphicsPipelineCreateInfo, numGInfo);
	VkPipelineShaderStageCreateInfo* pShaderInfos  = nullptr;;
	VkSpecializationMapEntry*        pSpecMaps     = nullptr;
	uint32*                          pSpecData     = nullptr;
	VkSpecializationInfo             specInfo      = {};

	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo   = {};
	VkVertexInputBindingDescription*     pVertexInputBindings    = nullptr;
	VkVertexInputAttributeDescription*   pVertexInputAttributes = nullptr;

	VkPipelineInputAssemblyStateCreateInfo pipelineIAStateInfo       = GConfig::Pipeline::DefaultInputAssemblyStateInfo;
	VkPipelineTessellationStateCreateInfo  pipelineTessStateInfo     = GConfig::Pipeline::DefaultTessellationStateInfo;
	VkPipelineViewportStateCreateInfo      pipelineViewportStateInfo = GConfig::Pipeline::DefaultViewportStateInfo;

	VkViewport currentViewport = {};
	VkRect2D   currentScissor  = {};
	this->SetViewport(currentViewport, currentScissor, m_window->GetWindowDesc().Width, m_window->GetWindowDesc().Height);

	VkPipelineRasterizationStateCreateInfo pipelineRSStateInfo           = GConfig::Pipeline::DefaultRasterizationStateInfo;
	VkPipelineMultisampleStateCreateInfo   pipelineMultisampleStateInfo  = GConfig::Pipeline::DefaultMultisampleStateInfo;
	VkSampleMask*                          pSampleMasks                  = nullptr;
	VkPipelineDepthStencilStateCreateInfo  pipelineDepthStencilStateInfo = GConfig::Pipeline::DefaultDepthStencilStateInfo;
	VkPipelineColorBlendStateCreateInfo    pipelineColorBlendStateInfo   = GConfig::Pipeline::DefaultColorBlendStateInfo;
	VkPipelineColorBlendAttachmentState*   pColorBlendAttachmentStates   = nullptr;
	VkPipelineDynamicStateCreateInfo       pipelineDynamicStateInfo      = GConfig::Pipeline::DefaultDynamicStateInfo;
	VkDynamicState*                        pDynamicStates                = nullptr;
	VkPipelineLayoutCreateInfo             pipelineLayoutInfo            = {};

	for (uint32 i = 0; i < numGInfo; i++)
	{
		auto& graphicInfo = bIsArray ? root["graphic_pipeline_infos"][i] : root["graphic_pipeline_infos"];

		pGraphicInfos[i].sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pGraphicInfos[i].pNext = nullptr;
		pGraphicInfos[i].flags = _jget_uint(graphicInfo["pipeline_flags"]);

		// Pipeline Stage.
		_jverify_return_log(graphicInfo["pipeline_stages_infos"], "json file: [pipeline_stages_infos] can not be null!");
		
		bIsArray            = graphicInfo["pipeline_stages_infos"].isArray();
		uint32 numStageInfo = bIsArray ? graphicInfo["pipeline_stages_infos"].size() : _count_1;
		pShaderInfos        = _safe_new(VkPipelineShaderStageCreateInfo, numStageInfo);

		pGraphicInfos[i].stageCount = numStageInfo;
		pGraphicInfos[i].pStages    = pShaderInfos;

		for (uint32 j = 0; j < numStageInfo; j++)
		{
			auto& shaderInfo = bIsArray ? graphicInfo["pipeline_stages_infos"][j] : graphicInfo["pipeline_stages_infos"];

			VkSmartPtr<VkShaderModule> pShaderModule;
			this->CreateShaderModule(pShaderModule.MakeInstance(), _jget_cstring(shaderInfo["stage_code_path"]));

			pShaderInfos[j].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pShaderInfos[j].pNext  = nullptr;
			pShaderInfos[j].flags  = _jget_uint(shaderInfo["stage_flags"]);
			pShaderInfos[j].stage  = Util::GetShaderStage(_jget_string(shaderInfo["stage_type"]));
			pShaderInfos[j].module = *pShaderModule;
			pShaderInfos[j].pName  = _jget_cstring_default(shaderInfo["entrypoint"], "main");
			pShaderInfos[j].pSpecializationInfo = &specInfo;
			
			if (shaderInfo["specialization_constants"] != Json::nullValue)
			{			
				bIsArray            = shaderInfo["specialization_constants"].isArray();
				uint32 numSpecConst = bIsArray ? shaderInfo["specialization_constants"].size() : _count_1;

				pSpecMaps = _safe_new(VkSpecializationMapEntry, numSpecConst);
				pSpecData = _safe_new(uint32, numSpecConst);

				specInfo.mapEntryCount = numSpecConst;
				specInfo.pMapEntries   = pSpecMaps;
				specInfo.dataSize      = numSpecConst * 4; // 4 byte per const, 32 bit value.
				specInfo.pData         = pSpecData;

				for (uint32 k = 0; k < numSpecConst; k++)
				{
					auto& value = bIsArray ? shaderInfo["specialization_constants"][k] : shaderInfo["specialization_constants"];

					pSpecMaps[k].constantID = k;
					pSpecMaps[k].offset     = k * 4; // 4 byte per const, 32 bit value.
					pSpecMaps[k].size       = 4;     // 4 byte per const, 32 bit value.

					//////////////////////////////////////////////////////////////
					// json value reinterpretation.
					switch (value.type())
					{
					case Json::ValueType::intValue:     pSpecMaps[k].size = sizeof(int32);    _reinterpret_data(pSpecData[k], value.asInt());   break;
					case Json::ValueType::uintValue:    pSpecMaps[k].size = sizeof(uint32);   _reinterpret_data(pSpecData[k], value.asUInt());  break;
					case Json::ValueType::realValue:    pSpecMaps[k].size = sizeof(float);    _reinterpret_data(pSpecData[k], value.asFloat()); break;
					case Json::ValueType::booleanValue: pSpecMaps[k].size = sizeof(VkBool32); _reinterpret_data(pSpecData[k], value.asBool());  break;
					default: _return_log("json file: not support [specialization_constants] value type!");
					}
					//////////////////////////////////////////////////////////////
				}				
			}
			else
			{
				pShaderInfos[j].pSpecializationInfo = nullptr;
			}		
		}

		// Vertex Input State.
		pGraphicInfos[i].pVertexInputState = &vertexInputStateInfo;
		_jverify_return_log(graphicInfo["vertex_input_attributes"], "json file: [vertex_input_attributes] can not be null!");

		// Bindings.
		bIsArray             = graphicInfo["vertex_input_attributes"].isArray();
		uint32 numBinding    = bIsArray ? graphicInfo["vertex_input_attributes"].size() : _count_1;
		pVertexInputBindings = _safe_new(VkVertexInputBindingDescription, numBinding);

		for (uint32 j = 0; j < numBinding; j++)
		{
			auto& binding    = bIsArray ? graphicInfo["vertex_input_attributes"][j] : graphicInfo["vertex_input_attributes"];
			uint32 bindingID = binding["binding_id"] != Json::nullValue ? binding["binding_id"].asUInt() : j;

			_bbreak_log(j >= 16u, "current app vertex input binding number is limit to 16!");
			_bcontinue_log(bindingID >= 16u, "current app vertex input binding number is limit to 16!");

			// Vertex Attributes.
			bIsArray               = binding["attributes"].isArray();
			uint32 numAttribute    = bIsArray ? binding["attributes"].size() : _count_1;
			pVertexInputAttributes = _safe_new(VkVertexInputAttributeDescription, numAttribute);

			uint32  attributeOffset  = 0;
			uint32& allAttributeSize = attributeOffset;
			for (uint32 k = 0; k < numAttribute; k++)
			{
				std::string attribute = bIsArray ? binding["attributes"][k].asString() : binding["attributes"].asString();

				pVertexInputAttributes[k].binding  = _index_0;
				pVertexInputAttributes[k].location = k;
				pVertexInputAttributes[k].format   = Util::GetVertexAttributeVkFormat(attribute);
				pVertexInputAttributes[k].offset   = attributeOffset;

				attributeOffset += Util::GetVertexAttributeSize(attribute);
			}
				
			pVertexInputBindings[j].binding   = bindingID;
			pVertexInputBindings[j].stride    = allAttributeSize;
			pVertexInputBindings[j].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			vertexInputStateInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputStateInfo.vertexBindingDescriptionCount   = numBinding;
			vertexInputStateInfo.pVertexBindingDescriptions      = pVertexInputBindings;
			vertexInputStateInfo.vertexAttributeDescriptionCount = numAttribute;
			vertexInputStateInfo.pVertexAttributeDescriptions    = pVertexInputAttributes;
		}
		
		// IA State.
		pGraphicInfos[i].pInputAssemblyState = &pipelineIAStateInfo;
		auto& inputAssemblyInfo              = graphicInfo["pipeline_input_assembly"];
		if (inputAssemblyInfo != Json::nullValue)
		{
			pipelineIAStateInfo.flags                  = _jget_uint(inputAssemblyInfo["flags"]);
			pipelineIAStateInfo.topology               = Util::GetPrimitiveTopology(_jget_string_default(inputAssemblyInfo["primitive_topology"], Util::DefaultPrimitiveTopology));
			pipelineIAStateInfo.primitiveRestartEnable = _jget_uint(inputAssemblyInfo["primitive_restart_enable"]);
		}

		// Tessellation State.
		auto& tessellationInfo = graphicInfo["tessellation_state"];
		if (tessellationInfo != Json::nullValue)
		{
			pGraphicInfos[i].pTessellationState      = &pipelineTessStateInfo;
			pipelineTessStateInfo.sType              = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
			pipelineTessStateInfo.flags              = _jget_uint(tessellationInfo["flags"]);
			pipelineTessStateInfo.patchControlPoints = _jget_uint(tessellationInfo["patch_control_points_count"]);
		}
		else
		{
			pGraphicInfos[i].pTessellationState = nullptr;
		}

		// Viewport State.
		pGraphicInfos[i].pViewportState = &pipelineViewportStateInfo;
		auto& viewportInfo              = graphicInfo["viewport_state"];
		if (viewportInfo != Json::nullValue)
		{
			bIsArray           = viewportInfo["viewports"].isArray();
			uint32 numViewport = bIsArray ? viewportInfo["viewports"].size() : _count_1;

			pipelineViewportStateInfo.flags         = _jget_uint(viewportInfo["flags"]);
			pipelineViewportStateInfo.viewportCount = numViewport;
			pipelineViewportStateInfo.scissorCount  = numViewport;
			pipelineViewportStateInfo.pViewports    = &currentViewport;
			pipelineViewportStateInfo.pScissors     = &currentScissor;

			for (uint32 j = 0; j < numViewport; j++)
			{
				auto& viewport = bIsArray ? viewportInfo["viewports"][j] : viewportInfo["viewports"];
				auto& scissor  = viewportInfo["scissor_rectangles"].isArray() ? viewportInfo["scissor_rectangles"][j] : viewportInfo["scissor_rectangles"];

				_breturn_log(!viewport["position"].isArray(),    "json file: viewport [position] must be an array [ first, second ]!"   );
				_breturn_log(!viewport["size"].isArray(),        "json file: viewport [size] must be an array [ first, second ]!"       );
				_breturn_log(!viewport["depth_range"].isArray(), "json file: viewport [depth_range] must be an array [ first, second ]!");
				_breturn_log(!scissor ["offset"].isArray(),      "json file: scissor [offset] must be an array [ first, second ]!"      );
				_breturn_log(!scissor ["size"].isArray(),        "json file: scissor [size] must be an array [ first, second ]!"        );

				currentViewport.x        = _jget_float(viewport["position"][0]);
				currentViewport.y        = _jget_float(viewport["position"][1]);
				currentViewport.width    = _jis_auto  (viewport["size"][0]) ? (float)m_window->GetWindowDesc().Width  : _jget_float(viewport["size"][0]);
				currentViewport.height   = _jis_auto  (viewport["size"][1]) ? (float)m_window->GetWindowDesc().Height : _jget_float(viewport["size"][1]);
				currentViewport.minDepth = _jget_float(viewport["depth_range"][0]);
				currentViewport.maxDepth = _jget_float(viewport["depth_range"][1]);

				currentScissor.offset.x      = _jget_int(scissor["offset"][0]);
				currentScissor.offset.y      = _jget_int(scissor["offset"][1]);
				currentScissor.extent.width  = _jis_auto(scissor["size"][0]) ? m_window->GetWindowDesc().Width  : _jget_uint(scissor["size"][0]);
				currentScissor.extent.height = _jis_auto(scissor["size"][1]) ? m_window->GetWindowDesc().Height : _jget_uint(scissor["size"][1]);
			}
		}

		// RS State.
		pGraphicInfos[i].pRasterizationState = &pipelineRSStateInfo;
		auto& rasterizationInfo              = graphicInfo["rasterization_state"];
		if (rasterizationInfo != Json::nullValue)
		{
			pipelineRSStateInfo.flags                   = _jget_uint(rasterizationInfo["flags"]);
			pipelineRSStateInfo.depthClampEnable        = _jget_uint(rasterizationInfo["depth_clamp_enable"]);
			pipelineRSStateInfo.rasterizerDiscardEnable = _jget_uint(rasterizationInfo["rasterizer_discard_enable"]);
			pipelineRSStateInfo.polygonMode             = Util::GetPolygonMode(_jget_string_default(rasterizationInfo["polygon_mode"], Util::DefaultPolygonMode));
			pipelineRSStateInfo.cullMode                = Util::GetCullMode(_jget_string_default(rasterizationInfo["cull_mode"], Util::DefaultCullMode));
			pipelineRSStateInfo.frontFace               = Util::GetFrontFace(_jget_string_default(rasterizationInfo["front_face"], Util::DefaultFrontFace));
			pipelineRSStateInfo.depthBiasEnable         = _jget_uint(rasterizationInfo["depth_bias_enable"]);
			pipelineRSStateInfo.depthBiasConstantFactor = _jget_float(rasterizationInfo["depth_bias_constant_factor"]);
			pipelineRSStateInfo.depthBiasClamp          = _jget_float(rasterizationInfo["depth_bias_clamp"]);
			pipelineRSStateInfo.depthBiasSlopeFactor    = _jget_float(rasterizationInfo["depth_bias_slope_factor"]);
			pipelineRSStateInfo.lineWidth               = _jget_float(rasterizationInfo["line_width"]);
		}

		// Multisample State.
		pGraphicInfos[i].pMultisampleState = &pipelineMultisampleStateInfo;
		auto& multisampleInfo              = graphicInfo["multisample_state"];
		if (multisampleInfo != Json::nullValue)
		{
			bIsArray             = multisampleInfo["sample_masks"].isArray();
			uint32 numSampleMask = bIsArray ? multisampleInfo["sample_masks"].size() : _count_1;
			pSampleMasks         = _safe_new(VkSampleMask, numSampleMask);
			for (uint32 j = 0; j < numSampleMask; j++)
				pSampleMasks[j] = bIsArray ? multisampleInfo["sample_masks"][j].asUInt() : multisampleInfo["sample_masks"].asUInt();

			pipelineMultisampleStateInfo.flags                 = _jget_uint(multisampleInfo["flags"]);
			pipelineMultisampleStateInfo.rasterizationSamples  = Util::GetMultisampleCount(_jget_uint(multisampleInfo["multisample_count"]));
			pipelineMultisampleStateInfo.sampleShadingEnable   = _jget_uint(multisampleInfo["sample_shading_enable"]);
			pipelineMultisampleStateInfo.minSampleShading      = _jget_float(multisampleInfo["min_sample_shading_factor"]);
			pipelineMultisampleStateInfo.pSampleMask           = pSampleMasks;
			pipelineMultisampleStateInfo.alphaToCoverageEnable = _jget_uint(multisampleInfo["alpha_to_coverage_enable"]);
			pipelineMultisampleStateInfo.alphaToOneEnable      = _jget_uint(multisampleInfo["alpha_to_one_enable"]);		
		}

		// Depth Stencil State.
		pGraphicInfos[i].pDepthStencilState = &pipelineDepthStencilStateInfo;
		auto& depthStencilInfo              = graphicInfo["depth_stencil_state"];
		if (depthStencilInfo != Json::nullValue)
		{
			pipelineDepthStencilStateInfo.flags                 = _jget_uint(depthStencilInfo["flags"]);
			pipelineDepthStencilStateInfo.depthTestEnable       = _jget_uint(depthStencilInfo["depth_test_enable"]);
			pipelineDepthStencilStateInfo.depthWriteEnable      = _jget_uint(depthStencilInfo["depth_write_enable"]);
			pipelineDepthStencilStateInfo.depthCompareOp        = Util::GetCompareOp(_jget_string_default(depthStencilInfo["depth_compare_op"], Util::DefaultCompareOp));
			pipelineDepthStencilStateInfo.depthBoundsTestEnable = _jget_uint(depthStencilInfo["depth_bounds_test_enable"]);
			pipelineDepthStencilStateInfo.stencilTestEnable     = _jget_uint(depthStencilInfo["stencil_test_enable"]);
			pipelineDepthStencilStateInfo.minDepthBounds        = _jget_float(depthStencilInfo["min_depth_bounds"]);
			pipelineDepthStencilStateInfo.maxDepthBounds        = _jget_float(depthStencilInfo["max_depth_bounds"]);

			auto& stencilInfo = depthStencilInfo["stencil_test_state"];
			if (stencilInfo != Json::nullValue)
			{
				if (!_jis_auto(stencilInfo["front"]))
				{
					pipelineDepthStencilStateInfo.front.failOp      = Util::GetStencilOp(_jget_string_default(stencilInfo["front"]["fail_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.front.passOp      = Util::GetStencilOp(_jget_string_default(stencilInfo["front"]["pass_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.front.depthFailOp = Util::GetStencilOp(_jget_string_default(stencilInfo["front"]["depth_fail_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.front.compareOp   = Util::GetCompareOp(_jget_string_default(stencilInfo["front"]["compare_op"], Util::DefaultCompareOp));
					pipelineDepthStencilStateInfo.front.compareMask = _jget_hex(stencilInfo["front"]["compare_mask"]);
					pipelineDepthStencilStateInfo.front.writeMask   = _jget_hex(stencilInfo["front"]["write_mask"]);
					pipelineDepthStencilStateInfo.front.reference   = _jget_uint(stencilInfo["front"]["reference"]);
				}

				if (!_jis_auto(stencilInfo["back"]))
				{
					pipelineDepthStencilStateInfo.back.failOp      = Util::GetStencilOp(_jget_string_default(stencilInfo["back"]["fail_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.back.passOp      = Util::GetStencilOp(_jget_string_default(stencilInfo["back"]["pass_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.back.depthFailOp = Util::GetStencilOp(_jget_string_default(stencilInfo["back"]["depth_fail_op"], Util::DefaultStencilOp));
					pipelineDepthStencilStateInfo.back.compareOp   = Util::GetCompareOp(_jget_string_default(stencilInfo["back"]["compare_op"], Util::DefaultCompareOp));
					pipelineDepthStencilStateInfo.back.compareMask = _jget_hex(stencilInfo["back"]["compare_mask"]);
					pipelineDepthStencilStateInfo.back.writeMask   = _jget_hex(stencilInfo["back"]["write_mask"]);
					pipelineDepthStencilStateInfo.back.reference   = _jget_uint(stencilInfo["back"]["reference"]);
				}			

				if (_jis_auto(stencilInfo["front"]))
					pipelineDepthStencilStateInfo.front = pipelineDepthStencilStateInfo.back;
				if (_jis_auto(stencilInfo["back"]))
					pipelineDepthStencilStateInfo.back  = pipelineDepthStencilStateInfo.front;
			}
		}

		// Color Blend State.
		pGraphicInfos[i].pColorBlendState = &pipelineColorBlendStateInfo;
		auto& colorBlendInfo              = graphicInfo["color_blend_state"];
		if (colorBlendInfo != Json::nullValue)
		{
			bIsArray                    = colorBlendInfo["attachments"].isArray();
			uint32 numAttachment        = bIsArray ? colorBlendInfo["attachments"].size() : _count_1;
			pColorBlendAttachmentStates = _safe_new(VkPipelineColorBlendAttachmentState, numAttachment);
			for (uint32 j = 0; j < numAttachment; j++)
			{
				auto& attachment = bIsArray ? colorBlendInfo["attachments"][j] : colorBlendInfo["attachments"];

				pColorBlendAttachmentStates[j].blendEnable         = _jget_uint(attachment["blend_enable"]);
				pColorBlendAttachmentStates[j].srcColorBlendFactor = attachment["src_color_factor"].isUInt() ? static_cast<VkBlendFactor>(_jget_uint(attachment["src_color_factor"])) : Util::GetBlendFactor(_jget_string_default(attachment["src_color_factor"], Util::DefaultBlendFactor));
				pColorBlendAttachmentStates[j].dstColorBlendFactor = attachment["dst_color_factor"].isUInt() ? static_cast<VkBlendFactor>(_jget_uint(attachment["dst_color_factor"])) : Util::GetBlendFactor(_jget_string_default(attachment["dst_color_factor"], Util::DefaultBlendFactor));
				pColorBlendAttachmentStates[j].colorBlendOp        = Util::GetBlendOp(_jget_string_default(attachment["color_blend_op"], Util::DefaultBlendOp));
				pColorBlendAttachmentStates[j].srcAlphaBlendFactor = attachment["src_alpha_factor"].isUInt() ? static_cast<VkBlendFactor>(_jget_uint(attachment["src_alpha_factor"])) : Util::GetBlendFactor(_jget_string_default(attachment["src_alpha_factor"], Util::DefaultBlendFactor));
				pColorBlendAttachmentStates[j].dstAlphaBlendFactor = attachment["dst_alpha_factor"].isUInt() ? static_cast<VkBlendFactor>(_jget_uint(attachment["dst_alpha_factor"])) : Util::GetBlendFactor(_jget_string_default(attachment["dst_alpha_factor"], Util::DefaultBlendFactor));
				pColorBlendAttachmentStates[j].alphaBlendOp        = Util::GetBlendOp(_jget_string_default(attachment["alpha_blend_op"], Util::DefaultBlendOp));
				pColorBlendAttachmentStates[j].colorWriteMask      = Util::GetColorComponentMask(_jget_string_default(attachment["component_mask"], Util::DefaultColorComponentMask));
			}

			pipelineColorBlendStateInfo.flags           = _jget_uint(colorBlendInfo["flags"]);
			pipelineColorBlendStateInfo.logicOpEnable   = _jget_uint(colorBlendInfo["logic_op_enable"]);
			pipelineColorBlendStateInfo.logicOp         = Util::GetLogicOp(_jget_string_default(colorBlendInfo["logic_op"], Util::DefaultLogicOp));
			pipelineColorBlendStateInfo.attachmentCount = numAttachment;
			pipelineColorBlendStateInfo.pAttachments    = pColorBlendAttachmentStates;

			bIsArray           = colorBlendInfo["blend_constants"].isArray();
			uint32 numConstant = bIsArray ? colorBlendInfo["blend_constants"].size() : _count_1;
			for (uint32 j = 0; j < numConstant; j++)
			{
				if (j >= 4) break;
				pipelineColorBlendStateInfo.blendConstants[j] = _jget_float(bIsArray ? colorBlendInfo["blend_constants"][j] : colorBlendInfo["blend_constants"]);
			}
		}

		// Dynamic State.
		pGraphicInfos[i].pDynamicState = &pipelineDynamicStateInfo;
		auto& dynamicStateInfo         = graphicInfo["dynamic_state"];
		if (dynamicStateInfo != Json::nullValue)
		{
			bIsArray               = dynamicStateInfo["state"].isArray();
			uint32 numDynamicState = bIsArray ? dynamicStateInfo["state"].size() : _count_1;
			pDynamicStates         = _safe_new(VkDynamicState, numDynamicState);
			for (uint32 j = 0; j < numDynamicState; j++)
				pDynamicStates[j] = Util::GetDynamicState(_jget_string_default(bIsArray ? dynamicStateInfo["state"][j] : dynamicStateInfo["state"], Util::DefaultDynamicState));

			pipelineDynamicStateInfo.flags             = _jget_uint(dynamicStateInfo["flags"]);
			pipelineDynamicStateInfo.dynamicStateCount = numDynamicState;
			pipelineDynamicStateInfo.pDynamicStates    = pDynamicStates;
		}

		// Pipeline Layout.
		 

		// this->CreatePipelineLayout();
	}




	// _vk_try(vkCreateGraphicsPipelines(m_device, InPipCache, numGInfo, pGraphicInfos, GetVkAllocator(), OutPipeline));

	// TODO:

	_safe_delete_array(pGraphicInfos);
	_safe_delete_array(pShaderInfos);
	_safe_delete_array(pSpecMaps);
	_safe_delete_array(pSpecData);
	_safe_delete_array(pVertexInputBindings);
	_safe_delete_array(pVertexInputAttributes);
	_safe_delete_array(pSampleMasks);
	_safe_delete_array(pColorBlendAttachmentStates);
	_safe_delete_array(pDynamicStates);
}

void LogicalDevice::FlushAllQueue()
{
	_vk_try(vkDeviceWaitIdle(m_device));
}

void LogicalDevice::ResetCmdPool()
{
	_vk_try(vkResetCommandPool(m_device, *m_pCmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
}
