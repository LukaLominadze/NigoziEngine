#pragma once
#include "ngpch.h"

namespace Nigozi
{
	static class ScriptEngine
	{
	public:
		static void Initialize();
		static void Deinitialize();
	private:
		static void LoadAssemblies(const std::filesystem::path& coralDir);
		static void UnloadAssemblies();
	};
}

