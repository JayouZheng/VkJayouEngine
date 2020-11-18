//
// LogicalDevice.h
//

#pragma once

#include "Core/Common.h"
#include "Core/SmartPtr/VkSmartPtr.h"
#include "Core/Render/GLSLCompiler.h"

class BaseLayer;
class BaseAllocator;
class CommandQueue;
class Window;

#include "Core/Base/Interface/IResourceHandler.h"

class LogicalDevice : public IResourceHandler
{

protected:

	VkDevice        m_device     = VK_NULL_HANDLE;
	BaseLayer*      m_pBaseLayer = nullptr;
	BaseAllocator*  m_pAllocator = nullptr;
	Window*         m_pWindow    = nullptr;

	GLSLCompiler    m_compiler;

	_declare_vk_smart_ptr(VkCommandPool,     m_pCmdPool);
	_declare_vk_smart_ptr(VkDescriptorPool,  m_pDescPool);

	std::vector<VkPipelineCache>             m_pipelineCaches;
	std::vector<VkSmartPtr<VkPipelineCache>> m_pipelineCachePtrs;

	std::unordered_map<std::string, uint32>  m_subpassNameIDMap;

	std::unordered_map<std::string, VkSmartPtr<VkRenderPass>> m_renderPassNamePtrMap;
	std::unordered_map<std::string, VkSmartPtr<VkPipeline>>   m_pipelineNamePtrMap;

	VkAllocationCallbacks* GetVkAllocator() const;

public:

	LogicalDevice() {}
	LogicalDevice(const VkDevice& InDevice);
	LogicalDevice& operator=(const VkDevice& InDevice);

	virtual ~LogicalDevice() {}

public:

	operator VkDevice() const;

	bool operator==(const VkDevice& InDevice) const;

	VkDevice  GetVkDevice() const;
	VkDevice* GetAddressOfVkDevice();

public:

	void SetVkDevice  (const VkDevice& InDevice   );
	void SetBaseLayer (BaseLayer*      InBaseLayer);
	void SetAllocator (BaseAllocator*  InAllocator);
	void SetWindow    (Window*         InWindow   );

	bool IsNoneAllocator() const;

	VkCommandPool GetCmdPool();

	void SetViewport(VkViewport& OutViewport, VkRect2D& OutScissor, uint32 InWidth, uint32 InHeight);

public:

	struct PipelineComputeDesc
	{
		std::string           EntryPoint;
		VkShaderModule        ShaderModule;
		VkPipelineLayout      PipLayout;	
		VkSpecializationInfo* pSpecialConstInfo;

		// Pipeline Derivatives.
		VkPipeline            BasePipelineHandle;
		int32                 BasePipelineIndex;

		PipelineComputeDesc()
		{
			EntryPoint         = "main";
			ShaderModule       = VK_NULL_HANDLE;
			PipLayout          = VK_NULL_HANDLE;
			pSpecialConstInfo  = nullptr;

			BasePipelineHandle = VK_NULL_HANDLE;
			BasePipelineIndex  = -1;
		}
	};

	struct PipelineGraphicDesc
	{
		VkRenderPass          RenderPass;

		std::string           EntryPoint;
		VkShaderModule        ShaderModule;
		VkPipelineLayout      PipLayout;
		VkSpecializationInfo* pSpecialConstInfo;

		// Pipeline Derivatives.
		VkPipeline            BasePipelineHandle;
		int32                 BasePipelineIndex;

		PipelineGraphicDesc()
		{
			RenderPass         = VK_NULL_HANDLE;

			EntryPoint         = "main";
			ShaderModule       = VK_NULL_HANDLE;
			PipLayout          = VK_NULL_HANDLE;
			pSpecialConstInfo  = nullptr;

			BasePipelineHandle = VK_NULL_HANDLE;
			BasePipelineIndex  = -1;
		}
	};

	struct PipelineCacheHeader
	{
		uint32 Length;
		uint32 Version;
		uint32 VendorID;
		uint32 DeviceID;
		uint8  UUID[VK_UUID_SIZE];

		PipelineCacheHeader() {}

		PipelineCacheHeader(const VkPhysicalDeviceProperties& InPDProp)
		{
			Length   = 32;
			Version  = VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
			VendorID = InPDProp.vendorID;
			DeviceID = InPDProp.deviceID;
			std::memcpy(UUID, InPDProp.pipelineCacheUUID, VK_UUID_SIZE);
		}

		usize GetDataSize() const
		{
			return 32;
		}

		void* GetData() const
		{
			return (void*)this;
		}
	};

	// TODO: Remove Pipeline Cache param from API.

	CommandQueue   GetQueue                      (uint32 InQueueFamilyIndex, uint32 InQueueIndex = _index_0);
	void           GetSwapchainImagesKHR         (VkSwapchainKHR InSwapchain, uint32* InOutImageCount, VkImage* OutImages);
	uint32         GetSwapchainNextImageKHR      (VkSwapchainKHR InSwapchain, uint64 InTimeout, VkSemaphore InSemaphore, VkFence InFence);

	VkRenderPass   GetRenderPass                 (const std::string& InName);
	VkPipeline     GetPipeline                   (const std::string& InName);

	// Simple Stupid API (In fact, for all create funs, we need to gather all create infos first, do creating next!).
	void           CreateCommandPool             (const VkCommandPoolCreateInfo& InCreateInfo);
	void           CreateCommandPool             (uint32 InQueueFamilyIndex, VkCommandPoolCreateFlags InFlags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	void           CreateSwapchainKHR            (VkSwapchainKHR* OutSwapchain, const VkSwapchainCreateInfoKHR& InCreateInfo);

	void           CreateShaderModule            (VkShaderModule* OutShaderModule, const VkShaderModuleCreateInfo& InCreateInfo);
	void           CreateShaderModule            (VkShaderModule* OutShaderModule, const uint32* InCodes, usize InCodeSize);
	bool           CreateShaderModule            (VkShaderModule* OutShaderModule, const char* InShaderPath, const char* InEntrypoint = "main", VkShaderStageFlags* OutShaderStage = nullptr);

	void           CreateComputePipelines        (VkPipeline* OutPipeline, const VkComputePipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount = _count_1, VkPipelineCache InPipCache = VK_NULL_HANDLE);
	void           CreateComputePipeline         (VkPipeline* OutPipeline, VkPipelineLayout InPipLayout, VkShaderModule InShaderModule, const char* InShaderEntryName = "main", const VkSpecializationInfo* InSpecialConstInfo = nullptr, VkPipelineCache InPipCache = VK_NULL_HANDLE);
	void           CreateComputePipelines        (VkPipeline* OutPipeline, const PipelineComputeDesc* InDescs, uint32 InDescCount = _count_1, VkPipelineCache InPipCache = VK_NULL_HANDLE);

	void           CreatePipelineCache           (VkPipelineCache* OutPipCache, const VkPipelineCacheCreateInfo& InCreateInfo);
	void           CreatePipelineCache           (VkPipelineCache* OutPipCache, const void* InData, usize InSize, VkPipelineCacheCreateFlags InFlags = _flag_none);
	bool           CreateEmptyPipelineCache      (VkPipelineCache* OutPipCache);
	bool           CreatePipelineCacheFromFile   (VkPipelineCache* OutPipCache, const char* InPath);
	usize         GetPipelineCacheDataSize      (VkPipelineCache  InPipCache);
	void           GetPipelineCacheData          (VkPipelineCache  InPipCache, usize InDataSize, void* OutData);
	void           GetPipelineCacheData          (VkPipelineCache  InPipCache, std::vector<uint8>& OutData);
	bool           SavePipelineCacheToFile       (const char*      InPath);
	void           MergePipelineCaches           (VkPipelineCache  OutMergedPipCache, const VkPipelineCache* InPipCaches, uint32 InSrcPipCacheCount);

	void           CreateDescriptorSetLayout     (VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutCreateInfo& InCreateInfo);
	void           CreateDescriptorSetLayout     (VkDescriptorSetLayout* OutLayout, const VkDescriptorSetLayoutBinding* InBindings, uint32 InBindingCount);
	void           CreateSingleDescriptorLayout  (VkDescriptorSetLayout* OutLayout, VkDescriptorType InDescType, VkShaderStageFlags InShaderStage, const VkSampler* InImmutableSamplers = nullptr);

	void           CreatePipelineLayout          (VkPipelineLayout* OutLayout, const VkPipelineLayoutCreateInfo& InCreateInfo);
	void           CreatePipelineLayout          (VkPipelineLayout* OutLayout, const VkDescriptorSetLayout* InDescSetLayouts, uint32 InSetCount = _count_1, const VkPushConstantRange* InPushConstants = nullptr, uint32 InConstCount = _count_0);

	void           CreateDescriptorPool          (const VkDescriptorPoolCreateInfo& InCreateInfo);
	void           CreateDescriptorPool          (uint32 InMaxSets, const VkDescriptorPoolSize* InPerDescTypeCounts, uint32 InDescTypeCount);

	void           AllocatorDescriptorSets       (VkDescriptorSet* OutDescSet, const VkDescriptorSetAllocateInfo& InAllocateInfo);
	void           AllocatorDescriptorSets       (VkDescriptorSet* OutDescSet, const VkDescriptorSetLayout* InSetLayouts, uint32 InSetCount = _count_1);
	void           FreeDescriptorSets            (const VkDescriptorSet* InDescSets, uint32 InSetCount = _count_1);
	void           ResetDescriptorPool           (VkDescriptorPoolResetFlags InFlags = _flag_none);

	void           UpdateDescriptorSets          (const VkWriteDescriptorSet* InDescWrites, uint32 InWriteSetCount = _count_1, const VkCopyDescriptorSet* InDescCopies = nullptr, uint32 InCopySetCount = _count_0);
	bool           UpdateImageOfDescSet			 (VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InImageDescType, const VkDescriptorImageInfo* InImageInfos, uint32 InImageCount = _count_1, uint32 InSetOffset = _offset_0);
	bool           UpdateBufferOfDescSet		 (VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InBufferDescType, const VkDescriptorBufferInfo* InBufferInfos, uint32 InBufferCount = _count_1, uint32 InSetOffset = _offset_0);
	bool           UpdateTexelBufferOfDescSet    (VkDescriptorSet InDescSet, uint32 InBindingIndex, VkDescriptorType InTBufferDescType, const VkBufferView* InTBufferViews, uint32 InTBufferCount = _count_1, uint32 InSetOffset = _offset_0);
	void           CopyDescriptorSets            (const VkCopyDescriptorSet* InDescCopies, uint32 InCopySetCount = _count_1);
	void           CopyDescriptorSet             (VkDescriptorSet InSrcSet, uint32 InSrcBindingIndex, VkDescriptorSet InDstSet, uint32 InDstBindingIndex, uint32 InCopyDescCount, uint32 InSrcSetOffset = _offset_0, uint32 InDstSetOffset = _offset_0);

	// TODO: Replace Sampler API with Sampler Enum.

	void           CreateSampler                 (VkSampler* OutSampler, const VkSamplerCreateInfo& InCreateInfo);
	void           CreatePointWrapSampler        (VkSampler* OutSampler);
	void           CreatePointClampSampler       (VkSampler* OutSampler);
	void           CreateLinearWrapSampler       (VkSampler* OutSampler);
	void           CreateLinearClampSampler      (VkSampler* OutSampler);
	bool           CreateAnisotropicWrapSampler  (VkSampler* OutSampler);
	bool           CreateAnisotropicClampSampler (VkSampler* OutSampler);
	void           CreatePCFSampler              (VkSampler* OutSampler);

	void           CreateRenderPass              (VkRenderPass* OutRenderPass, const VkRenderPassCreateInfo& InCreateInfo);
	bool           CreateRenderPass              (const std::string& InJsonPath);
	void           CreateSingleRenderPass        (VkRenderPass* OutRenderPass, VkFormat InColorFormat, VkFormat InDepthFormat);

	bool           CreateFrameBuffer             (VkFramebuffer* OutFrameBuffer, const VkFramebufferCreateInfo& InCreateInfo);
	bool           CreateFrameBuffer             (VkFramebuffer* OutFrameBuffer, VkRenderPass InRenderPass, const VkImageView* InImageViews, uint32 InViewCount, VkExtent3D InSize);

	void           CreateGraphicPipelines        (VkPipeline* OutPipeline, const VkGraphicsPipelineCreateInfo* InCreateInfos, uint32 InCreateInfoCount = _count_1, VkPipelineCache InPipCache = VK_NULL_HANDLE);
	void           CreateGraphicPipelines        (VkPipeline* OutPipeline, const PipelineGraphicDesc* InDescs, uint32 InDescCount = _count_1, VkPipelineCache InPipCache = VK_NULL_HANDLE);
	bool           CreateGraphicPipelines        (const std::string& InJsonPath, VkPipelineCache InPipCache = VK_NULL_HANDLE);

	void           FlushAllQueue();
	void           ResetCmdPool();

};

#define CreateRenderTarget    CreateFrameBuffer
typedef VkFramebuffer         VkRenderTarget;
