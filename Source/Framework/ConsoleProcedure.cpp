#include "ConsoleProcedure.h"

#include <assert.h>

namespace
{
#if defined(ENVIRONMENT_WINDOWS)
	ConsoleEventCallback gs_ConsoleCopyEvent = 0;
	ConsoleEventCallback gs_ConsoleBreakEvent = 0;
	ConsoleEventCallback gs_ConsoleCloseEvent = 0;
	ConsoleEventCallback gs_ConsoleLogOffEvent = 0;
	ConsoleEventCallback gs_ConsoleShutDownEvent = 0;
#endif
}

#if defined(ENVIRONMENT_WINDOWS)
BOOL WINAPI ConsoleProcedure(DWORD a_Signal)
{
	switch (a_Signal)
	{
		case CTRL_C_EVENT:
		{	
			if (0 != gs_ConsoleCopyEvent)
			{
				gs_ConsoleCopyEvent();
			}
			break;
		}
		case CTRL_BREAK_EVENT:
		{	
			if (0 != gs_ConsoleBreakEvent)
			{
				gs_ConsoleBreakEvent();
			}
			break;
		}	
		case CTRL_CLOSE_EVENT:
		{	
			if (0 != gs_ConsoleCloseEvent)
			{
				gs_ConsoleCloseEvent();
			}
			break;
		}
		case CTRL_LOGOFF_EVENT:
		{
			if (0 != gs_ConsoleLogOffEvent)
			{
				gs_ConsoleLogOffEvent();
			}
			break;
		}
		case CTRL_SHUTDOWN_EVENT:
		{
			if (0 != gs_ConsoleShutDownEvent)
			{
				gs_ConsoleShutDownEvent();
			}
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}

	return TRUE;
}
#endif

void EnableConsoleEventCallbacks()
{
#if defined(ENVIRONMENT_WINDOWS)
	const bool success = ::SetConsoleCtrlHandler(ConsoleProcedure, TRUE) == TRUE;
	assert(success);
#endif
}

void SetConsoleCopyEvent(ConsoleEventCallback a_ConsoleCopyEvent)
{
#if defined(ENVIRONMENT_WINDOWS)
	gs_ConsoleCloseEvent = a_ConsoleCopyEvent;
#endif
}

void SetConsoleBreakEvent(ConsoleEventCallback a_ConsoleBreakEvent)
{
#if defined(ENVIRONMENT_WINDOWS)
	gs_ConsoleBreakEvent = a_ConsoleBreakEvent;
#endif
}

void SetConsoleCloseEvent(ConsoleEventCallback a_ConsoleCloseEvent)
{
#if defined(ENVIRONMENT_WINDOWS)
	gs_ConsoleCloseEvent = a_ConsoleCloseEvent;
#endif
}

void SetConsoleLogOffEvent(ConsoleEventCallback a_ConsoleLogOffEvent)
{
#if defined(ENVIRONMENT_WINDOWS)
	gs_ConsoleLogOffEvent = a_ConsoleLogOffEvent;
#endif
}

void SetConsoleShutDownEvent(ConsoleEventCallback a_ConsoleShutDownEvent)
{
#if defined(ENVIRONMENT_WINDOWS)
	gs_ConsoleShutDownEvent = a_ConsoleShutDownEvent;
#endif
}
