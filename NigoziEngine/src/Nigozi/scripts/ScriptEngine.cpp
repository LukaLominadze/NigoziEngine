#include "ngpch.h"
#include "ScriptEngine.h"

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

namespace Nigozi
{
	void ScriptEngine::Initialize()
	{
		LoadAssemblies("");
	}

	void ScriptEngine::Deinitialize()
	{
		UnloadAssemblies();
	}

	static void ExceptionCallback(std::string_view InMessage)
	{
		std::cout << "Unhandled native exception: " << InMessage << std::endl;
	}

	void ScriptEngine::LoadAssemblies(const std::filesystem::path& coralDir)
	{
		Coral::HostSettings hostSettings;
		hostSettings.CoralDirectory = coralDir.string();
		hostSettings.ExceptionCallback = ExceptionCallback;

		Coral::HostInstance hostInstance;
		hostInstance.Initialize(hostSettings);

		//TODO:
	}

	void ScriptEngine::UnloadAssemblies()
	{

	}
}