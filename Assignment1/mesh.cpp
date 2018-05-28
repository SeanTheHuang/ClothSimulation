#include "mesh.h"

CMesh::CMesh(std::vector<SVertexData> vertices, std::vector<unsigned int> indices, std::vector<STextureData> textures)
{
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = textures;

	SetUpMesh();
}

void CMesh::Render(GLuint shader, Camera* camera, glm::mat4 modelMatrix, glm::vec3 colour)
{
	glUseProgram(shader);
	glEnable(GL_DEPTH_TEST);

	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // transfer unsigned int to stream
		else if (name == "texture_specular")
			ss << specularNr++; // transfer unsigned int to stream
		number = ss.str();

		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * modelMatrix;
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(camera->GetViewMatrix() * modelMatrix));
	glm::vec3 cameraPos = camera->GetPosition();

	GLint mvpLoc = glGetUniformLocation(shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint normalMatLoc = glGetUniformLocation(shader, "normalMat");
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	GLint cameraPosLoc = glGetUniformLocation(shader, "cameraPos");
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

	GLint modelMatrixLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint colourLoc = glGetUniformLocation(shader, "modelColour");
	glUniform3fv(colourLoc, 1, glm::value_ptr(colour));

	GLint fogColourLoc = glGetUniformLocation(shader, "fogColour");
	glUniform3fv(fogColourLoc, 1, glm::value_ptr(camera->FogColour()));

	GLint fogStartLoc = glGetUniformLocation(shader, "fogStart");
	glUniform1f(fogStartLoc, camera->FogStart());

	GLint fogEndLoc = glGetUniformLocation(shader, "fogEnd");
	glUniform1f(fogEndLoc, camera->FogEnd());

	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->m_textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_DEPTH_TEST);
}

void CMesh::RenderStencil(GLuint shader, Camera * camera, glm::mat4 modelMatrix, glm::vec3 colour)
{
	glUseProgram(shader);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * modelMatrix;
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(camera->GetViewMatrix() * modelMatrix));
	glm::vec3 cameraPos = camera->GetPosition();

	GLint mvpLoc = glGetUniformLocation(shader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint normalMatLoc = glGetUniformLocation(shader, "normalMat");
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	GLint cameraPosLoc = glGetUniformLocation(shader, "cameraPos");
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

	GLint modelMatrixLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	GLint colourLoc = glGetUniformLocation(shader, "modelColour");
	glUniform3fv(colourLoc, 1, glm::value_ptr(colour));

	GLint fogColourLoc = glGetUniformLocation(shader, "fogColour");
	glUniform3fv(fogColourLoc, 1, glm::value_ptr(camera->FogColour()));

	GLint fogStartLoc = glGetUniformLocation(shader, "fogStart");
	glUniform1f(fogStartLoc, camera->FogStart());

	GLint fogEndLoc = glGetUniformLocation(shader, "fogEnd");
	glUniform1f(fogEndLoc, camera->FogEnd());

	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisable(GL_DEPTH_TEST);
}

void CMesh::SetUpMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(SVertexData), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// layout 0 = vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)0);
	// layout 1 = vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)offsetof(SVertexData, normal));
	// layout 2 = vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertexData), (void*)offsetof(SVertexData, texCoords));

	glBindVertexArray(0);
}
