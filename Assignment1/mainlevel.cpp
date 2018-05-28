#include "mainlevel.h"
#include "modelloader.h"

void MainLevel::Initialize()
{
	//Create Camera(s) for the level
	m_pCamera = new Camera();

	//Creates all objects in the level.
	//IF WANT NEW LEVEL:
	//		- Create a new class override Level's Initialize for different levels

	// Create a floor
	Cube* floor = new Cube(m_pCamera);
	floor->Initialize(glm::vec3(0, -3, 10), glm::vec3(5, 1, 5), glm::vec3(0, 1, 0), 0, glm::vec4(0.7f, 0.7f, 0.7f, 1));
	this->AddCube(floor);
	
}

void MainLevel::Update()
{
	Level::Update();

	//Reset scene if player presses 'R'
	if (Input::Instance().GetKeyDown('r'))
	{
		CleanUp();
		Initialize();
	}
}