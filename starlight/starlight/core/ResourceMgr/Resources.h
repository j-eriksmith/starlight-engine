#pragma once
#include <string>

namespace Resources
{
	inline static std::string Get(const std::string& query)
	{
		return "Resources/" + query;
	}
}
