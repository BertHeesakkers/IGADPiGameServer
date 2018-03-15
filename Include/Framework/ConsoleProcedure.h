#pragma once

#if defined(ENVIRONMENT_WINDOWS)
#include <windows.h>
#endif

void EnableConsoleEventCallbacks();

typedef void(*ConsoleEventCallback)();

void SetConsoleCopyEvent(ConsoleEventCallback a_ConsoleCopyEvent);
void SetConsoleBreakEvent(ConsoleEventCallback a_ConsoleBreakEvent);
void SetConsoleCloseEvent(ConsoleEventCallback a_ConsoleCloseEvent);
void SetConsoleLogOffEvent(ConsoleEventCallback a_ConsoleLogOffEvent);
void SetConsoleShutDownEvent(ConsoleEventCallback a_ConsoleShutDownEvent);
