#pragma once
#include "core/Log.h"
#include "RefCounted.h"
#include "RefManager.h"

// This is the API for ref counted objects
/*
    Note(Luka):
    Any usage of this ref class will require the contained class
    to have a move constructor, that will "invalidate" the input instance
    to disable it's destructor behaviour

    For Ex.
    Test(Test&& other)
    {
        m_id = other.m_id
        other.m_id = -1; // Invalidate!
    }
    ~Test()
    {
        if (m_id != -1)
        {
            // Destruct!
        }
    }
*/
template<typename T>
class Ref
{
public:
    Ref<T>()
        :m_id(-1) 
    {}
    Ref<T>(uint32_t id)
        :m_id(id) 
    {}
    Ref<T>(const Ref<T>& other) 
    {
        m_id = other.m_id;
        if (m_id != -1) {
            RefManager<T>::Get().Increment(m_id);
        }
    }
    ~Ref<T>() 
    {
        if (m_id != -1) {
            RefManager<T>::Get().Decrement(m_id);
        }
    }

    template<typename... Args>
    static Ref<T> Create(Args&&... args) 
    {
        static_assert(std::is_move_constructible<T>::value, "Move constructor not found!");
        static_assert(std::is_nothrow_move_constructible<T>::value, "Move constructor with noexcept needed");
        uint32_t id = RefManager<T>::Get().Create(std::forward<Args>(args)...);
        return Ref<T>(id);
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
    bool operator==(Ref<T>& other) const noexcept { return m_id == other.m_id; }

    Ref<T>& operator=(Ref<T>&& other) noexcept {
        // NG_CORE_LOG_TRACE("[Ref::{}] Move assignment, this id: {}, other id: {}", typeid(T).name(), m_id, other.m_id);
        if (*this) {
            RefManager<T>::Get().Decrement(m_id);
        }
        m_id = other.m_id;
        other.m_id = -1;
        return *this;
    }
    Ref<T>& operator=(const Ref<T>& other) {
        // NG_CORE_LOG_TRACE("[Ref::{}] Copy assignment, this id: {}, other id: {}", typeid(T).name(), m_id, other.m_id);
        if (*this) {
            RefManager<T>::Get().Decrement(m_id);
        }
        m_id = other.m_id;
        RefManager<T>::Get().Increment(m_id);
        return *this;
    }
private:
    uint32_t m_id;
};
