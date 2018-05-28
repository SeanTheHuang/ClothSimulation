#include "cube.h"

#include "shaderhelper.h"

void Cube::CreateVAOwithVBOs()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	CreateVertexBuffer();
	CreateIndexBuffer();

	glBindVertexArray(0);
}

void Cube::CreateVertexBuffer()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

	//Location 0 = index positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	//Location 1 = UV coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(1);

	//Location 2 = Normal coords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::CreateIndexBuffer()
{
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
}

Cube::Cube(Camera* _camera)
{
	m_pCamera = _camera;
	ShaderHelper::CompileAndLinkShaders("cube_vs.glsl", "cube_fs.glsl", m_shader);
	CreateVAOwithVBOs();
	glUseProgram(m_shader);
}

void Cube::Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationDegrees, glm::vec4 _colour)
{
	m_v3Position = _pos;
	m_v3Scale = _scale;
	m_v3RotationAxis = _rotationAxis;
	m_fRotation = _rotationDegrees;
	m_v4Colour = _colour;
}

void Cube::Update()
{
}

void Cube::Render()
{
	glUseProgram(m_shader);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Send stuff to shader
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_v3Scale);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(m_fRotation), m_v3RotationAxis);
	glm::mat4 position = glm::translate(glm::mat4(1.0), m_v3Position);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(m_pCamera->GetViewMatrix() * position * rotation * scale));

	glm::mat4 mvp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() * position * rotation * scale;
	GLint mvpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint normalMatLoc = glGetUniformLocation(m_shader, "normalMat");
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	GLint colourLoc = glGetUniformLocation(m_shader, "mainColour");
	glUniform4fv(colourLoc, 1, glm::value_ptr(m_v4Colour));

	GLint modelMatrixLoc = glGetUniformLocation(m_shader, "model");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(position * rotation * scale));

	GLint cameraPosLoc = glGetUniformLocation(m_shader, "cameraPos");
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(m_pCamera->GetPosition()));

	GLint fogColourLoc = glGetUniformLocation(m_shader, "fogColour");
	glUniform3fv(fogColourLoc, 1, glm::value_ptr(m_pCamera->FogColour()));

	GLint fogStartLoc = glGetUniformLocation(m_shader, "fogStart");
	glUniform1f(fogStartLoc, m_pCamera->FogStart());

	GLint fogEndLoc = glGetUniformLocation(m_shader, "fogEnd");
	glUniform1f(fogEndLoc, m_pCamera->FogEnd());

	//Render here
	glBindVertexArray(m_VAO);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}

glm::mat4 Cube::ModelMatrix()
{
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_v3Scale);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(m_fRotation), m_v3RotationAxis);
	glm::mat4 position = glm::translate(glm::mat4(1.0), m_v3Position);
	return position * rotation * scale;
}
