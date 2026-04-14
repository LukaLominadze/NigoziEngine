#include "ngpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "scene/SceneTree.h"
#include "scene/Component.h"
#include "core/Log.h"
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Nigozi
{
	void ScriptGlue::RegisterGlue(Coral::ManagedAssembly& coreAssembly)
	{
		RegisterInternalCalls(coreAssembly);
	}

	void ScriptGlue::RegisterInternalCalls(Coral::ManagedAssembly& coreAssembly)
	{
		using namespace InternalCalls;

#define NG_REGISTER_INTERNAL_CALL(func) coreAssembly.AddInternalCall("Nigozi.InternalCalls", #func, reinterpret_cast<void*>(&func))
		NG_REGISTER_INTERNAL_CALL(Log_Info);
		NG_REGISTER_INTERNAL_CALL(Log_Warn);
		NG_REGISTER_INTERNAL_CALL(Log_Error);
		NG_REGISTER_INTERNAL_CALL(Log_Critical);

		NG_REGISTER_INTERNAL_CALL(SceneTree_CreateNode);
		NG_REGISTER_INTERNAL_CALL(SceneTree_DestroyNode);

		NG_REGISTER_INTERNAL_CALL(NameComponent_GetName);
		NG_REGISTER_INTERNAL_CALL(NameComponent_SetName);

		NG_REGISTER_INTERNAL_CALL(TagComponent_GetTag);
		NG_REGISTER_INTERNAL_CALL(TagComponent_SetTag);

		NG_REGISTER_INTERNAL_CALL(TransformComponent_GetTransform);
		NG_REGISTER_INTERNAL_CALL(TransformComponent_SetTransform);
		NG_REGISTER_INTERNAL_CALL(TransformComponent_GetWorldTransform);
		NG_REGISTER_INTERNAL_CALL(TransformComponent_SetWorldTransform);

		NG_REGISTER_INTERNAL_CALL(CameraComponent_GetZoom);
		NG_REGISTER_INTERNAL_CALL(CameraComponent_GetAspect);
		NG_REGISTER_INTERNAL_CALL(CameraComponent_GetCurrent);
		NG_REGISTER_INTERNAL_CALL(CameraComponent_SetZoom);
		NG_REGISTER_INTERNAL_CALL(CameraComponent_SetAspect);
		NG_REGISTER_INTERNAL_CALL(CameraComponent_SetCurrent);

		NG_REGISTER_INTERNAL_CALL(SpriteRendererComponent_GetSlot);
		NG_REGISTER_INTERNAL_CALL(SpriteRendererComponent_SetSlot);
		NG_REGISTER_INTERNAL_CALL(SpriteRendererComponent_GetSeperator);
		NG_REGISTER_INTERNAL_CALL(SpriteRendererComponent_SetSeperator);

		NG_REGISTER_INTERNAL_CALL(RigidbodyComponent_GetBodyType);
		NG_REGISTER_INTERNAL_CALL(RigidbodyComponent_SetBodyType);
		NG_REGISTER_INTERNAL_CALL(RigidbodyComponent_GetFreezeRotation);
		NG_REGISTER_INTERNAL_CALL(RigidbodyComponent_SetFreezeRotation);

		NG_REGISTER_INTERNAL_CALL(BoxColliderComponent_GetSize);
		NG_REGISTER_INTERNAL_CALL(BoxColliderComponent_SetSize);
		NG_REGISTER_INTERNAL_CALL(BoxColliderComponent_GetOffset);
		NG_REGISTER_INTERNAL_CALL(BoxColliderComponent_SetOffset);

		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_GetVolume);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_SetVolume);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_GetPlaying);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_SetPlaying);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_GetIsPaused);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_SetIsPaused);
		NG_REGISTER_INTERNAL_CALL(AudioStreamPlayerComponent_Stop);

		coreAssembly.UploadInternalCalls();
	}

	namespace InternalCalls
	{
		using namespace Nigozi;

		void Log_Info(Coral::String msgIn)
		{
			std::string msg(msgIn);
			NG_CORE_LOG_INFO(msg);
		}

		void Log_Warn(Coral::String msgIn)
		{
			std::string msg(msgIn);
			NG_CORE_LOG_WARN(msg);
		}

		void Log_Error(Coral::String msgIn)
		{
			std::string msg(msgIn);
			NG_CORE_LOG_ERROR(msg);
		}

		void Log_Critical(Coral::String msgIn)
		{
			std::string msg(msgIn);
			NG_CORE_LOG_CRITICAL(msg);
		}

		uint64_t SceneTree_CreateNode(uint16_t nodeType)
		{
			NG_CORE_LOG_INFO("Callback creating node {}", nodeType);
			NodeTypeComponent::Types type = (NodeTypeComponent::Types)nodeType;

			Entity entity = ScriptEngine::GetCurrentSceneTree()->CreateEntity("New Entity", "Empty");

			entity.AddComponent<NodeTypeComponent>().Type = type;

			if (type == NodeTypeComponent::Types::Camera) {
				entity.AddComponent<CameraComponent>();
			}
			else if (type == NodeTypeComponent::Types::SpriteRenderer) {
				entity.AddComponent<SpriteRendererComponent>();
			}

			return entity.GetUUID().GetUUID();
		}

		void SceneTree_DestroyNode(uint64_t id)
		{
			NG_CORE_LOG_INFO("Callback destroying node {}", id);
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity != Entity()) {
				entity.Destroy();
			}
		}

		Coral::String NameComponent_GetName(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return Coral::String::New("");
			}

			auto& name = entity.GetComponent<NameComponent>();
			return Coral::String::New(name.Name);
		}

		void NameComponent_SetName(uint64_t id, Coral::String inName)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& name = entity.GetComponent<NameComponent>();
			name.Name = std::string(inName);
		}

		Coral::String TagComponent_GetTag(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return Coral::String::New("");
			}

			auto& tag = entity.GetComponent<TagComponent>();
			return Coral::String::New(tag.Tag);
		}

		void TagComponent_SetTag(uint64_t id, Coral::String inTag)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& tag = entity.GetComponent<TagComponent>();
			tag.Tag = std::string(inTag);
		}

		static void UpdateRigidbodyTransform(Entity entity, const std::shared_ptr<SceneTree> m_currentContext) {
			if (!entity.HasComponent<RigidbodyComponent>()) {
				return;
			}
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			b2Body* body = (b2Body*)rigidbody.RuntimeBody;
			auto rTransform = body->GetTransform();
			auto worldTransform = m_currentContext->GetWorldSpaceTransform(entity);
			rTransform.p.x = worldTransform.Position.x;
			rTransform.p.y = worldTransform.Position.y;
			body->SetTransform(rTransform.p, glm::radians(-worldTransform.Rotation));
			body->SetAwake(true);
		}

		static void UpdateBoxCollider(Entity entity, const std::shared_ptr<SceneTree> m_currentContext, glm::vec2 size)
		{
			auto& boxCollider = entity.GetComponent<Nigozi::BoxColliderComponent>();
			b2Fixture* oldFixture = (b2Fixture*)boxCollider.RuntimeFixture;

			auto& rigidbody = entity.GetComponent<Nigozi::RigidbodyComponent>();
			b2Body* body = (b2Body*)rigidbody.RuntimeBody;

			float density = oldFixture->GetDensity();
			float friction = oldFixture->GetFriction();
			float restitution = oldFixture->GetRestitution();
			bool isSensor = oldFixture->IsSensor();

			body->DestroyFixture(oldFixture);

			b2PolygonShape newShape;
			newShape.SetAsBox(size.x, size.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &newShape;
			fixtureDef.density = boxCollider.Density;
			fixtureDef.friction = boxCollider.Friction;
			fixtureDef.restitution = boxCollider.Restitution;
			fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;

			boxCollider.RuntimeFixture = (void*)body->CreateFixture(&fixtureDef);

			body->ResetMassData();
		}

		void TransformComponent_GetTransform(uint64_t id, Transform* outTransform)
		{
			// NG_CORE_LOG_INFO("Transform callback get! {}: {}, {}", id, outTransform->Position.x, outTransform->Position.y);
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& transform = entity.GetComponent<TransformComponent>();
			outTransform->Position = transform.Position;
			outTransform->Scale = transform.Scale;
			outTransform->Rotation = transform.Rotation;
		}

		void TransformComponent_SetTransform(uint64_t id, Transform* inTransform)
		{
			// NG_CORE_LOG_INFO("Transform callback set! {}: {}, {}", id, inTransform->Position.x, inTransform->Position.y);
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& transform = entity.GetComponent<TransformComponent>();
			transform.Position = inTransform->Position;
			transform.Scale = inTransform->Scale;
			transform.Rotation = inTransform->Rotation;
			UpdateRigidbodyTransform(entity, ScriptEngine::GetCurrentSceneTree());
		}

		void TransformComponent_GetWorldTransform(uint64_t id, Transform* outTransform)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto sceneTree = ScriptEngine::GetCurrentSceneTree();
			auto worldTransform = sceneTree->GetWorldSpaceTransform(entity);

			outTransform->Position = worldTransform.Position;
			outTransform->Scale = worldTransform.Scale;
			outTransform->Rotation = worldTransform.Rotation;
		}

		void TransformComponent_SetWorldTransform(uint64_t id, Transform* inTransform)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto sceneTree = ScriptEngine::GetCurrentSceneTree();
			auto worldTransform = sceneTree->GetWorldSpaceTransform(entity);

			Transform delta;
			delta.Position = inTransform->Position - worldTransform.Position;
			delta.Scale = inTransform->Scale - worldTransform.Scale;
			delta.Rotation = inTransform->Rotation - worldTransform.Rotation;

			auto& transform = entity.GetComponent<TransformComponent>();
			transform.Position += delta.Position;
			transform.Scale += delta.Scale;
			transform.Rotation += delta.Rotation;
			UpdateRigidbodyTransform(entity, ScriptEngine::GetCurrentSceneTree());
		}

		float CameraComponent_GetZoom(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return 0.0f;
			}
			return entity.GetComponent<CameraComponent>().Zoom;
		}

		float CameraComponent_GetAspect(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return 0.0f;
			}
			return entity.GetComponent<CameraComponent>().Aspect;
		}

		bool CameraComponent_GetCurrent(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return false;
			}
			return entity.GetComponent<CameraComponent>().Current;
		}

		void CameraComponent_SetZoom(uint64_t id, float inZoom)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			entity.GetComponent<CameraComponent>().Zoom = inZoom;
		}

		void CameraComponent_SetAspect(uint64_t id, float inAspect)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			entity.GetComponent<CameraComponent>().Aspect = inAspect;
		}

		void CameraComponent_SetCurrent(uint64_t id, bool inCurrent)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& camera = entity.GetComponent<CameraComponent>();
			if (camera.Current) {
				return;
			}
			camera.Current = true;

			auto sceneTree = ScriptEngine::GetCurrentSceneTree();
			auto view = sceneTree->m_Registry.view<CameraComponent>();
			for (const auto entityHandle : view) {
				if ((uint64_t)entityHandle == id) {
					continue;
				}
				auto& otherCamera = Entity(entityHandle, sceneTree.get()).GetComponent<CameraComponent>();
				otherCamera.Current = false;
			}
		}

		glm::i32vec2 SpriteRendererComponent_GetSlot(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return glm::i32vec2();
			}

			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			return glm::i32vec2(sprite.Sprite.GetSlotX(), sprite.Sprite.GetSlotY());
		}

		void SpriteRendererComponent_SetSlot(uint64_t id, glm::i32vec2 inSlot)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			sprite.Sprite.SetSlot(inSlot.x, inSlot.y);
		}

		glm::i32vec2 SpriteRendererComponent_GetSeperator(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return glm::i32vec2();
			}

			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			glm::i32vec2 seperators = glm::i32vec2((glm::vec2)sprite.Sprite.GetTextureSize() / sprite.Sprite.GetSize());
			return seperators;
		}

		void SpriteRendererComponent_SetSeperator(uint64_t id, glm::i32vec2 inSeperator)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			glm::vec2 texSize = sprite.Sprite.GetTextureSize();
			glm::vec2 size(texSize.x / inSeperator.x, texSize.y / inSeperator.y);
			sprite.Sprite.SetSubTexture(size, sprite.Sprite.GetSlotX(), sprite.Sprite.GetSlotY());
		}

		uint16_t RigidbodyComponent_GetBodyType(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return 0;
			}

			return (uint16_t)entity.GetComponent<RigidbodyComponent>().Type;
		}

		void RigidbodyComponent_SetBodyType(uint64_t id, uint16_t inBodyType)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			rigidbody.Type = (RigidbodyComponent::BodyType)inBodyType;
			b2Body* body = (b2Body*)rigidbody.RuntimeBody;
			body->SetType((b2BodyType)rigidbody.Type);
			body->SetAwake(true);
		}

		bool RigidbodyComponent_GetFreezeRotation(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return false;
			}

			return entity.GetComponent<RigidbodyComponent>().FreezeRotation;
		}

		void RigidbodyComponent_SetFreezeRotation(uint64_t id, bool inFreezeRotation)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& rigidbody = entity.GetComponent<RigidbodyComponent>();
			rigidbody.FreezeRotation = inFreezeRotation;
			b2Body* body = (b2Body*)rigidbody.RuntimeBody;
			body->SetFixedRotation(rigidbody.FreezeRotation);
			body->SetAwake(true);
		}

		glm::vec2 BoxColliderComponent_GetSize(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return glm::vec2();
			}

			return entity.GetComponent<BoxColliderComponent>().Size;
		}

		void BoxColliderComponent_SetSize(uint64_t id, glm::vec2 inSize)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			entity.GetComponent<BoxColliderComponent>().Size = inSize;
			UpdateBoxCollider(entity, ScriptEngine::GetCurrentSceneTree(), inSize);
		}

		glm::vec2 BoxColliderComponent_GetOffset(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return glm::vec2();
			}

			return entity.GetComponent<BoxColliderComponent>().Offset;
		}

		void BoxColliderComponent_SetOffset(uint64_t id, glm::vec2 inOffset)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}

			auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
			boxCollider.Offset = inOffset;
			UpdateBoxCollider(entity, ScriptEngine::GetCurrentSceneTree(), boxCollider.Size);
		}

		float AudioStreamPlayerComponent_GetVolume(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return 0.0f;
			}

			return entity.GetComponent<AudioStreamPlayerComponent>().Volume;
		}

		void AudioStreamPlayerComponent_SetVolume(uint64_t id, float inVolume)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}
			auto& audio = entity.GetComponent<AudioStreamPlayerComponent>();
			audio.Volume = inVolume;
			audio.AudioHandle->SetVolume(audio.Volume);
		}

		bool AudioStreamPlayerComponent_GetPlaying(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return false;
			}

			return entity.GetComponent<AudioStreamPlayerComponent>().AudioHandle->IsPlaying();
		}

		void AudioStreamPlayerComponent_SetPlaying(uint64_t id, bool inPlaying)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}
			auto& audio = entity.GetComponent<AudioStreamPlayerComponent>();
			if (inPlaying) {
				if (audio.AudioHandle->IsPaused()) {
					audio.AudioHandle->Resume();
					return;
				}
				audio.AudioHandle->Play();
				return;
			}
			if (audio.AudioHandle->IsPlaying()) {
				audio.AudioHandle->Pause();
				return;
			}
		}

		bool AudioStreamPlayerComponent_GetIsPaused(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return false;
			}

			return entity.GetComponent<AudioStreamPlayerComponent>().AudioHandle->IsPaused();
		}

		void AudioStreamPlayerComponent_SetIsPaused(uint64_t id, bool inPaused)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}
			auto& audio = entity.GetComponent<AudioStreamPlayerComponent>();
			if (inPaused && audio.AudioHandle->IsPlaying()) {
				audio.AudioHandle->Pause();
				return;
			}
			if (audio.AudioHandle->IsPaused()) {
				audio.AudioHandle->Resume();
			}
		}

		void AudioStreamPlayerComponent_Stop(uint64_t id)
		{
			Entity entity = ScriptEngine::GetCurrentSceneTree()->TryGetEntityByUUID(UUID(id));
			if (entity == Entity()) {
				return;
			}
			auto& audio = entity.GetComponent<AudioStreamPlayerComponent>();
			if (audio.AudioHandle->IsPlaying() || audio.AudioHandle->IsPaused()) {
				audio.AudioHandle->Stop();
			}
		}
	}
}
