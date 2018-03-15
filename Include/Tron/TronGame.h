#pragma once

#include <stdint.h>

class ILogger;

class TronGame
{
public:
	TronGame(ILogger &a_Logger);
	~TronGame();

	static uint32_t GetNumPlayersPerGame();

private:
	ILogger &m_Logger;
};
