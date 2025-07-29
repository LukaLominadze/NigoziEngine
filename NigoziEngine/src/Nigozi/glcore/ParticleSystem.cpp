#include "ngpch.h"
#include "ParticleSystem.h"
#include "Renderer2D.h"

namespace Nigozi
{
    std::mt19937 Random::s_randomEngine;
    std::uniform_int_distribution< std::mt19937::result_type> Random::s_distribution;

    void Random::Init()
    {
        s_randomEngine.seed(std::random_device()());
    }

    float Random::Float()
    {
        return (float)s_distribution(s_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
    }

    ParticleSystem::ParticleSystem()
    {
        m_particlePool.resize(1000);
    }

    void ParticleSystem::OnUpdate(float timestep)
    {
        for (Particle& particle : m_particlePool) {
            if (!particle.Active) {
                continue;
            }

            if (particle.LifeTimeRemaining <= 0.0f) {
                particle.Active = false;
                continue;
            }

            particle.LifeTimeRemaining -= timestep;
            particle.Position += particle.Velocity * timestep;
            particle.Rotation += 0.01f * timestep;

            float lifeTimeNormal = (particle.LifeTimeRemaining / particle.LifeTime);

            float size = particle.SizeEnd + lifeTimeNormal * (particle.SizeBegin - particle.SizeEnd);
            glm::vec4 color = {
                particle.ColorEnd.x + lifeTimeNormal * (particle.ColorBegin.x - particle.ColorEnd.x),
                particle.ColorEnd.y + lifeTimeNormal * (particle.ColorBegin.y - particle.ColorEnd.y),
                particle.ColorEnd.z + lifeTimeNormal * (particle.ColorBegin.z - particle.ColorEnd.z),
                particle.ColorEnd.w + lifeTimeNormal * (particle.ColorBegin.w - particle.ColorEnd.w),
            };

            particle.Color = color;
            particle.Size = size;
        }
    }

    void ParticleSystem::OnRender()
    {
        for (Particle& particle : m_particlePool) {
            if (!particle.Active) {
                continue;
            }
            Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y },
                                        { particle.Size, particle.Size },
                                        particle.Rotation, nullptr, particle.Color);
        }
    }

    void ParticleSystem::Emit(const ParticleProps& props)
    {
        Particle& particle = m_particlePool[m_poolIndex];
        particle.Position = props.Position;
        particle.Rotation = glm::degrees(Random::Float() * 2.0f * glm::pi<float>());

        particle.Velocity = props.Velocity;
        particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
        particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);
        particle.Velocity.x *= (Random::Float() - 0.5f);
        particle.Velocity.y *= (Random::Float() - 0.5f);

        particle.ColorBegin = props.ColorBegin;
        particle.ColorEnd = props.ColorEnd;

        particle.LifeTime = props.LifeTime;
        particle.LifeTimeRemaining = particle.LifeTime;

        particle.SizeBegin = props.SizeBegin;
        particle.SizeEnd = props.SizeEnd;

        particle.Active = true;

        if (--m_poolIndex == -1) {
            m_poolIndex = m_particlePool.size() - 1;
        }
    }
}
