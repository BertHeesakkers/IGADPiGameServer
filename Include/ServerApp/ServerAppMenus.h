#pragma once

#if defined(USE_CURSES)

#include "ThirdParty/tui.h"

class ServerApp;

class ServerAppMenus
{
public:
	void ShowMainMenu();

	static void FileMenuOption(void);
	static void ServerMenuOption(void);
	static void GameMenuOption(void);
	static void PlayerMenuOption(void);

	static void StartServer(void);
	static void StopServer(void);

	static void ScotlandYardMenuOption(void);
	static void SokobanMenuOption(void);
	static void TronMenuOption(void);

private:
	static ServerApp *m_Application;

	friend class ServerApp;
};

#endif