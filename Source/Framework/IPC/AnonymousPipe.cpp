#include "AnonymousPipe.h"

#if defined(ENVIRONMENT_WINDOWS)

AnonymousPipe* ConstructAnonymousPipe(const PipeCreateParameters&)
{
	return nullptr;
}

AnonymousPipe* ConstructAnonymousPipeConnection(const PipeCreateParameters&)
{
	return nullptr;
}

AnonymousPipe::AnonymousPipe(HANDLE a_PipeHandle, const PipeCreateParameters &a_Parameters)
	: Pipe(a_PipeHandle, a_Parameters)
{
}

#endif