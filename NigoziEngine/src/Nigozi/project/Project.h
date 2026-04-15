#include <filesystem>
#include <string>

namespace Nigozi
{
	static class Project
	{
	public:
		inline static std::filesystem::path s_ProjectDir;
		inline static std::filesystem::path s_ProjectAssemblyDir;
		inline static std::string s_ProjectName;
	};
}