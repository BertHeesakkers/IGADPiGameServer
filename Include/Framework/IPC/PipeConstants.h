#pragma once

#include <stdint.h>

#if defined(ENVIRONMENT_WINDOWS)

#include <windows.h>

const uint32_t g_PipeMaxNumInstances = PIPE_UNLIMITED_INSTANCES;

enum
{
	PipeOpenMode_AccessDuplex = PIPE_ACCESS_DUPLEX,
	PipeOpenMode_AccessInbound = PIPE_ACCESS_INBOUND,
	PipeOpenMode_AccessOutbound = PIPE_ACCESS_OUTBOUND,
	
	PipeOpenMode_FileFirstPipeInstance = FILE_FLAG_FIRST_PIPE_INSTANCE,
	PipeOpenMode_FileFlagWriteThrough = FILE_FLAG_WRITE_THROUGH,
	PipeOpenMode_FileFlagOverlapped = FILE_FLAG_OVERLAPPED,

	PipeOpenMode_WriteDAC = WRITE_DAC,
	PipeOpenMode_WriteOwner = WRITE_OWNER,
	PipeOpenMode_AccessSystemSecurity = ACCESS_SYSTEM_SECURITY,

	PipeMode_Byte = PIPE_TYPE_BYTE,
	PipeMode_Message = PIPE_TYPE_MESSAGE,

	PipeMode_ReadModeByte = PIPE_READMODE_BYTE,
	PipeMode_ReadModeMessage = PIPE_READMODE_MESSAGE,
	
	PipeMode_Wait = PIPE_WAIT,
	PipeMode_NoWait = PIPE_NOWAIT
};

#endif
