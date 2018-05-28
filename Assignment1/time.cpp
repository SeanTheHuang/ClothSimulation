#include "time.h"

Time* Time::s_instance = nullptr;

Time& Time::Instance()
{
	if (!s_instance)
		s_instance = new Time();

	return (*s_instance);
}

void Time::Destroy()
{
	//Nothing to destroy
	if (!s_instance)
		return;

	delete s_instance;

	s_instance = nullptr;
}

void Time::Initialize()
{
	//Start game clock here
	m_lastTime = std::chrono::system_clock::now();
	m_dDeltaTime = 0;
	m_dTime = 0;
}

void Time::Update()
{
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = now - m_lastTime;
	m_dDeltaTime = diff.count();
	m_dTime += m_dDeltaTime;

	m_lastTime = now;
}

float Time::DeltaTime()
{
	return (float)m_dDeltaTime;
}

float Time::TotalTime()
{
	return (float)m_dTime;
}

Time::Time()
{
}
