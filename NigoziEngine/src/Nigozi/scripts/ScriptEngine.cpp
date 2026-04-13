#include "ngpch.h"
#include "core/Log.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "scene/Entity.h"

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

namespace Nigozi
{
	std::shared_ptr<SceneTree> ScriptEngine::s_currentScene;

	void ScriptEngine::Initialize(const std::filesystem::path& coralDir)
	{
		LoadAssemblies(coralDir);
	}

	void ScriptEngine::Deinitialize()
	{
		UnloadAssemblies();
	}

	static void ExceptionCallback(std::string_view InMessage)
	{
		std::cout << "Unhandled native exception: " << InMessage << std::endl;
	}

	static Coral::HostInstance hostInstance;

	void ScriptEngine::LoadAssemblies(const std::filesystem::path& coralDir)
	{
		Coral::HostSettings hostSettings;
		hostSettings.CoralDirectory = coralDir.string();
		hostSettings.ExceptionCallback = ExceptionCallback;

		hostInstance.Initialize(hostSettings);

		std::string contextName = "ExampleContext";
		Coral::AssemblyLoadContext loadContext = hostInstance.CreateAssemblyLoadContext(contextName);

		std::filesystem::path assemblyPath = coralDir / "Nigozi-CSharpCore.dll";
		Coral::ManagedAssembly& assembly = loadContext.LoadAssembly(assemblyPath.string());

		ScriptGlue::RegisterGlue(assembly);

		std::vector<std::string> typeNames{
			"Nigozi.Node",
			"Nigozi.SceneNode",
			"Nigozi.CameraNode",
			"Nigozi.SpriteNode",
			"Nigozi.Vector2",
			"Nigozi.IVector2",
			"Nigozi.Transform",
			"Nigozi.SceneTree",
		};

		for (const auto& typeName : typeNames) {
			auto& type = assembly.GetLocalType(typeName);
			NG_CORE_LOG_INFO("Scanning Type: {}", typeName);
			NG_CORE_LOG_INFO("{}", std::string(type.GetFullName()));

			auto fields = type.GetFields();
			for (const auto& field : fields) {
				NG_CORE_LOG_INFO("{0}, {1}", std::string(field.GetName()), (uint32_t)field.GetAccessibility());
			}

			auto properties = type.GetProperties();
			for (const auto& property : properties) {
				NG_CORE_LOG_INFO("{0}", std::string(property.GetName()));
			}

			NG_CORE_LOG_INFO("Base Type: {}", std::string(type.GetBaseType().GetFullName()));
			
			auto methods = type.GetMethods();
			for (const auto& method : methods) {
				NG_CORE_LOG_INFO("Method: {}", std::string(method.GetName()));
			}

			auto interfaces = type.GetInterfaceTypes();
			for (const auto& interface : interfaces) {
				NG_CORE_LOG_INFO("Interface {}", std::string(interface->GetFullName()));
			}
		}

		// Get a reference to the ExampleClass type
		auto& exampleType = assembly.GetLocalType("Nigozi.Test");

		auto fields = exampleType.GetFields();
		auto properties = exampleType.GetProperties();

		// Call the static method "StaticMethod" with value 50
		Coral::String msg = Coral::String::New("Message from engine");
		exampleType.InvokeStaticMethod("StaticLogFromCpp", msg);
		Coral::String::Free(msg);

		s_currentScene = std::make_shared<SceneTree>();
		s_currentScene->CreateEntity("Scene Root", "Empty");

		// Create an instance of type Example.Managed.ExampleClass and pass 50 to the constructor
		auto exampleInstance = exampleType.CreateInstance();

		// Invoke the method named "MemberMethod" with a MyVec3 argument (doesn't return anything)
		exampleInstance.InvokeMethod("LogFromCpp");
		exampleInstance.InvokeMethod("LogTest");

		exampleInstance.Destroy();

		s_currentScene->ClearSceneTree();
		s_currentScene.reset();

		// auto& sceneTreeType = assembly.GetLocalType("Nigozi.SceneTree");

		// sceneTreeType.InvokeStaticMethod("Node CreateNode()");
	}

	void ScriptEngine::UnloadAssemblies()
	{
		hostInstance.Shutdown();
	}
}