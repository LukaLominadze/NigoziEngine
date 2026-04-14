#include "ngpch.h"
#include "core/Log.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "scene/Entity.h"
#include "project/Project.h"
#include "scene/Component.h"
#include "utils/StringUtils.h"

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

namespace Nigozi
{
	std::shared_ptr<SceneTree> ScriptEngine::s_currentScene;

	Coral::HostInstance ScriptEngine::s_hostInstance;
	Coral::AssemblyLoadContext ScriptEngine::s_loadContext;
	Coral::ManagedAssembly* ScriptEngine::sp_coreAssembly;
	Coral::ManagedAssembly* ScriptEngine::sp_projectAssembly;

	std::filesystem::path ScriptEngine::s_coralDir;

	void ScriptEngine::Initialize(const std::filesystem::path& coralDir)
	{
		s_coralDir = coralDir;

		LoadHostInstance();
		LoadCoreAssembly();
	}

	void ScriptEngine::Deinitialize()
	{
		UnloadCoreAssembly();
	}

	void OnCoralMessage(std::string_view message, Coral::MessageLevel level)
	{
		switch (level)
		{
		case Coral::MessageLevel::Info:
			NG_CORE_LOG_INFO("[Script Engine] {}", std::string(message));
			break;
		case Coral::MessageLevel::Warning:
			NG_CORE_LOG_WARN("[Script Engine] {}", std::string(message));
			break;
		case Coral::MessageLevel::Error:
			NG_CORE_LOG_ERROR("[Script Engine] {}", std::string(message));
			break;
		}
	}

	static void ExceptionCallback(std::string_view InMessage)
	{
		NG_CORE_LOG_ERROR("[Script Engine] Unhandled native exception: {}", InMessage);
	}

	void ScriptEngine::StartRuntime()
	{
		auto view = s_currentScene->m_Registry.view<ScriptComponent>();

		for (auto entityHandle : view) {
			auto entity = Entity(entityHandle, s_currentScene.get());
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			std::string scriptName = StringUtils::SplitString(scriptComponent.ScriptPath.filename().string(), ".")[0];
			std::string scriptAssemblyName = Project::s_ProjectName + "." + scriptName;

			auto scriptType = sp_projectAssembly->GetLocalType(scriptAssemblyName);

			scriptComponent.ManagedScriptObject = scriptType.CreateInstance((uint64_t)entity.GetUUID());
		}
	}

	void ScriptEngine::EndRuntime()
	{
		auto view = s_currentScene->m_Registry.view<ScriptComponent>();

		for (auto entityHandle : view) {
			auto entity = Entity(entityHandle, s_currentScene.get());
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			scriptComponent.ManagedScriptObject.Destroy();
		}
	}

	void ScriptEngine::LoadHostInstance()
	{
		Coral::HostSettings hostSettings;
		hostSettings.CoralDirectory = s_coralDir.string();
		hostSettings.ExceptionCallback = ExceptionCallback;

		Coral::CoralInitStatus result = s_hostInstance.Initialize(hostSettings);

		switch (result) 
		{
		case Coral::CoralInitStatus::DotNetNotFound:
			NG_CORE_LOG_CRITICAL("[Script Engine] Dotnet not found! Ensure you have dotnet 9.0 or later installed");
			std::exit(-1);
		case Coral::CoralInitStatus::CoralManagedNotFound:
			NG_CORE_LOG_CRITICAL("[Script Engine] Coral not found! Ensure the assemblies exist or are in the right directory!");
			std::exit(-1);
		case Coral::CoralInitStatus::CoralManagedInitError:
			NG_CORE_LOG_CRITICAL("[Script Engine] Coral initializer error!");
			std::exit(-1);
		case Coral::CoralInitStatus::Success:
			NG_CORE_LOG_INFO("[Script Engine] Loaded host instance!");
			break;
		}
	}

	void ScriptEngine::LoadCoreAssembly()
	{
		std::string contextName = "CoreContext";
		s_loadContext = s_hostInstance.CreateAssemblyLoadContext(contextName);

		std::filesystem::path assemblyPath = s_coralDir / "Nigozi-CSharpCore.dll";
		sp_coreAssembly = &s_loadContext.LoadAssembly(assemblyPath.string());

		ScriptGlue::RegisterGlue(*sp_coreAssembly);

		// std::vector<std::string> typeNames{
		// 	"Nigozi.Node",
		// 	"Nigozi.SceneNode",
		// 	"Nigozi.CameraNode",
		// 	"Nigozi.SpriteNode",
		// 	"Nigozi.Vector2",
		// 	"Nigozi.IVector2",
		// 	"Nigozi.Transform",
		// 	"Nigozi.SceneTree",
		// };
		// 
		// for (const auto& typeName : typeNames) {
		// 	auto& type = sp_coreAssembly->.GetLocalType(typeName);
		// 	NG_CORE_LOG_INFO("Scanning Type: {}", typeName);
		// 	NG_CORE_LOG_INFO("{}", std::string(type.GetFullName()));
		// 
		// 	auto fields = type.GetFields();
		// 	for (const auto& field : fields) {
		// 		NG_CORE_LOG_INFO("{0}, {1}", std::string(field.GetName()), (uint32_t)field.GetAccessibility());
		// 	}
		// 
		// 	auto properties = type.GetProperties();
		// 	for (const auto& property : properties) {
		// 		NG_CORE_LOG_INFO("{0}", std::string(property.GetName()));
		// 	}
		// 
		// 	NG_CORE_LOG_INFO("Base Type: {}", std::string(type.GetBaseType().GetFullName()));
		// 	
		// 	auto methods = type.GetMethods();
		// 	for (const auto& method : methods) {
		// 		NG_CORE_LOG_INFO("Method: {}", std::string(method.GetName()));
		// 	}
		// 
		// 	auto interfaces = type.GetInterfaceTypes();
		// 	for (const auto& interface : interfaces) {
		// 		NG_CORE_LOG_INFO("Interface {}", std::string(interface->GetFullName()));
		// 	}
		// }
		// 
		// // Get a reference to the ExampleClass type
		// auto& exampleType = sp_coreAssembly->GetLocalType("Nigozi.Test");
		// 
		// auto fields = exampleType.GetFields();
		// auto properties = exampleType.GetProperties();
		// 
		// // Call the static method "StaticMethod" with value 50
		// Coral::String msg = Coral::String::New("Message from engine");
		// exampleType.InvokeStaticMethod("StaticLogFromCpp", msg);
		// Coral::String::Free(msg);
		// 
		// s_currentScene = std::make_shared<SceneTree>();
		// s_currentScene->CreateEntity("Scene Root", "Empty");
		// 
		// // Create an instance of type Example.Managed.ExampleClass and pass 50 to the constructor
		// auto exampleInstance = exampleType.CreateInstance();
		// 
		// // Invoke the method named "MemberMethod" with a MyVec3 argument (doesn't return anything)
		// exampleInstance.InvokeMethod("LogFromCpp");
		// exampleInstance.InvokeMethod("LogTest");
		// 
		// exampleInstance.Destroy();
		// 
		// s_currentScene->ClearSceneTree();
		// s_currentScene.reset();

		// auto& sceneTreeType = assembly.GetLocalType("Nigozi.SceneTree");

		// sceneTreeType.InvokeStaticMethod("Node CreateNode()");
	}

	void ScriptEngine::LoadProjectAssembly()
	{
		sp_projectAssembly = &s_loadContext.LoadAssembly((Project::s_ProjectAssemblyDir / (Project::s_ProjectName + ".dll")).string());
	}

	void ScriptEngine::UnloadCoreAssembly()
	{
		s_hostInstance.UnloadAssemblyLoadContext(s_loadContext);
		s_hostInstance.Shutdown();
	}
}