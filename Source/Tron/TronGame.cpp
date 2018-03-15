#include "TronGame.h"

TronGame::TronGame(ILogger &a_Logger)
	: m_Logger(a_Logger)
{
}

TronGame::~TronGame()
{
}

uint32_t TronGame::GetNumPlayersPerGame()
{
	return 4;
}
