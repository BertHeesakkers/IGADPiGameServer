#include "Ply.h"

#include "Framework/AssertMessage.h"
#include <algorithm>

Ply::Ply(uint32_t a_NumPlayers)
{
}

Ply::Ply(const std::vector<EPlayer> &a_Players)
	: m_Players(a_Players)
{
}

Ply::~Ply()
{
}

void Ply::Reset()
{
	m_CurrentPlayer = 0;
	m_Turn = 0;
	m_DisabledPlayers.clear();
}

EPlayer Ply::GetCurrentPlayer() const
{
	return m_Players[m_CurrentPlayer];
}

EPlayer Ply::GetNextPlayer() const
{
	 uint32_t nextPlayerIndex = m_CurrentPlayer + 1;
	if (nextPlayerIndex >= static_cast<uint32_t>(m_Players.size()))
	{
		nextPlayerIndex = 0;
	}
	return m_Players[nextPlayerIndex];
}

void Ply::FinishedTurn(EPlayer a_Player)
{
	AssertMessage(GetCurrentPlayer() == a_Player, "Not this player's turn!");
	const uint32_t numPlayers = static_cast<uint32_t>(m_Players.size());
	if (++m_CurrentPlayer >= numPlayers)
	{
		m_CurrentPlayer = 0;
		m_Turn++;
	}
	if (std::find(m_DisabledPlayers.begin(), m_DisabledPlayers.end(), GetCurrentPlayer()) != m_DisabledPlayers.end())
		FinishedTurn(GetCurrentPlayer());
}

uint32_t Ply::GetCurrentTurn() const
{
	return m_Turn;
}

void Ply::DisablePlayer(EPlayer a_Player)
{
	m_DisabledPlayers.push_back(a_Player);
}

uint32_t Ply::GetNumberOfDisabledPlayers()
{
	return m_DisabledPlayers.size();
}

