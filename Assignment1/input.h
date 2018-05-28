#pragma once

#include "include.h"


//Gets input through OpenGL systems
//Input is a [Singleton class]
class Input
{
	//=== Methods
public:
	static Input& Instance(); //Singleton stuff
							  //Clear memory
	static void Destroy();

	//Returns if key was pressed this frame
	bool GetKeyDown(char _key);
	//Returns if key is being pressed
	bool GetKey(char _key);
	//Returns if key was released this frame
	bool GetKeyUp(char _key);

	//Returns current mouse position in pixels
	glm::vec2 MousePosition();

	//Clears key down/up data for a frame
	void Clear();

	//Initializes glut call backs
	void Initialize();



private:
	Input(); //Singleton stuff
	Input(const Input&) = delete;
	void operator=(Input const&) = delete;

	static void KeyDown(unsigned char _key, int _x, int _y); //OpenGL input stuff
	static void KeyUp(unsigned char _key, int _x, int _y);
	static void MouseMove(int _x, int _y);

	//=== Members
private:
	static Input* s_instance;
	std::vector<unsigned char> m_pVecKeysDown;
	std::vector<unsigned char> m_pVecKeysUp;
	std::vector<unsigned char> m_pVecKeys;

	int m_iMouseX, m_iMouseY;
};