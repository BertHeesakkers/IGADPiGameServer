#pragma once

#include "Pipe.h"

#if defined(WIN32)

#include <windows.h>

struct PipeCreateParameters;

class AnonymousPipe : public Pipe
{
public:
	AnonymousPipe(HANDLE a_PipeHandle, const PipeCreateParameters &a_Parameters);

	bool Connect() override { return false; }
};

#endif
