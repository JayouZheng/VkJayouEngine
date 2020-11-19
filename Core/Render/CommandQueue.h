//
// CommandQueue.h
//

#pragma once

#include "Core/Common.h"

class CommandList;

class CommandQueue : public IResourceHandler
{
	_declare_create_interface(CommandQueue)

protected:

	VkQueue m_queue;

	CommandQueue();

public:
	
	virtual ~CommandQueue();
	CommandQueue& operator=(const VkQueue& InQueue);

public:

	operator VkQueue();
	operator VkQueue*();

	bool operator==(const VkQueue& InDevice) const;

public:

	void Execute(const CommandList* InCmdList);
	void Flush();

	// It needs to be supplemented...
	void Present(const VkPresentInfoKHR& InPresentInfo);
};