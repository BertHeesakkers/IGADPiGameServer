#include "ServerAppMenus.h"

#if defined(USE_CURSES)

#include "ServerApp.h"

#include <locale.h>

namespace
{
	static menu mainMenu[] =
	{
		{ "File", &ServerAppMenus::FileMenuOption, "File and application functions" },
		{ "Server", &ServerAppMenus::ServerMenuOption, "Server functions" },
		{ "Games", &ServerAppMenus::GameMenuOption, "Game functions" },
		{ "Players", &ServerAppMenus::PlayerMenuOption, "Player functions" },
		{ "", (FUNC)0, "" }   /* always add this as the last item! */
	};

	static menu fileSubMenu[] =
	{
		{ "Clear", clsbody, "Clear screen" },
		{ "Exit", DoExit, "Exit" },
		{ "", (FUNC)0, "" }
	};

	static menu serverSubMenu[] =
	{
		{ "StartServer", &ServerAppMenus::StartServer, "Start server" },
		{ "StopServer", &ServerAppMenus::StopServer, "Stop server" },
		{ "", (FUNC)0, "" }
	};

	static menu gamesSubMenu[] =
	{
		{ "Scotland Yard", &ServerAppMenus::ScotlandYardMenuOption, "Interactive file lister" },
		{ "Sokoban", &ServerAppMenus::SokobanMenuOption, "Get address data" },
		{ "Tron", &ServerAppMenus::TronMenuOption, "Get address data" },
		{ "", (FUNC)0, "" }
	};

	static menu playerSubMenu[] =
	{
		{ "", (FUNC)0, "" }
	};
}

ServerApp *ServerAppMenus::m_Application = nullptr;

void ServerAppMenus::ShowMainMenu()
{
	setlocale(LC_ALL, "");
	startmenu(mainMenu, "Network games server");
}

void ServerAppMenus::FileMenuOption(void)
{
	domenu(fileSubMenu);
}

void ServerAppMenus::ServerMenuOption(void)
{
	domenu(serverSubMenu);
}

void ServerAppMenus::GameMenuOption(void)
{
	domenu(gamesSubMenu);
}

void ServerAppMenus::PlayerMenuOption(void)
{
	domenu(playerSubMenu);
}

void ServerAppMenus::StartServer(void)
{
	m_Application->StartServer();
}

void ServerAppMenus::StopServer(void)
{
	m_Application->StopServer();
}

void ServerAppMenus::ScotlandYardMenuOption(void)
{
}

void ServerAppMenus::SokobanMenuOption(void)
{
}

void ServerAppMenus::TronMenuOption(void)
{
}

#endif
