#include <string>

namespace Nigozi
{
	static class Environment 
	{
	public:
		static std::string GetVariable(const std::string& name);
		static bool SetLocalVariable(const std::string& name, const std::string& value);
	};
}