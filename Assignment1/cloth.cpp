#include "cloth.h"
#include "shaderhelper.h"

Cloth::Cloth(Camera * _camera, glm::vec3 _position)
{
	m_pCamera = _camera;
	m_position = _position;

	Initialize();
}

Cloth::~Cloth()
{
}

void Cloth::Initialize()
{
	float xOffset = CLOTH_SIZE / ((float)CLOTH_LOD-1);
	float yOffset = CLOTH_SIZE / ((float)CLOTH_LOD-1);

	float uvOffset = 1.0f / ((float)CLOTH_LOD-1);

	// Start creating cloth nodes
	// Position of cloth = TOP LEFT
	for (int y = 0; y < CLOTH_LOD; y++)
	{
		for (int x = 0; x < CLOTH_LOD; x++)
		{
			m_clothNodes[x][y].Initialize(glm::vec3(xOffset * x, yOffset * y * -1, 0) + m_position, glm::vec2(uvOffset * x, 1 - uvOffset * y), x, y);
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

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_drawDataList.size());

	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void Cloth::Update()
{
	// Update position based on restrictions
	for (size_t i = 0; i < m_jointList.size(); i++)
	{
		m_jointList[i].Update();
	}

	for (int y = 0; y < CLOTH_LOD; y++)
	{
		for (int x = 0; x < CLOTH_LOD; x++)
		{
			// Apply acceleration objects
			// Gravity + some wind force
			m_clothNodes[x][y].m_acceleration = glm::vec3(0, -10, 8.5f);
			m_clothNodes[x][y].Update();
		}
	}
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
