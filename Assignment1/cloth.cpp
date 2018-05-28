#include "cloth.h"

Cloth::Cloth(Camera * _camera, glm::vec3 _position)
{
	m_pCamera = _camera;
	m_position = _position;

	Initialize();
}

Cloth::~Cloth()
{
}

void Cloth::Initialize()
{
	// Start creating cloth nodes


	// Set up opengl stuffs here
}

void Cloth::Render()
{
	// Draw cloth on both sides
	glDisable(GL_CULL_FACE);



	glEnable(GL_CULL_FACE);
}

void Cloth::Update()
{
}
