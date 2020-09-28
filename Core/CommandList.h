//
// CommandList.h
//

#pragma once

#include "Common.h"
#include "Global.h"

class BaseLayer;

class CommandList
{

protected:

	VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;

	VkDevice        m_device    = VK_NULL_HANDLE;
	VkCommandPool   m_cmdPool   = VK_NULL_HANDLE;

	BaseLayer*      m_pBaseLayer = nullptr;

public:

	CommandList() {}
	CommandList(BaseLayer* InBaseLayer);
	virtual ~CommandList();

public:

	VkCommandBuffer GetCmdBuffer() const;

public:

	void Reset();
	void Reset(VkCommandBufferResetFlags InFlags);

	// This func is beyond responsibility of cmdList.
	void ResetPool();

	void Free();

	void Close();

public:

	// Buffer.
	void CopyBuffer             (VkBuffer InSrcBuffer, VkBuffer InDstBuffer);
	void CopyBuffer             (VkBuffer InSrcBuffer, VkBuffer InDstBuffer, const VkBufferCopy& InRegion);	
	void CopyBuffer             (VkBuffer InSrcBuffer, VkBuffer InDstBuffer, uint32 InRegionCount, const VkBufferCopy* InRegions);

	void ClearBufferUint32      (VkBuffer InBuffer, const uint32 InValue);
	void ClearBufferFloat       (VkBuffer InBuffer, const float  InValue);
	void ClearBufferUint32      (VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const uint32 InValue);
	void ClearBufferFloat       (VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const float  InValue);

	void UpdateBuffer           (VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const void* InData);

	// Image.
	void CopyBufferToImage      (VkBuffer InSrcBuffer, VkImage InDstImage, uint32 InWidth, uint32 InHeight, VkImageAspectFlags InAspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
	void CopyBufferToImage      (VkBuffer InSrcBuffer, VkImage InDstImage, const VkBufferImageCopy& InRegion);
	void CopyBufferToImage      (VkBuffer InSrcBuffer, VkImage InDstImage, uint32 InRegionCount, const VkBufferImageCopy* InRegions);

	void CopyImageToBuffer      (VkImage InSrcImage, uint32 InWidth, uint32 InHeight, VkBuffer InDstBuffer, VkImageAspectFlags InAspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
	void CopyImageToBuffer      (VkImage InSrcImage, VkBuffer InDstBuffer, const VkBufferImageCopy& InRegion);
	void CopyImageToBuffer      (VkImage InSrcImage, VkBuffer InDstBuffer, uint32 InRegionCount, const VkBufferImageCopy* InRegions);

	void CopyImage              (VkImage InSrcImage, VkImage InDstImage, uint32 InWidth, uint32 InHeight, VkImageAspectFlags InAspectMask = VK_IMAGE_ASPECT_COLOR_BIT);
	void CopyImage              (VkImage InSrcImage, VkImage InDstImage, const VkImageCopy& InRegion);
	void CopyImage              (VkImage InSrcImage, VkImage InDstImage, uint32 InRegionCount, const VkImageCopy* InRegions);

	void NonUniformImageCopy    (VkImage InSrcImage, VkImage InDstImage, const VkImageBlit& InRegion, VkFilter InFilter);
	void NonUniformImageCopy    (VkImage InSrcImage, VkImage InDstImage, uint32 InRegionCount, const VkImageBlit* InRegions, VkFilter InFilter);

	void ClearColorImage        (VkImage InImage, const float* InClearColor);
	void ClearColorImage        (VkImage InImage, const VkClearColorValue* InClearColor);
	void ClearColorImage        (VkImage InImage, const VkClearColorValue* InClearColor, const VkImageSubresourceRange& InSubresRange);
	void ClearColorImage        (VkImage InImage, const VkClearColorValue* InClearColor, uint32 InRangeCount, const VkImageSubresourceRange* InSubresRanges);

	void ClearDepthStencilImage (VkImage InImage, float InClearDepthValue, uint32 InClearStencilValue);
	void ClearDepthStencilImage (VkImage InImage, const VkClearDepthStencilValue* InClearValue);

	void BindPipeline           (VkPipeline InPipeline, VkPipelineBindPoint InPipBindPoint);
	void BindComputePipeline    (VkPipeline InPipeline);
	void BindGraphicPipeline    (VkPipeline InPipeline);

	void Dispatch               (uint32 x, uint32 y, uint32 z);
	void DispatchIndirect       (VkBuffer InBuffer, VkDeviceSize InOffset);

	void BindDescriptorSets     (VkPipelineLayout InPipLayout, VkPipelineBindPoint InPipBindPoint, const VkDescriptorSet* InDescSets, uint32 InSetCount = _count_1, uint32 InSetOffset = _offset_0, const uint32* InDynamicOffsets = nullptr, uint32 InDynamicOffsetCount = _count_0);
	void BindComputeDescSets    (VkPipelineLayout InPipLayout, const VkDescriptorSet* InDescSets, uint32 InSetCount = _count_1, uint32 InSetOffset = _offset_0, const uint32* InDynamicOffsets = nullptr, uint32 InDynamicOffsetCount = _count_0);
	void BindGraphicDescSets    (VkPipelineLayout InPipLayout, const VkDescriptorSet* InDescSets, uint32 InSetCount = _count_1, uint32 InSetOffset = _offset_0, const uint32* InDynamicOffsets = nullptr, uint32 InDynamicOffsetCount = _count_0);

	void PushConstants          (VkPipelineLayout InPipLayout, VkShaderStageFlags InStageFlags, const void* InValues, uint32 InSize, uint32 InOffset = _offset_0);

	void DrawVertexInstanced    (uint32 InStartVertex, uint32 InVertexCount, uint32 InStartInstance = _offset_start, uint32 InInstanceCount = _count_1);
	void DrawIndexedInstanced   (uint32 InStartIndex, uint32 InIndexCount, uint32 InStartVertex = _offset_start, uint32 InStartInstance = _offset_start, uint32 InInstanceCount = _count_1);
	void DrawVertexIndirect     (VkBuffer InBuffer, VkDeviceSize InOffset, uint32 InDrawCount, uint32 InStride);
	void DrawIndexedIndirect    (VkBuffer InBuffer, VkDeviceSize InOffset, uint32 InDrawCount, uint32 InStride);


#pragma region PiplineBarrier

public:

	struct SBufferBarrier
	{
		VkPipelineStageFlags srcStageMask;
		VkPipelineStageFlags dstStageMask;
		VkAccessFlags        srcAccessMask;
		VkAccessFlags        dstAccessMask;
		VkBuffer             buffer;
		VkDeviceSize         offset;
		VkDeviceSize         size;
	};

	struct SImageBarrier
	{
		VkPipelineStageFlags srcStageMask;
		VkPipelineStageFlags dstStageMask;
		VkAccessFlags        srcAccessMask;
		VkAccessFlags        dstAccessMask;
		VkImageLayout        oldLayout;
		VkImageLayout        newLayout;
		VkImage              image;
		VkImageSubresourceRange subresourceRange = Util::ColorSubresRange;
	};

	void ResourceBarriers(
		VkPipelineStageFlags         InSrcStageMask,
		VkPipelineStageFlags         InDstStageMask,		
		// Memory Barrier.
		uint32                       InMemBarrierCount,
		const VkMemoryBarrier*       InMemBarriers,
		// Buffer Memory Barrier.
		uint32                       InBufferMemBarrierCount,
		const VkBufferMemoryBarrier* InBufferMemBarriers,
		// Image Memory Barrier.
		uint32                       InImageMemBarrierCount,
		const VkImageMemoryBarrier*  InImageMemoryBarriers,
		// Dependency Flags
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void MemoryBarrier(
		VkPipelineStageFlags   InSrcStageMask,
		VkPipelineStageFlags   InDstStageMask,
		const VkMemoryBarrier& InMemBarrier,
		VkDependencyFlags      InDependencyFlags = _flag_none);

	void MemoryBarrier(
		VkPipelineStageFlags   InSrcStageMask,
		VkPipelineStageFlags   InDstStageMask,
		VkAccessFlags          InSrcAccessMask,
		VkAccessFlags          InDstAccessMask,
		VkDependencyFlags      InDependencyFlags = _flag_none);

	void MemoryBarriers(
		VkPipelineStageFlags   InSrcStageMask,
		VkPipelineStageFlags   InDstStageMask,
		uint32                 InMemBarrierCount,
		const VkMemoryBarrier* InMemBarriers,
		VkDependencyFlags      InDependencyFlags = _flag_none);

	void BufferBarrier(
		VkPipelineStageFlags         InSrcStageMask,
		VkPipelineStageFlags         InDstStageMask,
		const VkBufferMemoryBarrier& InBufferMemBarrier,
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void BufferBarrier(
		const SBufferBarrier&        InSBufferBarrier, 
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void BufferBarriers(
		VkPipelineStageFlags         InSrcStageMask,
		VkPipelineStageFlags         InDstStageMask,
		uint32                       InBufferMemBarrierCount,
		const VkBufferMemoryBarrier* InBufferMemBarriers,
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void ImageBarrier(
		VkPipelineStageFlags         InSrcStageMask,
		VkPipelineStageFlags         InDstStageMask,
		const VkImageMemoryBarrier&  InImageMemBarrier,
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void ImageBarrier(
		const SImageBarrier&         InSImageBarrier, 
		VkDependencyFlags            InDependencyFlags = _flag_none);

	void ImageBarriers(
		VkPipelineStageFlags         InSrcStageMask,
		VkPipelineStageFlags         InDstStageMask,
		uint32                       InImageMemBarrierCount,
		const VkImageMemoryBarrier*  InImageMemBarriers,
		VkDependencyFlags            InDependencyFlags = _flag_none);

#pragma endregion

};