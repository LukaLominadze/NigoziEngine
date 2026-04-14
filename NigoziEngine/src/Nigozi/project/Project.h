#pragma once
#include <filesystem>

namespace Nigozi
{
	static class Project
	{
	public:
		static void SerializeProjectMetadata();
		static void DeserializeProjectMetadata();
		
		static std::filesystem::path s_ProjectDir;
		static std::filesystem::path s_ProjectAssemblyDir;
		static std::string s_ProjectName;
	};
}
