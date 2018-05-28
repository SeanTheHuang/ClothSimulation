#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

#include "include.h"
#include "modelloader.h"
#include "gamemaster.h"

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Call render in game master
	GameMaster::Instance().Render();

	//Finished rendering
	glutSwapBuffers();
}

void Update()
{
	//Update time
	Time::Instance().Update();

	//Call update on game master
	GameMaster::Instance().Update();

	//Clear inputs for that frame
	Input::Instance().Clear();

	//Render again
	glutPostRedisplay();
}

void InitializeGlutCallbacks()
{
	//Draw and update loops
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Update);
	
	//Time
	Time::Instance().Initialize();

	//Mouse and keyboard input
	Input::Instance().Initialize();
	
	//Sounds
	//TODO D:<<<<<

	//Models
	ModelLoader::Instance().Initialize();

	//Overall game
	GameMaster::Instance().Initialize();
}

void ApplicationCleanUp()
{
	Time::Destroy();
	Input::Destroy();
	ModelLoader::Destroy();
	GameMaster::Destroy();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("The greatest template created of all time that Sean has made so far which he hopes he will not need to make another one in the forseeable future");

	GLenum res = glewInit();
	srand((unsigned int)time(NULL));
	if (res != GLEW_OK)
	{
		std::cout << "error: ," << glewGetErrorString(res);
		return 1;
	}

	InitializeGlutCallbacks();
	glClearColor(0.05f, 0.00f, 0.07f, 1.0f);

	//Sissor test
	int cutAmount = 60;
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, cutAmount, WINDOW_WIDTH, WINDOW_HEIGHT - 2*cutAmount);

	//AA
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

	//BF culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//=== Start game ===

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

	//=== Clean up after game over ===
	ApplicationCleanUp();
	
	_CrtDumpMemoryLeaks(); //TEMP CHECK FOR MEMORY LEAKS
	return 0;
}