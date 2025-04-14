#pragma once
#include "ngpch.h"

#include <random>

namespace Nigozi
{
	class Random {
	public:
		static void Init();
		static float Float();
	private:
		static std::mt19937 s_randomEngine;
		static std::uniform_int_distribution< std::mt19937::result_type> s_distribution;
	};

	struct ParticleProps {
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem {
	public:
		ParticleSystem();

		void OnUpdate(float timestep);
		void OnRender();

		void Emit(const ParticleProps& props);
	private:
		struct Particle {
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation;
			glm::vec4 Color;
			float SizeBegin, SizeEnd, Size;
			float LifeTime, LifeTimeRemaining = 0.0f;

			bool Active = false;
		};

		std::vector<Particle> m_particlePool;
		uint32_t m_poolIndex = 999;
	};
}