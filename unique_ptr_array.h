#pragma once

/**
 * @file unique_ptr_array.h
 * @description
 * @copyright
 * @author Levon Ghukasyan
 */

template <typename T, class Deleter>
class unique_ptr<T[], Deleter>
{
public:
    using pointer_type = T*;
    using value_type = T;
    /// @brief constructors and description
    /// @{
public:
    constexpr unique_ptr() noexcept
        : m_pointer(pointer_type())
        , m_deleter(Deleter())
    {
    }

    explicit unique_ptr(pointer_type ptr) noexcept
        : m_pointer(ptr)
        , m_deleter(Deleter())
    {
    }

    unique_ptr(pointer_type ptr, const Deleter& del) noexcept
        : m_deleter(del)
        , m_pointer(ptr)
    {
    }

    unique_ptr(unique_ptr&& ptr) noexcept
        : m_deleter(std::forward<Deleter>(ptr.get_deleter()))
        , m_pointer(ptr.release())
    {
    }

    template <class U, class E>
    unique_ptr(unique_ptr<U, E>&& u_ptr) noexcept
        : m_deleter(std::forward<Deleter>(u_ptr.get_deleter()))
        , m_pointer(u_ptr.release())
    {
    }

    ~unique_ptr() noexcept
    {
        reset();
    }
    /// @}
    
    /// @brief assignment operators
    /// @{
public:
    unique_ptr& operator= (unique_ptr&& r) noexcept
    {
        reset(r.release());
        get_deleter() = std::move(r.get_deleter());
        return *this;
    }

    template<class U, class E>
    unique_ptr& operator= (unique_ptr<U, E>&& r)
    {
        reset(r.release());
        get_deleter() = std::move(r.get_deleter());
        return *this;
    }

    unique_ptr& operator= (std::nullptr_t)
    {
        reset();
        return *this;
    }

    unique_ptr(const unique_ptr& other) = delete;

    template <class U, class E>
    unique_ptr(const unique_ptr<U, E>& other) = delete;

    template <class U, class E>
    unique_ptr& operator= (const unique_ptr<U, E>& other) = delete;
    /// @}
    
    /// @brief 
    /// @{
public:
    value_type operator[] (size_t idx) const
    {
        return get()[idx];
    }

    pointer_type get() const noexcept
    {
        return m_pointer;
    }

    Deleter get_deleter() noexcept
    {
        return m_deleter;
    }

    const Deleter& get_deleter() const noexcept
    {
        return m_deleter;
    }

    inline pointer_type release() noexcept
    {
        pointer_type ptr = get();
        m_pointer = nullptr;
        return ptr;
    }
    /// @}

public:
    void reset(pointer_type ptr = pointer_type())
    {
        if (ptr != get()) {
            get_deleter()(get());
            m_pointer = ptr;
        }
    }

    void swap(unique_ptr&& ptr)
    {
        std::swap(m_pointer, ptr.m_pointer);
        std::swap(m_deleter, ptr.m_deleter);
    }

private:
    pointer_type m_pointer;
    Deleter m_deleter;
};
