#pragma once
#include "core/Log.h"
#include "RefCounted.h"
#include "RefManager.h"
#include "Ref.h"

// This is the API for ref counted objects
template<typename T>
class WeakRef
{
public:
    WeakRef<T>()
        :m_id(-1)
    {
    }
    WeakRef<T>(const Ref<T>& other)
    {
        m_id = other.m_id;
    }
    ~WeakRef<T>()
    {
    }

    T* Get()
    {
        if (m_id != -1) {
            return RefManager<T>::Get().GetObj(m_id);
        }
        return nullptr;
    }
    const T* Get() const noexcept
    {
        if (m_id != -1) {
            return RefManager<T>::Get().GetObj(m_id);
        }
        return nullptr;
    }

    T* operator->()
    {
        if (m_id != -1) {
            return RefManager<T>::Get().GetObj(m_id);
        }
        return nullptr;
    }
    const T* operator->() const noexcept
    {
        if (m_id != -1) {
            return RefManager<T>::Get().GetObj(m_id);
        }
        return nullptr;
    }

    operator bool() const noexcept { return m_id != -1; }
    bool operator==(WeakRef<T>& other) const noexcept { return m_id == other.m_id; }
    bool operator==(Ref<T>& other) const noexcept { return m_id == other.m_id; }

    WeakRef<T>& operator=(WeakRef<T>&& other) = delete;
    WeakRef<T>& operator=(const WeakRef<T>& other) = delete;
    WeakRef<T>& operator=(Ref<T>&& other) = delete;
    WeakRef<T>& operator=(const Ref<T>& other) {
        // NG_CORE_LOG_TRACE("[WeakRef::{}] Copy assignment, this id: {}, other id: {}", typeid(T).name(), m_id, other.m_id);
        m_id = other.m_id;
        return *this;
    }
private:
    uint32_t m_id;
};
