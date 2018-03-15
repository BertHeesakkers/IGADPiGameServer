#pragma once

#include "PipeCreateParameters.h"
#include "../NoCopy.h"

#include <string>

#if defined(ENVIRONMENT_WINDOWS)
#include <windows.h>

struct PipeCreateParameters;

class Pipe : public NoCopy
{
public:
	Pipe(HANDLE a_Handle, const PipeCreateParameters &a_Parameters);

	bool IsAnonymous() const;
	virtual bool Connect() = 0;

	HANDLE GetHandle() const;
	void Disconnect();

	uint32_t GetInputBufferByteSize() const;
	uint32_t GetOutputBufferByteSize() const;

protected:
	const PipeCreateParameters m_Parameters;
	std::wstring m_FullName;
	HANDLE m_Handle = nullptr;
};

#endif
