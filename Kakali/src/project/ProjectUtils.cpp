#include <Nigozi.h>
#include "ProjectUtils.h"

ProjectUtils::Error ProjectUtils::CreateProject(const std::filesystem::path& projectDir, const std::string& projectName)
{
	if (!std::filesystem::exists(projectDir) || !std::filesystem::is_directory(projectDir)) {
		return ProjectUtils::Error::InvalidProjectDir;
	}

	std::filesystem::path fullPath = projectDir / projectName;
	if (std::filesystem::exists(fullPath)) {
		return ProjectUtils::Error::ProjectPathAlreadyExists;
	}

	Nigozi::Project::s_ProjectDir = fullPath;
	Nigozi::Project::s_ProjectName = projectName;

	std::filesystem::create_directories(Nigozi::Project::s_ProjectDir / "Resources" / "Build");
	std::filesystem::create_directories(Nigozi::Project::s_ProjectDir / "Resources" / "Intermediates");
	std::filesystem::create_directories(Nigozi::Project::s_ProjectDir / "Assets" / "Scenes");
	std::filesystem::create_directories(Nigozi::Project::s_ProjectDir / "Assets" / "Scripts");
	Nigozi::Project::s_ProjectAssemblyDir = Nigozi::Project::s_ProjectDir / "Resources" / "Build";

	std::string premakePath = Nigozi::Environment::GetVariable("premake5");
	if (premakePath.empty()) {
		NG_CLIENT_LOG_INFO("[Project] Premake variable not found! setting one locally...");
			
		#ifdef NG_PLATFORM_WINDOWS
			std::string premakeBin = "premake5.exe";
		#else
			std::string premakeBin = "premake5";
		#endif
			
		premakePath = std::filesystem::absolute(std::filesystem::path(premakeBin)).string();
		Nigozi::Environment::SetLocalVariable("premake5", premakePath);
	}

	std::string csharpCoreBinPathName = "NIGOZI_CSHARP_CORE_BIN";
	std::string csharpCoreBinPath = Nigozi::Environment::GetVariable("NIGOZI_CSHARP_CORE_BIN");
	if (csharpCoreBinPath.empty()) {
		NG_CLIENT_LOG_INFO("[Project] Setting CSharp Core path...");
			
		std::filesystem::path path("src/res/scripts");

		csharpCoreBinPath = std::filesystem::absolute(path).string();
		Nigozi::Environment::SetLocalVariable(csharpCoreBinPathName, csharpCoreBinPath);
	}

	std::filesystem::path premakeWorkspaceTemplatePath("src/project/premake5-workspace.lua");
	if (!std::filesystem::exists(premakeWorkspaceTemplatePath)) {
		NG_CLIENT_LOG_CRITICAL("[Project] Premake project config script not found!");

		Nigozi::Application::Close();
	}

	std::filesystem::path premakeProjectTemplatePath("src/project/premake5-project.lua");
	if (!std::filesystem::exists(premakeProjectTemplatePath)) {
		NG_CLIENT_LOG_CRITICAL("[Project] Premake project config script not found!");

		Nigozi::Application::Close();
	}

	std::ifstream premakeWorkspaceTemplate(premakeWorkspaceTemplatePath);
	std::stringstream premakeWorkspaceTemplateStream;
	premakeWorkspaceTemplateStream << premakeWorkspaceTemplate.rdbuf();
	std::string premakeWorkspaceTemplateContent = premakeWorkspaceTemplateStream.str();

	std::ifstream premakeProjectTemplate(premakeProjectTemplatePath);
	std::stringstream premakeProjectTemplateStream;
	premakeProjectTemplateStream << premakeProjectTemplate.rdbuf();
	std::string premakeProjectTemplateContent = premakeProjectTemplateStream.str();

	{
		// Set variables in script
		std::string projectNameVar = "%{PROJECT_NAME}";
		size_t startPos = 0;
		while ((startPos = premakeWorkspaceTemplateContent.find(projectNameVar, startPos)) != std::string::npos) {
			premakeWorkspaceTemplateContent.replace(startPos, projectNameVar.length(), Nigozi::Project::s_ProjectName);
			startPos += Nigozi::Project::s_ProjectName.length();
		}

		// Set variables in script
		startPos = 0;
		while ((startPos = premakeProjectTemplateContent.find(projectNameVar, startPos)) != std::string::npos) {
			premakeProjectTemplateContent.replace(startPos, projectNameVar.length(), Nigozi::Project::s_ProjectName);
			startPos += Nigozi::Project::s_ProjectName.length();
		}

		std::string csharpCoreBinName = "%{NIGOZI_CSHARP_CORE_BIN}";
		// for premake format
		std::replace(csharpCoreBinPath.begin(), csharpCoreBinPath.end(), '\\', '/');
		startPos = 0;
		while ((startPos = premakeProjectTemplateContent.find(csharpCoreBinName, startPos)) != std::string::npos) {
			premakeProjectTemplateContent.replace(startPos, csharpCoreBinName.length(), csharpCoreBinPath);
			startPos += csharpCoreBinPath.length();
		}
	}

	#ifdef NG_PLATFORM_WINDOWS
		std::string premakeScript = "CreateScriptProject.bat";
		std::string sysCommand = "cmd.exe /C cd " + Nigozi::Project::s_ProjectDir.string() + " && ";
		sysCommand += premakeScript;
	#else
		std::string premakeScript = "CreateScriptProject.sh";
		std::string sysCommand = "cd " + Nigozi::Project::s_ProjectDir.string() + " && ";
		sysCommand += ("sh " + premakeScript);
	#endif

	// Copy scripts files
	{
		std::ofstream out(Nigozi::Project::s_ProjectDir / "premake5.lua");
		out << premakeWorkspaceTemplateContent;
	}
	{
		std::ofstream out(Nigozi::Project::s_ProjectDir / "Assets" / "Scripts" / "premake5.lua");
		out << premakeProjectTemplateContent;
	}
	std::filesystem::copy_file(std::filesystem::path("src/res/scripts") / premakeScript, Nigozi::Project::s_ProjectDir / premakeScript);
	std::filesystem::copy_file(std::filesystem::path("src/res/scripts") / ".editorconfig", Nigozi::Project::s_ProjectDir / ".editorconfig");

	if (!Nigozi::SubProcess::Run(sysCommand)) {
		return ProjectUtils::Error::SubprocessFailed;
	}

	SerializeProjectMetadata();

	return ProjectUtils::Error::None;
}

void ProjectUtils::RecreateScriptsProject()
{
	std::string premakePath = Nigozi::Environment::GetVariable("premake5");
	if (premakePath.empty()) {
		NG_CLIENT_LOG_INFO("[Project] Premake variable not found! setting one locally...");

		#ifdef NG_PLATFORM_WINDOWS
			std::string premakeBin = "premake5.exe";
		#else
			std::string premakeBin = "premake5";
		#endif

		premakePath = std::filesystem::absolute(std::filesystem::path(premakeBin)).string();
		Nigozi::Environment::SetLocalVariable("premake", premakePath);
	}

	#ifdef NG_PLATFORM_WINDOWS
		std::string premakeScript = "CreateScriptProject.bat";
		std::string sysCommand = "cmd.exe /C cd " + Nigozi::Project::s_ProjectDir.string() + " && ";
		sysCommand += premakeScript;
	#else
		std::string premakeScript = "CreateScriptProject.sh";
		std::string sysCommand = "cd " + Nigozi::Project::s_ProjectDir.string() + " && ";
		sysCommand += ("sh" + (Nigozi::Project::s_ProjectDir / premakeScript).string();
	#endif

	Nigozi::SubProcess::Run(sysCommand);
}

void ProjectUtils::SerializeProjectMetadata()
{
	YAML::Node data;

	data["Project"]["Name"] = Nigozi::Project::s_ProjectName;
	data["Project"]["AssemblyDir"] = std::filesystem::absolute(Nigozi::Project::s_ProjectAssemblyDir.relative_path()).string();

	std::ofstream outFile(Nigozi::Project::s_ProjectDir / (Nigozi::Project::s_ProjectName + ".ngprj"));
	outFile << std::setw(4) << data << std::endl;
	if (!outFile.is_open()) {
		NG_CLIENT_LOG_ERROR("Can't open file");
	}
}

void ProjectUtils::DeserializeProjectMetadata()
{
	for (const auto& entry : std::filesystem::directory_iterator(Nigozi::Project::s_ProjectDir)) {
		if (std::filesystem::is_directory(entry)) {
			continue;
		}
		auto tokens = Nigozi::StringUtils::SplitString(entry.path().filename().string(), ".");
		if (tokens.size() < 2) {
			continue;
		}
		if (tokens[1] == "ngprj") {
			YAML::Node data = YAML::LoadFile(entry.path().string());
			Nigozi::Project::s_ProjectName = data["Project"]["Name"].as<std::string>();
			Nigozi::Project::s_ProjectAssemblyDir = data["Project"]["AssemblyDir"].as<std::string>();
			break;
		}
	}
}
