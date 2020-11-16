//
// CommandQueue.h
//

#pragma once

#include "Core/Render/CommandList.h"

class CommandQueue
{

protected:

	VkQueue m_queue = VK_NULL_HANDLE;

public:

	CommandQueue() {}
	CommandQueue(const VkQueue& InQueue);
	CommandQueue& operator=(const VkQueue& InQueue);
	virtual ~CommandQueue() {};

public:

	operator VkQueue();
	operator VkQueue*();

	bool operator==(const VkQueue& InDevice) const;

public:

	void Execute(const CommandList& InCmdList);
	void Flush();

	// It needs to be supplemented...
	void Present(const VkPresentInfoKHR& InPresentInfo);
};