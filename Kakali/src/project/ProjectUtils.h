#pragma once
#include <filesystem>


static class ProjectUtils
{
public:
	enum class Error 
	{
		None = 0,
		InvalidProjectDir,
		ProjectPathAlreadyExists,
		SubprocessFailed
	};

	static Error CreateProject(const std::filesystem::path& projectDir, const std::string& projectName);
	static void RecreateScriptsProject();

	static void SerializeProjectMetadata();
	static void DeserializeProjectMetadata();
};
