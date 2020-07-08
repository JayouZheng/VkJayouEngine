//
// CommandQueue.cpp
//

#include "CommandQueue.h"

CommandQueue::CommandQueue(VkQueue InQueue)
{
	m_queue = InQueue;
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
