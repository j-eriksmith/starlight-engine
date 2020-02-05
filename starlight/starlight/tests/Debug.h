#pragma once
#include <iostream>
#include <string>

constexpr bool DEBUG = false;
#define Log(x) if (DEBUG) std::cout << x << std::endl