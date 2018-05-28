#include "gamemaster.h"

#include "mainlevel.h"

GameMaster* GameMaster::s_instance = nullptr;

GameMaster::~GameMaster()
{
	for (auto it = m_mapLevels.begin(); it != m_mapLevels.end(); it++)
	{
		delete it->second;
	}
}

GameMaster& GameMaster::Instance()
{
	if (!s_instance)
		s_instance = new GameMaster();

	return (*s_instance);
}

void GameMaster::Destroy()
{
	//Nothing to destroy
	if (!s_instance)
		return;

	delete s_instance;
	
	s_instance = nullptr;
}

void GameMaster::Initialize()
{
	//Create all levels here
	//First level put into level map will be start level
	//Entities and UI elements are Initialized here

	//===== Main Scene ======
	Level* mainMenu = new MainLevel();
	m_mapLevels.insert(std::pair<std::string, Level*>("MainMenu", mainMenu));


	//Initialize all levels now
	for (auto it = m_mapLevels.begin(); it != m_mapLevels.end(); it++)
	{
		it->second->Initialize();
	}

	m_pCurrentLevel = m_mapLevels.begin()->second;
}

void GameMaster::ChangeLevel(std::string _levelName)
{

}

void GameMaster::Update()
{
	m_pCurrentLevel->Update();
}

void GameMaster::Render()
{
	m_pCurrentLevel->Render();
}

GameMaster::GameMaster()
{
	m_pCurrentLevel = nullptr;

	//Make randomiser work
	srand((unsigned int)time(NULL));
}
