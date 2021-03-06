#include "cloth.h"
#include "shaderhelper.h"
#include <math.h>

Cloth::Cloth(Camera * _camera, glm::vec3 _position, std::string _textPath)
{
	m_pCamera = _camera;
	m_position = _position;
	m_textPath = _textPath;

	Initialize();
}

Cloth::~Cloth()
{
	if (m_pTexture)
		delete m_pTexture;
}

void Cloth::Initialize()
{
	m_windForce = 15;
	m_windDirection = glm::normalize(glm::vec3(-5, 0, -5));

	float xOffset = CLOTH_SIZE / ((float)CLOTH_LOD-1);
	float yOffset = CLOTH_SIZE / ((float)CLOTH_LOD-1);

	float uvOffset = 1.0f / ((float)CLOTH_LOD-1);

	// Start creating cloth nodes
	// Position of cloth = TOP LEFT
	for (int y = 0; y < CLOTH_LOD; y++)
	{
		for (int x = 0; x < CLOTH_LOD; x++)
		{
			m_clothNodes[x][y].Initialize(glm::vec3(xOffset * x, yOffset * y * -1, 0) + m_position, glm::vec2(uvOffset * x, uvOffset * y), x, y);
		}
	}

	// Make some nodes unmovable
	m_clothNodes[0][0].m_canMove = false;
	m_clothNodes[CLOTH_LOD - 1][0].m_canMove = false;
	m_clothNodes[CLOTH_LOD / 2][0].m_canMove = false;

	// Set up draw data
	SetUpRenderData();

	// Set up joints
	for (int y = 0; y < CLOTH_LOD; y++)
	{
		for (int x = 0; x < CLOTH_LOD; x++)
		{
			// For each node, try create a link between node [UNDER] & [TO THE LEFT] of them
			if (x + 1 < CLOTH_LOD)
				m_jointList.push_back(ClothJoint(&m_clothNodes[x][y], &m_clothNodes[x + 1][y]));

			if (y + 1 < CLOTH_LOD)
				m_jointList.push_back(ClothJoint(&m_clothNodes[x][y], &m_clothNodes[x][y + 1]));
		}
	}

	// Set up opengl stuffs here
	ShaderHelper::CompileAndLinkShaders("cloth_vs.glsl", "cloth_fs.glsl", m_program);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DrawData) * m_drawDataList.size(), &m_drawDataList[0], GL_STATIC_DRAW);

	//Location 0 = index positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	//Location 1 = UV coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create texture
	m_pTexture = new Texture(GL_TEXTURE_2D, m_textPath);
	if (!m_pTexture->load())
	{
		std::cout << "Error Loading Texture from filepath: " << m_textPath;
	}
}

void Cloth::Render()
{
	// Draw cloth on both sides
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	SetUpRenderData();

	glUseProgram(m_program);

	glm::mat4 vp = m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix();
	GLint vpLoc = glGetUniformLocation(m_program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(DrawData) * m_drawDataList.size(), &m_drawDataList[0], GL_STATIC_DRAW);

	m_pTexture->bind(GL_TEXTURE0);
	glUniform1i(m_sampler, 0);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_drawDataList.size());

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void Cloth::Update()
{
	if (Input::Instance().GetKeyDown('e'))
	{
		RotateWindDirection();
	}

	// Update position based on restrictions
	for (size_t i = 0; i < m_jointList.size(); i++)
	{
		m_jointList[i].Update();
	}

	// Wind force
	ApplyWind();

	for (int y = 0; y < CLOTH_LOD; y++)
	{
		for (int x = 0; x < CLOTH_LOD; x++)
		{
			// Gravity
			m_clothNodes[x][y].AddAcceleration(glm::vec3(0, -10, 0));
			m_clothNodes[x][y].Update();
		}
	}
}

void Cloth::DropCloth()
{
	// Make some nodes unmovable
	m_clothNodes[0][0].m_canMove = true;
	m_clothNodes[CLOTH_LOD - 1][0].m_canMove = true;
	m_clothNodes[CLOTH_LOD / 2][0].m_canMove = true;
}

void Cloth::SetUpRenderData()
{
	m_drawDataList.clear();

	for (int y = 0; y < CLOTH_LOD-1; y++)
	{
		for (int x = 0; x < CLOTH_LOD-1; x++)
		{
			// Set up bunch of triangles in m_drawDataList
			m_drawDataList.push_back(DrawData(m_clothNodes[x][y].m_position, m_clothNodes[x][y].m_uv));
			m_drawDataList.push_back(DrawData(m_clothNodes[x+1][y].m_position, m_clothNodes[x][y].m_uv));
			m_drawDataList.push_back(DrawData(m_clothNodes[x][y+1].m_position, m_clothNodes[x][y].m_uv));

			m_drawDataList.push_back(DrawData(m_clothNodes[x][y+1].m_position, m_clothNodes[x][y].m_uv));
			m_drawDataList.push_back(DrawData(m_clothNodes[x + 1][y].m_position, m_clothNodes[x][y].m_uv));
			m_drawDataList.push_back(DrawData(m_clothNodes[x + 1][y + 1].m_position, m_clothNodes[x][y].m_uv));
		}
	}
}

void Cloth::RotateWindDirection()
{
	const float angle = 45.0f;

	float cosPart = cosf(angle * 3.14159f / 180.0f);
	float sinPart = sinf(angle * 3.14159f / 180.0f);

	glm::vec3 newDirection = glm::vec3();
	newDirection.x = m_windDirection.x * cosPart - m_windDirection.z * sinPart;
	newDirection.z = m_windDirection.x * sinPart + m_windDirection.z * cosPart;
	m_windDirection = newDirection;
}

void Cloth::ApplyWind()
{
	// Apply wind in triangles
	for (int y = 0; y < CLOTH_LOD - 1; y++)
	{
		for (int x = 0; x < CLOTH_LOD - 1; x++)
		{
			glm::vec3 normal = GetNormalFromPoints(m_clothNodes[x][y], m_clothNodes[x + 1][y], m_clothNodes[x][y + 1]);
			float intensity = glm::dot(normal, m_windDirection);
			m_clothNodes[x][y].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));
			m_clothNodes[x+1][y].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));
			m_clothNodes[x][y+1].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));

			normal = GetNormalFromPoints(m_clothNodes[x][y+1], m_clothNodes[x + 1][y], m_clothNodes[x + 1][y + 1]);
			intensity = glm::dot(normal, m_windDirection);
			m_clothNodes[x][y + 1].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));
			m_clothNodes[x + 1][y].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));
			m_clothNodes[x + 1][y + 1].AddAcceleration(glm::vec3(0, 0, intensity * m_windForce));
		}
	}
}

glm::vec3 Cloth::GetNormalFromPoints(ClothNode _n1, ClothNode _n2, ClothNode _n3)
{
	glm::vec3 temp1 = glm::normalize(_n2.m_position - _n1.m_position);
	glm::vec3 temp2 = glm::normalize(_n3.m_position - _n1.m_position);
	return glm::cross(temp1, temp2);
}
