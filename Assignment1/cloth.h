#pragma once

#include "include.h"
#include "camera.h"
#include <vector>

const int CLOTH_SIZE = 40;
const float CLOTH_WIDTH = 5;
const float CLOTH_HEIGHT = 3;

struct ClothNode
{
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	glm::vec2 m_uv;
	bool m_canMove;
	int m_X, m_Y;

	void Initialize(glm::vec3 _position, glm::vec2 _uv, int _X, int _Y)
	{
		m_position = _position;
		m_uv = _uv;
		m_velocity = glm::vec3(0);
		m_acceleration = glm::vec3(0);
		m_canMove = true;
		m_X = _X;
		m_Y = _Y;
	}
};

struct DrawData
{
	glm::vec3 m_position;
	glm::vec2 m_uv;

	DrawData(glm::vec3 _position, glm::vec2 _uv)
	{
		m_position = _position;
		m_uv = _uv;
	}
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
	void SetUpRenderData();

	std::vector<DrawData> m_drawDataList;
	ClothNode m_clothNodes[CLOTH_SIZE][CLOTH_SIZE];
	glm::vec3 m_position;
	Camera* m_pCamera;

	GLuint m_program, m_vbo, m_vao;
};