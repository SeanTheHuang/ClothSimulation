#include "camera.h"

Camera::Camera()
{
	m_position = glm::vec3(0, 0, 0);
	m_target = glm::vec3(0, 0, 10);

	m_projectionMatrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 5000.0f);
	m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0, 1, 0));

	m_v3FogColour = glm::vec3(0.10f, 0.0f, 0.12f);
	m_fFogStart = 15;
	m_fFogEnd = 60;
}


glm::mat4 Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

glm::vec3 Camera::GetCameraForward()
{
	return glm::normalize(m_target - m_position);
}

glm::vec3 Camera::FogColour()
{
	return m_v3FogColour;
}

float Camera::FogStart()
{
	return m_fFogStart;
}

float Camera::FogEnd()
{
	return m_fFogEnd;
}

void Camera::UpdateCameraPosition(glm::vec3 newPosition)
{
	m_position = newPosition;
	m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0, 1, 0));
}

void Camera::UpdateCameraViewDirection(glm::vec3 viewDirection)
{
	m_target = m_position + viewDirection;
	m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0, 1, 0));
}

