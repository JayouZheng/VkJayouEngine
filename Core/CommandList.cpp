//
// CommandList.cpp
//

#include "CommandList.h"
#include "BaseLayer.h"

CommandList::CommandList(BaseLayer* InBaseLayer)
	: m_baseLayer(InBaseLayer)
{
	m_device = InBaseLayer->GetLogicalDevice().GetVkDevice();
	m_cmdPool = InBaseLayer->GetLogicalDevice().GetCmdPool();

	// Single Primary Command Buffer.
	VkCommandBufferAllocateInfo cmdBufferAllocInfo = {};
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.pNext = nullptr;
	cmdBufferAllocInfo.commandPool = m_cmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = 1;

	_vk_try(vkAllocateCommandBuffers(m_device, &cmdBufferAllocInfo, &m_cmdBuffer));

	
}

CommandList::~CommandList()
{
	Free();
}

VkCommandBuffer CommandList::GetCmdBuffer() const
{
	return m_cmdBuffer;
}

void CommandList::Reset()
{
	VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufferBeginInfo.pNext = nullptr;
	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	cmdBufferBeginInfo.pInheritanceInfo = nullptr;

	_vk_try(vkBeginCommandBuffer(m_cmdBuffer, &cmdBufferBeginInfo));
}

void CommandList::Reset(VkCommandBufferResetFlags InFlags)
{
	_vk_try(vkResetCommandBuffer(m_cmdBuffer, InFlags));
}

void CommandList::ResetPool()
{
	_vk_try(vkResetCommandPool(m_device, m_cmdPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
}

void CommandList::Free()
{
	vkFreeCommandBuffers(m_device, m_cmdPool, _count_1, &m_cmdBuffer);
}

void CommandList::Close()
{
	_vk_try(vkEndCommandBuffer(m_cmdBuffer));
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer)
{
	VkBufferCopy region = {};
	region.srcOffset = _offset_start;
	region.dstOffset = _offset_start;
	region.size = VK_WHOLE_SIZE;

	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, _count_1, &region);
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer, const VkBufferCopy& InRegion)
{
	_exit_log(InRegion.srcOffset % 4 != 0, "CopyBuffer, SrcOffset is not a multiple of 4!");
	_exit_log(InRegion.dstOffset % 4 != 0, "CopyBuffer, DstOffset is not a multiple of 4!");
	_exit_log(InRegion.size % 4 != 0, "CopyBuffer, Size is not a multiple of 4!");

	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, _count_1, &InRegion);
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer, uint32 InRegionCount, const VkBufferCopy* InRegions)
{
	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, InRegionCount, InRegions);
}

void CommandList::ClearBufferUint32(VkBuffer InBuffer, const uint32 InValue)
{
	vkCmdFillBuffer(m_cmdBuffer, InBuffer, _offset_start, VK_WHOLE_SIZE, InValue);
}

void CommandList::ClearBufferFloat(VkBuffer InBuffer, const float InValue)
{
	vkCmdFillBuffer(m_cmdBuffer, InBuffer, _offset_start, VK_WHOLE_SIZE, *(const uint32*)&InValue);
}

void CommandList::ClearBufferUint32(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const uint32 InValue)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferUint32, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferUint32, Size is not a multiple of 4!");

	vkCmdFillBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, InValue);
}

void CommandList::ClearBufferFloat(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const float InValue)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferFloat, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferFloat, Size is not a multiple of 4!");

	vkCmdFillBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, *(const uint32*)&InValue);
}

void CommandList::UpdateBuffer(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const void* InData)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferFloat, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferFloat, Size is not a multiple of 4!");
	_exit_log(InSize > 65536u, "ClearBufferFloat, The maximum size of data that can be placed in a buffer with vkCmdUpdateBuffer() is 65,536 bytes!");

	vkCmdUpdateBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, InData);
}

void CommandList::CopyBufferToImage(VkBuffer InSrcBuffer, VkImage InDstImage, uint32 InWidth, uint32 InHeight, VkImageAspectFlags InAspectMask /*= VK_IMAGE_ASPECT_COLOR_BIT*/)
{
	VkImageSubresourceLayers imageSubresLayers = {};
	imageSubresLayers.aspectMask = InAspectMask;
	imageSubresLayers.mipLevel = _index_0;
	imageSubresLayers.baseArrayLayer = _index_0;
	imageSubresLayers.layerCount = _count_1;

	VkBufferImageCopy region = {};
	region.bufferOffset = _offset_start;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource = imageSubresLayers;
	region.imageOffset.x = _offset_start;
	region.imageOffset.y = _offset_start;
	region.imageOffset.z = 0;
	region.imageExtent.width = InWidth;
	region.imageExtent.height = InHeight;
	region.imageExtent.depth = 0;

	vkCmdCopyBufferToImage(m_cmdBuffer, InSrcBuffer, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CommandList::CopyBufferToImage(VkBuffer InSrcBuffer, VkImage InDstImage, const VkBufferImageCopy& InRegion)
{
	vkCmdCopyBufferToImage(m_cmdBuffer, InSrcBuffer, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &InRegion);
}

void CommandList::CopyBufferToImage(VkBuffer InSrcBuffer, VkImage InDstImage, uint32 InRegionCount, const VkBufferImageCopy* InRegions)
{
	vkCmdCopyBufferToImage(m_cmdBuffer, InSrcBuffer, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, InRegionCount, InRegions);
}

void CommandList::CopyImageToBuffer(VkImage InSrcImage, uint32 InWidth, uint32 InHeight, VkBuffer InDstBuffer, VkImageAspectFlags InAspectMask /*= VK_IMAGE_ASPECT_COLOR_BIT*/)
{
	VkImageSubresourceLayers imageSubresLayers = {};
	imageSubresLayers.aspectMask = InAspectMask;
	imageSubresLayers.mipLevel = _index_0;
	imageSubresLayers.baseArrayLayer = _index_0;
	imageSubresLayers.layerCount = _count_1;

	VkBufferImageCopy region = {};
	region.bufferOffset = _offset_start;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource = imageSubresLayers;
	region.imageOffset.x = _offset_start;
	region.imageOffset.y = _offset_start;
	region.imageOffset.z = 0;
	region.imageExtent.width = InWidth;
	region.imageExtent.height = InHeight;
	region.imageExtent.depth = 0;

	vkCmdCopyImageToBuffer(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstBuffer, 1, &region);
}

void CommandList::CopyImageToBuffer(VkImage InSrcImage, VkBuffer InDstBuffer, const VkBufferImageCopy& InRegion)
{
	vkCmdCopyImageToBuffer(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstBuffer, 1, &InRegion);
}

void CommandList::CopyImageToBuffer(VkImage InSrcImage, VkBuffer InDstBuffer, uint32 InRegionCount, const VkBufferImageCopy* InRegions)
{
	vkCmdCopyImageToBuffer(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstBuffer, InRegionCount, InRegions);
}

void CommandList::CopyImage(VkImage InSrcImage, VkImage InDstImage, uint32 InWidth, uint32 InHeight, VkImageAspectFlags InAspectMask /*= VK_IMAGE_ASPECT_COLOR_BIT*/)
{
	VkImageSubresourceLayers imageSubresLayers = {};
	imageSubresLayers.aspectMask = InAspectMask;
	imageSubresLayers.mipLevel = _index_0;
	imageSubresLayers.baseArrayLayer = _index_0;
	imageSubresLayers.layerCount = _count_1;

	VkImageCopy region = {};
	region.srcSubresource = imageSubresLayers;
	region.srcOffset = { _offset_start, _offset_start, 0 };
	region.dstSubresource = region.srcSubresource;
	region.dstOffset = region.srcOffset;
	region.extent = { InWidth, InHeight, 0 };

	vkCmdCopyImage(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void CommandList::CopyImage(VkImage InSrcImage, VkImage InDstImage, const VkImageCopy& InRegion)
{
	vkCmdCopyImage(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &InRegion);
}

void CommandList::CopyImage(VkImage InSrcImage, VkImage InDstImage, uint32 InRegionCount, const VkImageCopy* InRegions)
{
	vkCmdCopyImage(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, InRegionCount, InRegions);
}

void CommandList::NonUniformImageCopy(VkImage InSrcImage, VkImage InDstImage, const VkImageBlit& InRegion, VkFilter InFilter)
{
	vkCmdBlitImage(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &InRegion, InFilter);
}

void CommandList::NonUniformImageCopy(VkImage InSrcImage, VkImage InDstImage, uint32 InRegionCount, const VkImageBlit* InRegions, VkFilter InFilter)
{
	vkCmdBlitImage(m_cmdBuffer, InSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, InDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, InRegionCount, InRegions, InFilter);
}

void CommandList::ClearColorImage(VkImage InImage, const float* InClearColor)
{
	vkCmdClearColorImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, (VkClearColorValue*)InClearColor, _count_1, &Util::ColorSubresRange);
}

void CommandList::ClearColorImage(VkImage InImage, const VkClearColorValue* InClearColor)
{
	vkCmdClearColorImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, InClearColor, _count_1, &Util::ColorSubresRange);
}

void CommandList::ClearColorImage(VkImage InImage, const VkClearColorValue* InClearColor, const VkImageSubresourceRange& InSubresRange)
{
	vkCmdClearColorImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, InClearColor, _count_1, &InSubresRange);
}

void CommandList::ClearColorImage(VkImage InImage, const VkClearColorValue* InClearColor, uint32 InRangeCount, const VkImageSubresourceRange* InSubresRanges)
{
	vkCmdClearColorImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, InClearColor, InRangeCount, InSubresRanges);
}

void CommandList::ClearDepthStencilImage(VkImage InImage, float InClearDepthValue, uint32 InClearStencilValue)
{
	VkClearDepthStencilValue clearValue = {};
	clearValue.depth = InClearDepthValue;
	clearValue.stencil = InClearStencilValue;

	vkCmdClearDepthStencilImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, &clearValue, _count_1, &Util::DepthStencilSubresRange);
}

void CommandList::ClearDepthStencilImage(VkImage InImage, const VkClearDepthStencilValue* InClearValue)
{
	vkCmdClearDepthStencilImage(m_cmdBuffer, InImage, VK_IMAGE_LAYOUT_GENERAL, InClearValue, _count_1, &Util::DepthStencilSubresRange);
}

void CommandList::BindPipeline(VkPipeline InPipeline, VkPipelineBindPoint InPipBindPoint)
{
	vkCmdBindPipeline(m_cmdBuffer, InPipBindPoint, InPipeline);
}

void CommandList::BindComputePipeline(VkPipeline InPipeline)
{
	vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, InPipeline);
}

void CommandList::BindGraphicPipeline(VkPipeline InPipeline)
{
	vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, InPipeline);
}

void CommandList::Dispatch(uint32 x, uint32 y, uint32 z)
{
	bool bIsOverflow = false;
	bIsOverflow = bIsOverflow || x > m_baseLayer->GetMainPDLimits().maxComputeWorkGroupCount[0];
	bIsOverflow = bIsOverflow || y > m_baseLayer->GetMainPDLimits().maxComputeWorkGroupCount[1];
	bIsOverflow = bIsOverflow || z > m_baseLayer->GetMainPDLimits().maxComputeWorkGroupCount[2];
	_exit_log(bIsOverflow, "Compute Dispatch Goes Beyond The Physical Limits!");
	
	vkCmdDispatch(m_cmdBuffer, x, y, z);
}

void CommandList::DispatchIndirect(VkBuffer InBuffer, VkDeviceSize InOffset)
{
	// Do Something Check...
	// VkDispatchIndirectCommands
	vkCmdDispatchIndirect(m_cmdBuffer, InBuffer, InOffset);
}

void CommandList::BindDescriptorSets(VkPipelineLayout InPipLayout, VkPipelineBindPoint InPipBindPoint, const VkDescriptorSet* InDescSets, uint32 InSetCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/, const uint32* InDynamicOffsets /*= nullptr*/, uint32 InDynamicOffsetCount /*= _count_0*/)
{
	vkCmdBindDescriptorSets(m_cmdBuffer, InPipBindPoint, InPipLayout, InSetOffset, InSetCount, InDescSets, InDynamicOffsetCount, InDynamicOffsets);
}

void CommandList::BindComputeDescSets(VkPipelineLayout InPipLayout, const VkDescriptorSet* InDescSets, uint32 InSetCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/, const uint32* InDynamicOffsets /*= nullptr*/, uint32 InDynamicOffsetCount /*= _count_0*/)
{
	vkCmdBindDescriptorSets(m_cmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, InPipLayout, InSetOffset, InSetCount, InDescSets, InDynamicOffsetCount, InDynamicOffsets);
}

void CommandList::BindGraphicDescSets(VkPipelineLayout InPipLayout, const VkDescriptorSet* InDescSets, uint32 InSetCount /*= _count_1*/, uint32 InSetOffset /*= _offset_0*/, const uint32* InDynamicOffsets /*= nullptr*/, uint32 InDynamicOffsetCount /*= _count_0*/)
{
	vkCmdBindDescriptorSets(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, InPipLayout, InSetOffset, InSetCount, InDescSets, InDynamicOffsetCount, InDynamicOffsets);
}

void CommandList::PushConstants(VkPipelineLayout InPipLayout, VkShaderStageFlags InStageFlags, const void* InValues, uint32 InSize, uint32 InOffset /*= _offset_0*/)
{
	vkCmdPushConstants(m_cmdBuffer, InPipLayout, InStageFlags, InOffset, InSize, InValues);
}

void CommandList::ResourceBarriers(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask,
	/* Memory Barrier. */ 
	uint32 InMemBarrierCount, 
	const VkMemoryBarrier* InMemBarriers, 
	/* Buffer Memory Barrier. */ 
	uint32 InBufferMemBarrierCount, 
	const VkBufferMemoryBarrier* InBufferMemBarriers, 
	/* Image Memory Barrier. */ 
	uint32 InImageMemBarrierCount, 
	const VkImageMemoryBarrier* InImageMemoryBarriers, 
	/* Dependency Flags */ 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		InMemBarrierCount, 
		InMemBarriers, 
		InBufferMemBarrierCount, 
		InBufferMemBarriers, 
		InImageMemBarrierCount, 
		InImageMemoryBarriers);
}

void CommandList::MemoryBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	const VkMemoryBarrier& InMemBarrier, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer,
		InSrcStageMask,
		InDstStageMask,
		InDependencyFlags,
		_count_1,
		&InMemBarrier,
		_count_0, nullptr,
		_count_0, nullptr);
}

void CommandList::MemoryBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	VkAccessFlags InSrcAccessMask, 
	VkAccessFlags InDstAccessMask, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	VkMemoryBarrier memBarrier = {};
	memBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
	memBarrier.srcAccessMask = InSrcStageMask;
	memBarrier.dstAccessMask = InDstStageMask;

	this->MemoryBarrier(InSrcStageMask, InDstStageMask, memBarrier, InDependencyFlags);
}

void CommandList::MemoryBarriers(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	uint32 InMemBarrierCount, 
	const VkMemoryBarrier* InMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		InMemBarrierCount, 
		InMemBarriers, 
		_count_0, nullptr, 
		_count_0, nullptr);
}

void CommandList::BufferBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	const VkBufferMemoryBarrier& InBufferMemBarrier, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		_count_0, nullptr, 
		_count_1, &InBufferMemBarrier, 
		_count_0, nullptr);
}

void CommandList::BufferBarrier(const SBufferBarrier& InSBufferBarrier, VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	VkBufferMemoryBarrier bufferMemBarrier = {};
	bufferMemBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	bufferMemBarrier.srcAccessMask = InSBufferBarrier.srcAccessMask;
	bufferMemBarrier.dstAccessMask = InSBufferBarrier.dstAccessMask;
	bufferMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	bufferMemBarrier.buffer = InSBufferBarrier.buffer;
	bufferMemBarrier.offset = InSBufferBarrier.offset;
	bufferMemBarrier.size = InSBufferBarrier.size;

	this->BufferBarrier(InSBufferBarrier.srcStageMask, InSBufferBarrier.dstStageMask, bufferMemBarrier, InDependencyFlags);
}

void CommandList::BufferBarriers(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	uint32 InBufferMemBarrierCount, 
	const VkBufferMemoryBarrier* InBufferMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		_count_0, nullptr, 
		InBufferMemBarrierCount, 
		InBufferMemBarriers, 
		_count_0, nullptr);
}

void CommandList::ImageBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	const VkImageMemoryBarrier& InImageMemBarrier, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		_count_0, nullptr, 
		_count_0, nullptr, 
		_count_1, &InImageMemBarrier);
}

void CommandList::ImageBarrier(const SImageBarrier& InSImageBarrier, VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	VkImageMemoryBarrier imageMemBarrier = {};
	imageMemBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	imageMemBarrier.srcAccessMask = InSImageBarrier.srcAccessMask;
	imageMemBarrier.dstAccessMask = InSImageBarrier.dstAccessMask;
	imageMemBarrier.oldLayout = InSImageBarrier.oldLayout;
	imageMemBarrier.newLayout = InSImageBarrier.newLayout;
	imageMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemBarrier.image = InSImageBarrier.image;
	imageMemBarrier.subresourceRange = InSImageBarrier.subresourceRange;

	this->ImageBarrier(InSImageBarrier.srcStageMask, InSImageBarrier.dstStageMask, imageMemBarrier, InDependencyFlags);
}

void CommandList::ImageBarriers(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	uint32 InImageMemBarrierCount, 
	const VkImageMemoryBarrier* InImageMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= _flag_none*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		_count_0, nullptr, 
		_count_0, nullptr, 
		InImageMemBarrierCount, 
		InImageMemBarriers);
}
