#pragma once
#include <iostream>

constexpr bool DEBUG = false;
#define Log(x) if (DEBUG) std::cout << x << std::endl