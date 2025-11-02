

#include <utility>

//default Scopeptr

template<typename T>
class ScopePtr {
private:
    T* m_ptr;

public:
    ScopePtr()
        : m_ptr{nullptr} {}
    explicit ScopePtr(T* ptr) : m_ptr{ptr} {}
    ScopePtr(const ScopePtr&) = delete;
    ScopePtr& operator=(const ScopePtr&) = delete;
    ScopePtr(ScopePtr&& other)
        : m_ptr{std::exchange(other.m_ptr, nullptr)} {}
    
    ScopePtr& operator=(ScopePtr&& other) {
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }
    explicit operator bool() const { return nullptr == m_ptr; }
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    T* get() const { return m_ptr; }
    T* release() {
        auto old_ptr = m_ptr;
        m_ptr = nullptr;
        return old_ptr;
    }
    void reset(T* new_ptr) {
        delete m_ptr;
        m_ptr = new_ptr;
    }

    ~ScopePtr() { delete m_ptr; }
};

//Scopeptr for arrays

template<typename T>
class ScopePtr<T[]> {
private:
    T* m_ptr;

public:
    ScopePtr()
        : m_ptr{nullptr} {}
    explicit ScopePtr(T* ptr) : m_ptr{ptr} {}
    ScopePtr(const ScopePtr&) = delete;
    ScopePtr& operator=(const ScopePtr&) = delete;
    
    ScopePtr(ScopePtr&& other)
        : m_ptr{std::exchange(other.m_ptr, nullptr)} {}
    
    ScopePtr& operator=(ScopePtr&& other) {
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }
    explicit operator bool() const { return nullptr == m_ptr; }
    T& operator [](std::size_t index) const {return m_ptr[index];}
    T* get() const { return m_ptr; }
    T* release() {
        auto old_ptr = m_ptr;
        m_ptr = nullptr;
        return old_ptr;
    }
    void reset(T* new_ptr) {
        delete[] m_ptr;
        m_ptr = new_ptr;
    }

    ~ScopePtr() { delete [] m_ptr; }
};

//is_same

template<typename T, typename U>
struct is_same {
    static constexpr bool value = false;
};

template<typename T>
struct is_same<T, T> {
    static constexpr bool value = true;
};

//are_same

template<typename... Types>
struct are_same;

template<typename T>
struct are_same <T> {
    static constexpr bool value = true;
};

template<typename T, typename U>
struct are_same<T, U> {
    static constexpr bool value = is_same<T, U>::value;
};

template<typename T, typename U, typename... Rest>
struct are_same<T, U, Rest...> {
    static constexpr bool value = 
        is_same<T, U>::value && are_same<U, Rest...>::value;
};

