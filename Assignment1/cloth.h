#pragma once

#include "include.h"
#include "camera.h"
#include <vector>

// Cloth size, and how many particles in the cloth (CLOTH_LOD * CLOTH_LOD)
const int CLOTH_LOD = 20;
const float CLOTH_SIZE = 5;

// Strength of the joints in a cloth
const float REST_JOINT_DIST = 0.26f * (20.0f / CLOTH_LOD);
const float STIFFNESS = 1.0f;

const float DAMPING = 0.2f; // Affects how far particles move towards eachother (0 - small, 1 - large)

struct ClothNode
{
	glm::vec3 m_position;
	glm::vec3 m_oldPosition;
	glm::vec3 m_acceleration;
	glm::vec2 m_uv;
	bool m_canMove;
	int m_X, m_Y;

	void Initialize(glm::vec3 _position, glm::vec2 _uv, int _X, int _Y)
	{
		m_position = _position;
		m_oldPosition = _position;
		m_uv = _uv;
		m_acceleration = glm::vec3(0);
		m_canMove = true;
		m_X = _X;
		m_Y = _Y;
	}

	void OffsetPosition(glm::vec3 _newPos) 
	{ 
		m_position += _newPos; 
	}

	void AddAcceleration(glm::vec3 _newAccel) { m_acceleration += _newAccel; }
	void Update()
	{
		// Cannot move, don't do movement calcuations
		if (!m_canMove)
			return;

		// Verlet Integration
		float accelSquared = Time::Instance().DeltaTime() * Time::Instance().DeltaTime();
		glm::vec3 velocity = m_position - m_oldPosition;
		glm::vec3 temp = glm::vec3(m_position.x + velocity.x * (1 - DAMPING)
								, m_position.y + velocity.y * (1 - DAMPING)
								, m_position.z + velocity.z * (1 - DAMPING));

		glm::vec3 nextPosition = temp + m_acceleration * accelSquared;
		
		m_oldPosition = m_position;
		m_position = nextPosition;

		// Reset acceleration for this frame
		m_acceleration = glm::vec3(0);

		// Random floor "Collision detection"
		if (m_position.y < -4)
			m_position.y = -4;
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

struct ClothJoint
{
	ClothNode *m_body1, *m_body2;

	ClothJoint(ClothNode* _body1, ClothNode* _body2)
	{
		m_body1 = _body1;
		m_body2 = _body2;
	}

	// A joint has both bodies
	void Update()
	{
		// Both bodies can't move
		if (!m_body1->m_canMove && !m_body2->m_canMove)
			return;

		glm::vec3 diff = m_body1->m_position - m_body2->m_position;
		float distance = sqrtf(glm::dot(diff, diff));
		float difference = (REST_JOINT_DIST - distance) / distance;
		float scalar = 0.5f * difference * STIFFNESS;

		glm::vec3 translate = glm::vec3(diff.x * scalar, diff.y * scalar, diff.z * scalar);

		if (!m_body1->m_canMove)
		{
			m_body2->OffsetPosition(-glm::vec3(translate.x * 2, translate.y * 2, translate.z * 2));
		}
		else if (!m_body2->m_canMove)
		{
			m_body1->OffsetPosition(glm::vec3(translate.x * 2, translate.y * 2, translate.z * 2));
		}
		else
		{
			m_body1->OffsetPosition(translate);
			m_body2->OffsetPosition(-translate);
		}
			
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
	void ApplyWind();
	glm::vec3 GetNormalFromPoints(ClothNode _n1, ClothNode _n2, ClothNode _n3);

	std::vector<ClothJoint> m_jointList;
	std::vector<DrawData> m_drawDataList;
	ClothNode m_clothNodes[CLOTH_LOD][CLOTH_LOD];
	glm::vec3 m_position;
	Camera* m_pCamera;

	GLuint m_program, m_vbo, m_vao;
};