#pragma once

#include "Network/EPlayer.h"

#include <stdint.h>
#include <vector>

class Ply
{
public:
	Ply(uint32_t a_NumPlayers);
	Ply(const std::vector<EPlayer> &a_Players);
	~Ply();

	EPlayer GetCurrentPlayer() const;
	EPlayer GetNextPlayer() const;
	void FinishedTurn(EPlayer a_Player);
	uint32_t GetCurrentTurn() const;
	void DisablePlayer(EPlayer a_Player);
	uint32_t GetNumberOfDisabledPlayers();

	void Reset();

private:
	uint32_t m_CurrentPlayer = 0;
	std::vector<EPlayer> m_Players;
	std::vector<EPlayer> m_DisabledPlayers;
	mutable uint32_t m_Turn = 0;
};