#pragma once

#include "include.h"

//Camera initial position [0, 0, 0]
//		 initial facing direction [0, 0, 1]
class Camera
{
public:
	Camera();
	~Camera() = default;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetPosition();
	glm::vec3 GetCameraForward();

	glm::vec3 FogColour();
	float FogStart();
	float FogEnd();

	void UpdateCameraPosition(glm::vec3 newPosition);
	void UpdateCameraViewDirection(glm::vec3 viewDirection);

private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	glm::vec3 m_position;
	glm::vec3 m_target;

	glm::vec3 m_v3FogColour;
	float m_fFogStart;
	float m_fFogEnd;
};

