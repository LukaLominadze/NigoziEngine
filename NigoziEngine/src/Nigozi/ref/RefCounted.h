#pragma once

// This is what holds the ref counted object
template<typename T>
struct RefCounted
{
    uint32_t RefCount = 1;
    T Obj;

    template<typename... Args>
    RefCounted(Args&&... args)
        :Obj(std::forward<Args>(args)...)
    {
    }
    RefCounted(const RefCounted&) = delete;
    RefCounted(RefCounted&& other) noexcept = default;

    RefCounted<T>& operator=(RefCounted<T>&& other) noexcept {
        RefCount = other.RefCount;
        Obj = std::move(other.Obj);
        return *this;
    }
};