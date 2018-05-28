#include "modelloader.h"

ModelLoader* ModelLoader::s_instance = nullptr;

ModelLoader& ModelLoader::Instance()
{
	if (!s_instance)
		s_instance = new ModelLoader();

	return (*s_instance);
}

void ModelLoader::Destroy()
{
	if (!s_instance)
		return;

	//Delete all models
	for (auto it = s_instance->m_mapModels.begin(); it != s_instance->m_mapModels.end(); it++)
	{
		delete it->second;
	}

	//Delete singleton
	delete s_instance;
	s_instance = nullptr;
}

void ModelLoader::Initialize()
{
	//=== Load all models ===

	//=== Initialize all ===
	for (auto it = m_mapModels.begin(); it != m_mapModels.end(); it++)
	{
		it->second->Initialise();
	}

}

CModel* ModelLoader::GetModel(std::string _modelName)
{
	return m_mapModels[_modelName];
}

ModelLoader::ModelLoader()
{
}
