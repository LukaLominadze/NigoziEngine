#pragma once

#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>
#include <Coral/Assembly.hpp>

namespace Nigozi
{
	static class ScriptGlue
	{
	public:
		static void RegisterGlue(Coral::ManagedAssembly& coreAssembly);
	private:
		static void RegisterInternalCalls(Coral::ManagedAssembly& coreAssembly);
	};

	namespace InternalCalls
	{
#pragma region Log
		void Log_Info(Coral::String msgIn);
		void Log_Warn(Coral::String msgIn);
		void Log_Error(Coral::String msgIn);
		void Log_Critical(Coral::String msgIn);
#pragma endregion

#pragma region SceneTree
		uint64_t SceneTree_CreateNode(uint16_t nodeType);
		uint64_t SceneTree_GetParent(uint64_t childId);
		bool SceneTree_SetParent(uint64_t childId, uint64_t newParentId);
		uint64_t SceneTree_GetChildByIndex(uint64_t id, int32_t index);
		uint16_t SceneTree_GetNodeType(uint64_t id);
		void SceneTree_DestroyNode(uint64_t id);
#pragma endregion

#pragma region Input
		bool Input_IsKeyPressed(int32_t inKeyCode);
		bool Input_IsKeyJustPressed(int32_t inKeyCode);
		bool Input_IsKeyReleased(int32_t inKeyCode);
		bool Input_IsKeyJustReleased(int32_t inKeyCode);

		bool Input_IsMouseButtonPressed(int32_t inButton);
		bool Input_IsMouseButtonJustPressed(int32_t inButton);
		bool Input_IsMouseButtonReleased(int32_t inButton);
		bool Input_IsMouseButtonJustReleased(int32_t inButton);

		float Input_GetAxis(int32_t inNegativeKeyCode, int32_t inPositiveKeyCode);
		glm::vec2 Input_GetVec2Axis(glm::i32vec2 inXKeyCodes, glm::i32vec2 inYKeyCodes);
#pragma endregion

#pragma region NameComponent
		Coral::String NameComponent_GetName(uint64_t id);
		void NameComponent_SetName(uint64_t id, Coral::String inName);
#pragma endregion

#pragma region TagComponent
		Coral::String TagComponent_GetTag(uint64_t id);
		void TagComponent_SetTag(uint64_t id, Coral::String inTag);
#pragma endregion

#pragma region TransformComponent
		struct Transform 
		{
			glm::vec2 Position;
			glm::vec2 Scale;
			float Rotation;
		};

		void TransformComponent_GetTransform(uint64_t id, Transform* outTransform);
		void TransformComponent_SetTransform(uint64_t id, Transform* inTransform);
		void TransformComponent_GetWorldTransform(uint64_t id, Transform* outTransform);
		void TransformComponent_SetWorldTransform(uint64_t id, Transform* inTransform);
#pragma endregion

#pragma region CameraComponent
		float CameraComponent_GetZoom(uint64_t id);
		float CameraComponent_GetAspect(uint64_t id);
		bool CameraComponent_GetCurrent(uint64_t id);
		void CameraComponent_SetZoom(uint64_t id, float inZoom);
		void CameraComponent_SetAspect(uint64_t id, float inAspect);
		void CameraComponent_SetCurrent(uint64_t id, bool inCurrent);
#pragma endregion

#pragma region SpriteRendererComponent
		glm::i32vec2 SpriteRendererComponent_GetSlot(uint64_t id);
		void SpriteRendererComponent_SetSlot(uint64_t id, glm::i32vec2 inSlot);
		glm::i32vec2 SpriteRendererComponent_GetSeperator(uint64_t id);
		void SpriteRendererComponent_SetSeperator(uint64_t id, glm::i32vec2 inSeperator);
		glm::vec4 SpriteRendererComponent_GetColor(uint64_t id);
		void SpriteRendererComponent_SetColor(uint64_t id, glm::vec4 inColor);
#pragma endregion

#pragma region RigidbodyComponent
		uint16_t RigidbodyComponent_GetBodyType(uint64_t id);
		void RigidbodyComponent_SetBodyType(uint64_t id, uint16_t inBodyType);
		bool RigidbodyComponent_GetFreezeRotation(uint64_t id);
		void RigidbodyComponent_SetFreezeRotation(uint64_t id, bool inFreezeRotation);
		void RigidbodyComponent_ApplyForce(uint64_t id, glm::vec2 inForce);
		void RigidbodyComponent_ApplyImpulse(uint64_t id, glm::vec2 inImpulse);
#pragma endregion

#pragma region BoxColliderComponent
		glm::vec2 BoxColliderComponent_GetSize(uint64_t id);
		void BoxColliderComponent_SetSize(uint64_t id, glm::vec2 inSize);
		glm::vec2 BoxColliderComponent_GetOffset(uint64_t id);
		void BoxColliderComponent_SetOffset(uint64_t id, glm::vec2 inOffset);
#pragma endregion

#pragma region AudioStreamPlayer
		float AudioStreamPlayerComponent_GetVolume(uint64_t id);
		void AudioStreamPlayerComponent_SetVolume(uint64_t id, float inVolume);
		bool AudioStreamPlayerComponent_GetPlaying(uint64_t id);
		void AudioStreamPlayerComponent_SetPlaying(uint64_t id, bool inPlaying);
		bool AudioStreamPlayerComponent_GetIsPaused(uint64_t id);
		void AudioStreamPlayerComponent_SetIsPaused(uint64_t id, bool inPaused);
		void AudioStreamPlayerComponent_Stop(uint64_t id);
#pragma endregion
	}
}

