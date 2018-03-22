#pragma once

#if defined(WIN32)
#include <windows.h>
#endif

void EnableConsoleEventCallbacks();

typedef void(*ConsoleEventCallback)();

void SetConsoleCopyEvent(ConsoleEventCallback a_ConsoleCopyEvent);
void SetConsoleBreakEvent(ConsoleEventCallback a_ConsoleBreakEvent);
void SetConsoleCloseEvent(ConsoleEventCallback a_ConsoleCloseEvent);
void SetConsoleLogOffEvent(ConsoleEventCallback a_ConsoleLogOffEvent);
void SetConsoleShutDownEvent(ConsoleEventCallback a_ConsoleShutDownEvent);
