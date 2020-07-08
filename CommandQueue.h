//
// CommandQueue.h
//

#pragma once

#include "CommandList.h"

class CommandQueue
{

protected:

	VkQueue m_queue = VK_NULL_HANDLE;

public:

	CommandQueue(VkQueue InQueue);
	virtual ~CommandQueue() {};

public:

	void Execute(const CommandList& InCmdList);
	void Flush();
};