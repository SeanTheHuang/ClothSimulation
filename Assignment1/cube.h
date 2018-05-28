#pragma once

#include "include.h"
#include "camera.h"

static GLfloat CubeVertices[]
{
	1.0f,  1.0f,  1.0f,	1.0f, 0.0f, 0,0,1,// Front Top-right
	-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0,0,1,// Front Top-left
	-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0,0,1,// Front Bot-Left	
	1.0f, -1.0f,  1.0f,	1.0f, 1.0f, 0,0,1,// Front Bot-right

	-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, -1,0,0,// Left Top-right
	-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1,0,0,// Left Top-left
	-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1,0,0,// Left Bot-Left
	-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, -1,0,0,// Left Bot-right

	-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0,0,-1,// Back Top-left
	1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 0,0,-1,// Back Top-right
	1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0,0,-1,// Back Bot-right
	-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0,0,-1,// Back Bot-Left

	1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1,0,0,// Right Top-left
	1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1,0,0,// Right Top-right
	1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 1,0,0,// Right Bot-right
	1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1,0,0,// Right Bot-Left

	1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0,1,0,// Top Top-right
	-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0,1,0,// Top Top-left
	-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0,1,0,// Top Bot-Left
	1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0,1,0,// Top Bot-right

	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0,-1,0,// Bottom Top-left
	1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0,-1,0,// Bottom Top-right
	1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0,-1,0,// Bottom Bot-right
	-1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  0,-1,0// Bottom Bot-Left
};

static GLuint CubeIndices[]
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23
};


class Cube
{
//=== Methods
public:
	Cube() = delete;
	Cube(Camera* _camera);

	void Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationRadians, glm::vec4 _colour);
	void Update();
	void Render();
	glm::mat4 ModelMatrix();

private:
	void CreateVAOwithVBOs();
	void CreateVertexBuffer();
	void CreateIndexBuffer();

//=== Members
private:
	glm::vec3 m_v3Position;
	glm::vec3 m_v3Scale;
	glm::vec3 m_v3RotationAxis;
	float m_fRotation;
	glm::vec4 m_v4Colour;
	std::string m_textFilePath;
	Camera* m_pCamera;

	GLuint m_shader, m_VAO, m_VBO, m_EBO;
};