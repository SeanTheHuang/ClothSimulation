#pragma once

#include "include.h"
#include "model.h"

//An entity is always a 3D object in the scene
class Entity
{
//=== Methods
public:
	Entity();
	Entity(CModel* _model, Camera* _camera);

	//Set Initial MVP of Entity
	void Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationRadians, glm::vec3 _colour);
	void Update();
	void Render();
	void EnableStencil(glm::vec3 _colour);

//=== Members
protected:
	CModel* m_pModel;
	Camera* m_pCamera;
	glm::vec3 m_v3Colour;

	glm::vec3 m_v3Position;
	glm::vec3 m_v3Scale;
	glm::vec3 m_v3RotationAxis;
	float m_fRotation;

	glm::vec3 m_v3StencilColour;
	bool m_bStencil;

};