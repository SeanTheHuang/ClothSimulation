#pragma once
#include <chrono>

class Time
{
public:
	//== Methods
public:
	static Time& Instance(); //Singleton stuff
								   //Clear Memory. Calls Destroy on all Level objects
	static void Destroy();
	//Initializes the game clcok
	void Initialize();
	//Calls updates deltaTime and total time played
	void Update();

	//Get time since last frame in seconds
	float DeltaTime();
	//Get total time of application in seconds
	float TotalTime();

private:
	Time(); //Singleton stuff
	Time(const Time&) = delete;
	void operator=(Time const&) = delete;


	//== Members
private:
	static Time* s_instance;

	std::chrono::time_point<std::chrono::system_clock> m_lastTime;
	double m_dTime;
	double m_dDeltaTime;
};