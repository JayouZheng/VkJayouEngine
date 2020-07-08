﻿//
// CommandList.cpp
//

#include "CommandList.h"

CommandList::CommandList(VkDevice InDevice, VkCommandPool InCmdPool)
{
	// Single Primary Command Buffer.
	VkCommandBufferAllocateInfo cmdBufferAllocInfo = {};
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.pNext = nullptr;
	cmdBufferAllocInfo.commandPool = InCmdPool;
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocInfo.commandBufferCount = 1;

	_vk_try(vkAllocateCommandBuffers(InDevice, &cmdBufferAllocInfo, &m_cmdBuffer));

	m_device  = InDevice;
	m_cmdPool = InCmdPool;
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
	vkFreeCommandBuffers(m_device, m_cmdPool, 1, &m_cmdBuffer);
}

void CommandList::Close()
{
	_vk_try(vkEndCommandBuffer(m_cmdBuffer));
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer)
{
	VkBufferCopy copyDesc = {};
	copyDesc.srcOffset = 0;
	copyDesc.dstOffset = 0;
	copyDesc.size = VK_WHOLE_SIZE;

	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, 1, &copyDesc);
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer, const VkBufferCopy& InRegion)
{
	_exit_log(InRegion.srcOffset % 4 != 0, "CopyBuffer, SrcOffset is not a multiple of 4!");
	_exit_log(InRegion.dstOffset % 4 != 0, "CopyBuffer, DstOffset is not a multiple of 4!");
	_exit_log(InRegion.size % 4 != 0, "CopyBuffer, Size is not a multiple of 4!");

	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, 1, &InRegion);
}

void CommandList::CopyBuffer(VkBuffer InSrcBuffer, VkBuffer InDstBuffer, uint32_t InRegionCount, const VkBufferCopy* InRegions)
{
	vkCmdCopyBuffer(m_cmdBuffer, InSrcBuffer, InDstBuffer, InRegionCount, InRegions);
}

void CommandList::ClearBufferUint32(VkBuffer InBuffer, const uint32_t InValue)
{
	vkCmdFillBuffer(m_cmdBuffer, InBuffer, 0, VK_WHOLE_SIZE, InValue);
}

void CommandList::ClearBufferFloat(VkBuffer InBuffer, const float InValue)
{
	vkCmdFillBuffer(m_cmdBuffer, InBuffer, 0, VK_WHOLE_SIZE, *(const uint32_t*)&InValue);
}

void CommandList::ClearBufferUint32(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const uint32_t InValue)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferUint32, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferUint32, Size is not a multiple of 4!");

	vkCmdFillBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, InValue);
}

void CommandList::ClearBufferFloat(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const float InValue)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferFloat, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferFloat, Size is not a multiple of 4!");

	vkCmdFillBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, *(const uint32_t*)&InValue);
}

void CommandList::UpdateBuffer(VkBuffer InBuffer, VkDeviceSize InOffset, VkDeviceSize InSize, const void* InData)
{
	_exit_log(InOffset % 4 != 0, "ClearBufferFloat, Offset is not a multiple of 4!");
	_exit_log(InSize % 4 != 0, "ClearBufferFloat, Size is not a multiple of 4!");
	_exit_log(InSize > 65536u, "ClearBufferFloat, The maximum size of data that can be placed in a buffer with vkCmdUpdateBuffer() is 65,536 bytes!");

	vkCmdUpdateBuffer(m_cmdBuffer, InBuffer, InOffset, InSize, InData);
}

void CommandList::ResourceBarriers(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask,
	/* Memory Barrier. */ 
	uint32_t InMemBarrierCount, 
	const VkMemoryBarrier* InMemBarriers, 
	/* Buffer Memory Barrier. */ 
	uint32_t InBufferMemBarrierCount, 
	const VkBufferMemoryBarrier* InBufferMemBarriers, 
	/* Image Memory Barrier. */ 
	uint32_t InImageMemBarrierCount, 
	const VkImageMemoryBarrier* InImageMemoryBarriers, 
	/* Dependency Flags */ 
	VkDependencyFlags InDependencyFlags /*= 0*/)
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
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer,
		InSrcStageMask,
		InDstStageMask,
		InDependencyFlags,
		1,
		&InMemBarrier,
		0, nullptr,
		0, nullptr);
}

void CommandList::MemoryBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	VkAccessFlags InSrcAccessMask, 
	VkAccessFlags InDstAccessMask, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
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
	uint32_t InMemBarrierCount, 
	const VkMemoryBarrier* InMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		InMemBarrierCount, 
		InMemBarriers, 
		0, nullptr, 
		0, nullptr);
}

void CommandList::BufferBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	const VkBufferMemoryBarrier& InBufferMemBarrier, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		0, nullptr, 
		1, &InBufferMemBarrier, 
		0, nullptr);
}

void CommandList::BufferBarrier(const SBufferBarrier& InSBufferBarrier, VkDependencyFlags InDependencyFlags /*= 0*/)
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
	uint32_t InBufferMemBarrierCount, 
	const VkBufferMemoryBarrier* InBufferMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		0, nullptr, 
		InBufferMemBarrierCount, 
		InBufferMemBarriers, 
		0, nullptr);
}

void CommandList::ImageBarrier(
	VkPipelineStageFlags InSrcStageMask, 
	VkPipelineStageFlags InDstStageMask, 
	const VkImageMemoryBarrier& InImageMemBarrier, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		0, nullptr, 
		0, nullptr, 
		1, &InImageMemBarrier);
}

void CommandList::ImageBarrier(const SImageBarrier& InSImageBarrier, VkDependencyFlags InDependencyFlags /*= 0*/)
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
	uint32_t InImageMemBarrierCount, 
	const VkImageMemoryBarrier* InImageMemBarriers, 
	VkDependencyFlags InDependencyFlags /*= 0*/)
{
	vkCmdPipelineBarrier(
		m_cmdBuffer, 
		InSrcStageMask, 
		InDstStageMask, 
		InDependencyFlags, 
		0, nullptr, 
		0, nullptr, 
		InImageMemBarrierCount, 
		InImageMemBarriers);
}