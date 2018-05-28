#pragma once

#include "include.h"
#include "camera.h"

#include <iostream>
#include <sstream>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

struct SVertexData {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct STextureData {
	unsigned int id;
	std::string type;
	aiString path;

};

class CMesh {
public:

	std::vector<SVertexData> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<STextureData> m_textures;

	CMesh(std::vector<SVertexData> vertices, std::vector<unsigned int> indices, std::vector<STextureData> textures);
	void Render(GLuint shader, Camera* camera, glm::mat4 modelMatrix, glm::vec3 colour);
	void RenderStencil(GLuint shader, Camera* camera, glm::mat4 modelMatrix, glm::vec3 colour);
private:

	GLuint m_VAO, m_VBO, m_EBO;

	void SetUpMesh();
};