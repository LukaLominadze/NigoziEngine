#include "ngpch.h"
#include "Project.h"
#include "yaml-cpp/yaml.h"
#include "utils/StringUtils.h"
#include "core/Log.h"

namespace Nigozi
{
	std::filesystem::path Project::s_ProjectDir;
	std::filesystem::path Project::s_ProjectAssemblyDir;
	std::string Project::s_ProjectName;

	void Project::SerializeProjectMetadata()
	{
		YAML::Node data;

		data["Project"]["Name"] = s_ProjectName;
		data["Project"]["AssemblyDir"] = s_ProjectAssemblyDir.relative_path().string();

		{
			std::ofstream outFile(s_ProjectDir / (s_ProjectName + ".ngprj"));
			outFile << std::setw(4) << data << std::endl;
			if (!outFile.is_open()) {
				NG_CORE_LOG_ERROR("Can't open file");
			}
		}
	}

	void Project::DeserializeProjectMetadata()
	{
		for (const auto& entry : std::filesystem::directory_iterator(s_ProjectDir)) {
			if (!std::filesystem::is_directory(entry)) {
				auto tokens = StringUtils::SplitString(entry.path().filename().string(), ".");
				if (tokens.size() < 2) {
					continue;
				}
				if (tokens[1] == "ngprj") {
					YAML::Node data = YAML::LoadFile(entry.path().string());
					s_ProjectName = data["Project"]["Name"].as<std::string>();
					s_ProjectAssemblyDir = data["Project"]["AssemblyDir"].as<std::string>();
					break;
				}
			}
		}
	}
}
