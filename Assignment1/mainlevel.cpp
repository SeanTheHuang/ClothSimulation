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
	floor->Initialize(glm::vec3(0, -5, 10), glm::vec3(20, 1, 20), glm::vec3(0, 1, 0), 0, glm::vec4(0.7f, 0.7f, 0.7f, 1));
	this->AddCube(floor);

	this->SetCloth(new Cloth(m_pCamera, glm::vec3(-2.5f, 4, 10), "Assets/Images/Jeffrey.png"));
	
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

	if (Input::Instance().GetKeyDown('q'))
	{
		m_pCloth->DropCloth();
	}
}