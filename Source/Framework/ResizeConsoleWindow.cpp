#include "ResizeConsoleWindow.h"

#include "AssertMessage.h"
#include "Math/Min.h"

void ResizeConsoleWindow(short xSize, short ySize)
{
#if defined(WIN32)
	HANDLE stdOutputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	BOOL success = ::GetConsoleScreenBufferInfo(stdOutputHandle, &consoleScreenBufferInfo);
	COORD screenSize = GetLargestConsoleWindowSize(stdOutputHandle);

	SMALL_RECT windowRect;
	windowRect.Right = static_cast<SHORT>(Min(xSize, screenSize.X) - 1);
	windowRect.Bottom = static_cast<SHORT>(Min(ySize, screenSize.Y) - 1);
	windowRect.Left = windowRect.Top = static_cast<SHORT>(0);

	screenSize.X = xSize;
	screenSize.Y = ySize;

	if (static_cast<uint32_t>(consoleScreenBufferInfo.dwSize.X) * static_cast<uint32_t>(consoleScreenBufferInfo.dwSize.Y) >
		static_cast<uint32_t>(xSize) * static_cast<uint32_t>(ySize))
	{
		success = SetConsoleWindowInfo(stdOutputHandle, TRUE, &windowRect);
		AssertMessage(TRUE == success, "Failed to set console window info!");
		success = SetConsoleScreenBufferSize(stdOutputHandle, screenSize);
		AssertMessage(TRUE == success, "Failed to set console screen buffer size!");
	}

	if (static_cast<uint32_t>(consoleScreenBufferInfo.dwSize.X) * static_cast<uint32_t>(consoleScreenBufferInfo.dwSize.Y) <
		static_cast<uint32_t>(xSize) * static_cast<uint32_t>(ySize))
	{
		success = SetConsoleScreenBufferSize(stdOutputHandle, screenSize);
		AssertMessage(TRUE == success, "Failed to set console screen buffer size!");
		success = SetConsoleWindowInfo(stdOutputHandle, TRUE, &windowRect);
		AssertMessage(TRUE == success, "Failed to set console window info!");
	}
#endif
}
