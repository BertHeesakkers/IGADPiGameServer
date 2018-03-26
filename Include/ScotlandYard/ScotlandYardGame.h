#pragma once

#include "Network/EPlayer.h"
#include "ETravelResult.h"
#include "Map.h"
#include "Edge.h"
#include "Node.h"

#include <string>
#include <vector>

class Player;
class Ply;
class Spy;
class TravelLog;

class ScotlandYardGame
{
public:
	ScotlandYardGame();
	virtual ~ScotlandYardGame();

	void AddPlayer(EPlayer a_Player);
	const Player& GetPlayer(EPlayer a_Player) const;
	bool IsSpy(EPlayer a_Player);
	ETravelResult Travel(EPlayer a_Player, uint32_t a_Destination, ETravelOption a_TravelOption);

	bool IsActive() const;
	EPlayer WhoseTurnIsIt() const;
	const TravelLog GetSpyTravelLog() const;

	uint32_t GetNumPlayers() const;
	static uint32_t GetNumPlayersPerGame();

private:
	static const std::string& GetMapFilename();

	void Start();
	void LoadMap(const std::string &a_Filename);
	Player* FindPlayer(EPlayer a_Player);
	Player& GetPlayer(EPlayer a_Player);
	Spy* FindSpy();
	const Spy* FindSpy() const;
	Spy& GetSpy();
	const Spy& GetSpy() const;

	bool m_IsActive = false;
	uint32_t m_NumPlayers;
	Map m_Map;
	bool m_GameOver = false;
	Ply *m_Ply;

	std::vector<Player*> m_Players;

	friend class ScotlandYardServerGame;
};
