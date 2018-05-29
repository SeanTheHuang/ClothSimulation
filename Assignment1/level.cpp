#include "level.h"

Level::Level()
{
	
}

Level::~Level()
{
	CleanUp();
}

void Level::Update()
{
	//Update cube
	for (size_t i = 0; i < m_vecCubes.size(); i++)
	{
		m_vecCubes[i]->Update();
	}

	//Update Entities
	for (size_t i = 0; i < m_vecEntity.size(); i++)
	{
		m_vecEntity[i]->Update();
	}

	//Update Quads
	for (size_t i = 0; i < m_vecQuads.size(); i++)
	{
		m_vecQuads[i]->Update();
	}

	if (m_pCloth)
		m_pCloth->Update();
}

void Level::Render()
{
	//Update cube
	for (size_t i = 0; i < m_vecCubes.size(); i++)
	{
		m_vecCubes[i]->Render();
	}

	//Render Entities
	for (size_t i = 0; i < m_vecEntity.size(); i++)
	{
		m_vecEntity[i]->Render();
	}

	//Render transparent quads
	for (size_t i = 0; i < m_vecQuads.size(); i++)
	{
		m_vecQuads[i]->Render();
	}

	if (m_pCloth)
		m_pCloth->Render();
}

void Level::CleanUp()
{
	if (m_pCamera)
		delete m_pCamera;

	//Delete Entities
	for (size_t i = 0; i < m_vecEntity.size(); i++)
	{
		delete m_vecEntity[i];
	}
	m_vecEntity.clear();

	//Delete Quads
	for (size_t i = 0; i < m_vecQuads.size(); i++)
	{
		delete m_vecQuads[i];
	}
	m_vecQuads.clear();

	//Delete Cubes
	for (size_t i = 0; i < m_vecCubes.size(); i++)
	{
		delete m_vecCubes[i];
	}
	m_vecCubes.clear();

	if (m_pCloth)
		delete m_pCloth;
	m_pCloth = nullptr;

}

void Level::AddEntity(Entity* _newEntity)
{
	m_vecEntity.push_back(_newEntity);
}

void Level::AddQuad(Quad* _newQuad)
{
	m_vecQuads.push_back(_newQuad);
}

void Level::AddCube(Cube * _newCube)
{
	m_vecCubes.push_back(_newCube);
}

void Level::SetCloth(Cloth * _cloth)
{
	m_pCloth = _cloth;
}
