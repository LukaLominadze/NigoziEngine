#pragma once

#include "RefCounted.h"

#include <vector>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <new>
#include "core/Log.h"

// This is what manages ref counted objects
/*
    Note(Luka):
    This ref manager's purpose is to put all referenced objects inside a single, contigious, container.
*/
template<typename T>
class RefManager
{
public:
    static RefManager<T>& Get()
    {
        static RefManager<T> s_refManager;
        return s_refManager;
    }

    template<typename... Args>
    uint32_t Create(Args&&... args)
    {
        if (m_invalidatedRefs.size() > 0) {
            uint32_t id = *--m_invalidatedRefs.end();
            m_invalidatedRefs.pop_back();

            new (&m_refs[id]) RefCounted<T>(std::forward<Args>(args)...);
            // m_refs[id] = RefCounted<T>(std::forward<Args>(args)...);
            return id;
        }
        m_refs.emplace_back(std::forward<Args>(args)...);
        // NG_CORE_LOG_TRACE("[RefManager::{}] Create {}, {}", typeid(T).name(), m_refs.size(), m_invalidatedRefs.size());
        return m_refs.size() - 1;
    }

    void Increment(uint32_t id)
    {
        m_refs[id].RefCount++;
        // NG_CORE_LOG_TRACE("[RefManager::{}] Increment id: {}, refsSize: {}, invalidSize: {}, RefCount: {}", typeid(T).name(), id, m_refs.size(), m_invalidatedRefs.size(), m_refs[id].RefCount);
    }
    void Decrement(uint32_t id)
    {
        // NG_CORE_LOG_TRACE("[RefManager::{}] Decrement id: {}, refsSize: {}, invalidSize: {}, RefCount: {} - 1", typeid(T).name(), id, m_refs.size(), m_invalidatedRefs.size(), m_refs[id].RefCount);
        if (--m_refs[id].RefCount == 0) {
            m_invalidatedRefs.push_back(id);
            if (m_invalidatedRefs.size() == m_refs.size()) {
                m_refs.clear();
                m_invalidatedRefs.clear();
            }
            else {
                m_refs[id].Obj.~T();
            }
        }
    }
    T* GetObj(uint32_t id)
    {
        return &m_refs[id].Obj;
    }
private:
    std::vector<RefCounted<T>> m_refs;
    std::vector<uint32_t> m_invalidatedRefs;
};