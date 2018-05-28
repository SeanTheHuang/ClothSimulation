#pragma once

#include <map>
#include "model.h"

//Will load all models used in the game
//[Singleton class]. Other classes call this class to a model pointer
class ModelLoader
{
//== Methods
public:
	static ModelLoader& Instance(); //Singleton stuff
	//Clear Memory. Destroys all models
	static void Destroy();
	//Initializes all the models and puts into map
	void Initialize();
	CModel* GetModel(std::string _modelName);

private:
	ModelLoader(); //Singleton stuff
	ModelLoader(const ModelLoader&) = delete;
	void operator=(ModelLoader const&) = delete;


	//== Members
private:
	static ModelLoader* s_instance;
	std::map<std::string, CModel*> m_mapModels;
};
