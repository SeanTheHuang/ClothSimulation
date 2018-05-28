#include "input.h"

Input* Input::s_instance = nullptr;

Input& Input::Instance()
{
	if (!s_instance)
		s_instance = new Input();

	return (*s_instance);
}

void Input::Destroy()
{
	//Nothing to destroy
	if (!s_instance)
		return;

	delete s_instance;

	s_instance = nullptr;
}

bool Input::GetKeyDown(char _key)
{
	return (find(m_pVecKeysDown.begin(), m_pVecKeysDown.end(), _key) != m_pVecKeysDown.end());
}

bool Input::GetKey(char _key)
{
	return (find(m_pVecKeys.begin(), m_pVecKeys.end(), _key) != m_pVecKeys.end());
}

bool Input::GetKeyUp(char _key)
{
	return (find(m_pVecKeysUp.begin(), m_pVecKeysUp.end(), _key) != m_pVecKeysUp.end());
}

void Input::Clear()
{
	m_pVecKeysDown.clear();
	m_pVecKeysUp.clear();
}

glm::vec2 Input::MousePosition()
{
	return glm::vec2(m_iMouseX, m_iMouseY);
}

void Input::Initialize()
{
	glutKeyboardFunc(this->KeyDown);
	glutKeyboardUpFunc(this->KeyUp);
	glutPassiveMotionFunc(this->MouseMove);
}

Input::Input()
{
	//Do nothing?
}

void Input::KeyDown(unsigned char _key, int _x, int _y)
{
	//Key down seems to call more than once. Only record key if it is not in vector already
	if (std::find(s_instance->m_pVecKeys.begin(), s_instance->m_pVecKeys.end(), _key) == s_instance->m_pVecKeys.end())
	{
		s_instance->m_pVecKeys.push_back(_key);
		s_instance->m_pVecKeysDown.push_back(_key);
	}
}

void Input::KeyUp(unsigned char _key, int _x, int _y)
{
	s_instance->m_pVecKeysUp.push_back(_key);

	//Remove key from m_pVecKeys vector, as it was released
	s_instance->m_pVecKeys.erase(remove(s_instance->m_pVecKeys.begin(),
		s_instance->m_pVecKeys.end(), _key), s_instance->m_pVecKeys.end());
}

void Input::MouseMove(int _x, int _y)
{
	s_instance->m_iMouseX = _x;
	s_instance->m_iMouseY = _y;
}
