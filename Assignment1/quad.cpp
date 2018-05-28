#include "quad.h"
#include "texture.h"
#include "shaderhelper.h"

Quad::Quad(Camera* _camera, std::string _texFilePath)
{
	m_pCamera = _camera;
	m_textFilePath = _texFilePath;
	m_pTexture = nullptr;
	m_fTimeAlive = 0; //TEMP TO ALLOW NICE RESET

	ShaderHelper::CompileAndLinkShaders("quad_vs.glsl", "quad_fs.glsl", m_shader);
	CreateVAOwithVBOs();
	glUseProgram(m_shader);

	CreateAndLoadTexture(m_textFilePath);
}

Quad::~Quad()
{
	if (m_pTexture)
		delete m_pTexture;
}

void Quad::Initialize(glm::vec3 _pos, glm::vec3 _scale, glm::vec3 _rotationAxis, float _rotationDegrees, glm::vec4 _colour)
{
	m_v3Position = _pos;
	m_v3Scale = _scale;
	m_v3RotationAxis = _rotationAxis;
	m_fRotation = _rotationDegrees;
	m_v4QuadColour = _colour;
}

void Quad::Update()
{
}

void Quad::Render()
{
	glUseProgram(m_shader);
	m_pTexture->bind(GL_TEXTURE0);

	//Set up transparency
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Send stuff to shader
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_v3Scale);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(m_fRotation), m_v3RotationAxis);
	glm::mat4 position = glm::translate(glm::mat4(1.0), m_v3Position);

	glm::mat4 mvp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix() * position * rotation * scale;
	GLint mvpLoc = glGetUniformLocation(m_shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint colourLoc = glGetUniformLocation(m_shader, "mainColour");
	glUniform4fv(colourLoc, 1, glm::value_ptr(m_v4QuadColour));

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

	glUniform1i(m_sampler, 0);

	//Render here
	glBindVertexArray(m_VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Quad::CreateVAOwithVBOs()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	CreateVertexBuffer();
	CreateIndexBuffer();

	glBindVertexArray(0);
}

void Quad::CreateVertexBuffer()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);

	//Location 0 = index positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	//Location 1 = UV coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Quad::CreateIndexBuffer()
{
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndices), QuadIndices, GL_STATIC_DRAW);
}

void Quad::CreateAndLoadTexture(std::string fileName)
{
	m_pTexture = new Texture(GL_TEXTURE_2D, fileName);
	if (!m_pTexture->load())
	{
		std::cout << "Error Loading Texture from filepath: " << fileName;
	}
}