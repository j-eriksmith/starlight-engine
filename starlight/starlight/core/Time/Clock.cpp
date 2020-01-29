#include "Clock.h"
#include <iostream>

double Clock::GetTimeSinceStartup()
{
	std::chrono::duration<double> BetweenNowAndStart = std::chrono::high_resolution_clock::now() - StartTime;
	return BetweenNowAndStart.count();
}