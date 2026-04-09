#pragma once
#include "ngpch.h"

#include <xhash>

namespace Nigozi 
{
	class UUID
	{
	public:
		static constexpr uint64_t Null = 0;
	public:
		UUID();
		UUID(uint64_t uuid);
		~UUID() = default;

		inline uint64_t GetUUID() const { return m_UUID; }

		UUID& operator=(uint64_t otherUUID) {
			if (m_UUID != otherUUID) {
				this->m_UUID = otherUUID;
			}

			return *this;
		}

		bool operator==(UUID other) {
			return other.m_UUID == m_UUID;
		}

		bool operator!=(UUID other) {
			return other.m_UUID != m_UUID;
		}

		operator uint64_t() { return m_UUID; }
		operator const uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Nigozi::UUID>
	{
		std::size_t operator()(const Nigozi::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
