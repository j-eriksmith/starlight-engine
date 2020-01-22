#pragma once
#include <chrono>

class Clock
{
public:
	Clock()
		: StartTime(std::chrono::high_resolution_clock::now()) {}
	
	double GetTimeSinceStartup();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTime;
};
