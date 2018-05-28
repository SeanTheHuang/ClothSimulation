#pragma once

#include "include.h"
#include "level.h"

#include <map>

//Entry point to game
//Controls transitions between levels
//GameMaster is a [Singleton class]
class GameMaster
{
//== Methods
public:
	~GameMaster();
	static GameMaster& Instance(); //Singleton stuff
	//Clear Memory. Calls Destroy on all Level objects
	static void Destroy();
	//Create & Initialize all the levels
	//First level in map will be the default map
	void Initialize();
	//Calls update on current Level object
	void Update();
	//Calls Render on current Level object
	void Render();
	//Sets current level to specified level
	void ChangeLevel(std::string _levelName);

private:
	GameMaster(); //Singleton stuff
	GameMaster(const GameMaster&) = delete;
	void operator=(GameMaster const&) = delete;


//== Members
private:
	static GameMaster* s_instance;
	std::map<std::string, Level*> m_mapLevels;
	Level* m_pCurrentLevel;
};