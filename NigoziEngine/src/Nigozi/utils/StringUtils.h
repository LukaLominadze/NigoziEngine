#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace Nigozi 
{
	namespace StringUtils
	{
		// From Walnut - https://github.com/StudioCherno/Walnut
		std::vector<std::string> SplitString(const std::string_view string, const std::string_view& delimiters);
		std::vector<std::string> SplitString(const std::string_view string, const char delimiter);
	}
}
