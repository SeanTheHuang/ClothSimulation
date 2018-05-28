#include "entity.h"

Entity::Entity()
{
	//Calling default constructor = inheriting this class
	m_pModel = nullptr;
}

Entity::Entity(CModel* _model, Camera* _camera)
{
	m_pModel = _model;
	m_pCamera = _camera;
	m_bStencil = false;
}

void Entity::Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationDegrees, glm::vec3 _colour)
{
	m_v3Position = _pos;
	m_v3Scale = _scale;
	m_v3RotationAxis = _rotationAxis;
	m_fRotation = _rotationDegrees;
	m_v3Colour = _colour;
}

void Entity::Update()
{
	m_fRotation += 90 * Time::Instance().DeltaTime();
}

void Entity::Render()
{
	//Prepare stencil rendering
	if (m_bStencil)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}

	//Render main model
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_v3Scale);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(m_fRotation), m_v3RotationAxis);
	glm::mat4 position = glm::translate(glm::mat4(1.0), m_v3Position);

	m_pModel->SetColour(m_v3Colour);
	m_pModel->SetModelMatrix(position * rotation * scale);
	m_pModel->Render(m_pCamera);

	//Render stencil
	if (m_bStencil)
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		//Slightly enlarge for stencil to show up
		scale = glm::scale(glm::mat4(1.0f), m_v3Scale * 1.1f);

		//Render here
		m_pModel->SetModelMatrix(position * rotation * scale);
		m_pModel->RenderStencil(m_pCamera, m_v3StencilColour);

		//Clean up
		glStencilMask(0xFF);
		glDisable(GL_STENCIL_TEST);
	}
}

void Entity::EnableStencil(glm::vec3 _colour)
{
	m_bStencil = true;
	m_v3StencilColour = _colour;
}
