#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "mesh.h"

class CModel
{
//=== Methods
public:
	CModel(GLchar* path);
	~CModel();
	void Render(Camera* _camera);
	void RenderStencil(Camera* _camera, glm::vec3 _colour);
	void SetModelMatrix(glm::mat4 modelMatrix);
	void SetColour(glm::vec3 colour);
	bool Initialise();
private:

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	CMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<STextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char* path, std::string directory);

//=== Members
private:
	GLuint m_mainShader, m_stencilShader;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_colour;
	GLchar* m_path;
	std::vector<CMesh> meshes;
	std::string directory;
	std::vector<STextureData> textures_loaded;
};