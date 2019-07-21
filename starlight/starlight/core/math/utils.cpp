#include "utils.h"

#include <cmath>

float Utils::Cos(float radians) { return cosf(radians); }
float Utils::Sin(float radians) { return sinf(radians); }
float Utils::Tan(float radians) { return tanf(radians); }
float Utils::Acos(float radians) { return acosf(radians); }
float Utils::Asin(float radians) { return asinf(radians); }
float Utils::Atan(float radians) { return atanf(radians); }
float Utils::Atan2(float y, float x) { return atan2f(y, x); }
float Utils::Sqrt(float n) { return sqrtf(n); }
float Utils::Abs(float n) { return fabs(n); }

float Utils::RadToDeg(float radians)
{
	return radians * (180.0f / PI);
}

float Utils::DegToRad(float degrees)
{
	return degrees * (PI / 180.0f);
}
