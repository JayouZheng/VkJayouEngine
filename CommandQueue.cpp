//
// CommandQueue.cpp
//

#include "CommandQueue.h"

CommandQueue::CommandQueue(const VkQueue& InQueue)
	: m_queue(InQueue)
{
	
}

CommandQueue& CommandQueue::operator=(const VkQueue& InQueue)
{
	m_queue = InQueue;
	return *this;
}

CommandQueue::operator VkQueue()
{
	return m_queue;
}

CommandQueue::operator VkQueue*()
{
	return &m_queue;
}

bool CommandQueue::operator==(const VkQueue& InQueue) const
{
	return m_queue == InQueue;
}

void CommandQueue::Execute(const CommandList& InCmdList)
{
	VkCommandBuffer cmdBuffer = InCmdList.GetCmdBuffer();

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = _count_1;
	submitInfo.pCommandBuffers = &cmdBuffer;

	_vk_try(vkQueueSubmit(m_queue, _count_1, &submitInfo, VK_NULL_HANDLE));
}

void CommandQueue::Flush()
{
	_vk_try(vkQueueWaitIdle(m_queue));
}

void CommandQueue::Present(const VkPresentInfoKHR& InPresentInfo)
{
	_vk_try(vkQueuePresentKHR(m_queue, &InPresentInfo));
}
