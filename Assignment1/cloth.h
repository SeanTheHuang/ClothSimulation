#pragma once

#include "include.h"
#include "camera.h"

const int CLOTH_SIZE = 40;

struct ClothNode
{
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	bool m_canMove;
};

class Cloth
{
public:
	Cloth(Camera* _camera, glm::vec3 _position);
	~Cloth();
	void Initialize();
	void Render();
	void Update();

private:
	ClothNode m_clothNodes[CLOTH_SIZE][CLOTH_SIZE];
	glm::vec3 m_position;
	Camera* m_pCamera;

	GLuint m_program, m_vbo, m_vao;
};