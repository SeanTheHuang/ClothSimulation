#pragma once

#include "include.h"
#include "camera.h"
#include <string>

static GLfloat QuadVertices[]
{
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,	//Top left
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //Top right
	1.0f, -1.0f, 0.0f, 1.0f, 1.0f, //Bot right
	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f //Bot left
};

static GLuint QuadIndices[]
{
	0, 1, 2, 3
};

class Texture;

class Quad
{
//=== Methods
public:
	Quad() = delete;
	Quad(Camera* _camera, std::string _texFilePath);
	~Quad();

	 void Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationRadians, glm::vec4 _colour);
	 void Update();
	 void Render();


protected:
	void CreateVAOwithVBOs();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateAndLoadTexture(std::string fileName);

//=== Members
protected:
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Scale;
	glm::vec3 m_v3RotationAxis;
	float m_fRotation;
	glm::vec4 m_v4QuadColour;
	std::string m_textFilePath;
	Texture* m_pTexture;
	Camera* m_pCamera;
	float m_fTimeAlive; //TEMP TO ALLOW NICE RESET

	GLuint m_shader, m_VAO, m_VBO, m_EBO, m_sampler;
};