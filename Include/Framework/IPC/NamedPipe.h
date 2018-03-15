#pragma once

#include "Pipe.h"

#include <string>

#if defined(ENVIRONMENT_WINDOWS)

#include <windows.h>

struct PipeCreateParameters;

class NamedPipe : public Pipe
{
public:
	~NamedPipe();

	bool Connect() override;

private:
	NamedPipe(HANDLE a_PipeHandle, const PipeCreateParameters &a_Parameters);
	
	friend NamedPipe* ConstructNamedPipe(const PipeCreateParameters&);
	friend NamedPipe* ConstructNamedPipeConnection(const PipeCreateParameters&);
};

NamedPipe* ConstructNamedPipe(const PipeCreateParameters &a_Parameters);
void NamedPipeWrite(const std::string &a_PipeName, const std::string &a_Data);

#endif
