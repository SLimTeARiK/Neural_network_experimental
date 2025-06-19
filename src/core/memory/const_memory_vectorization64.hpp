

#pragma once
#include <cstdlib>
#include <cstring>
#include <utility>
#include <type_traits>
#include <iostream>
#include <memory>

#ifdef _DEBUG
#define BOUNDS_CHECK(index, size) \
    if ((index) >= (size)) { \
        std::cerr << "Bounds violation in " << __FILE__ << ":" << __LINE__ \
                  << " - index: " << (index) << ", size: " << (size) << "\n"; \
        std::abort(); \
    }
#else
#define BOUNDS_CHECK(index, size)
#endif



namespace aligned {
enum class FLAG {
    COPY = 0,          // Копирование данных
    MOVE = 1 << 0,          // Передача владения (std::move)
    VIEW = 1 << 1,          // Передача указателя на часть массива (без владения)
    UNIQUE = 1 << 2    // Передача с гарантией уникальности (только одна копия) пока нереализованно
};

template <typename T>
struct info_align64_memory{
T* const ptr;
uint64_t size;
FLAG flags;
};

// Операторы для работы с флагами
constexpr FLAG operator|(FLAG a, FLAG b) {
    return static_cast<FLAG>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr FLAG operator&(FLAG a, FLAG b) {
    return static_cast<FLAG>(static_cast<int>(a) & static_cast<int>(b));
}

constexpr FLAG operator~(FLAG a) {
    return static_cast<FLAG>(~static_cast<int>(a));
}

constexpr bool any(FLAG flags, FLAG test) {
    return (flags & test) != FLAG::COPY;
}


template <typename T>
class const_align64_memory {
    static_assert(std::is_standard_layout<T>::value,
                 "const_align64_memory only supports standard layout types");

    T* const ptr;
    uint64_t size_;
    static constexpr uint64_t alignment = 64;
    FLAG flag;

public:
    // Конструкторы
    explicit const_align64_memory(uint64_t size) noexcept : size_(size), flag(FLAG::UNIQUE), ptr(static_cast<T*>(allocate<T>(size * sizeof(T), alignment))) {
        if (!ptr) std::abort();
    }
    explicit const_align64_memory(const info_align64_memory<T> info) noexcept :  size_(info.size),
                                                                        flag(info.flags),
                                                                        ptr((is_aligned(info.ptr, alignment) && !(info.flags & FLAG::UNIQUE)) ? static_cast<T*>(info.ptr) : static_cast<T*>(allocate<T>(size * sizeof(T), alignment))) {
        if (!ptr) {
            std::cout << "The ptr is 0";
            std::abort();
        }
        if(((info.flags & FLAG::UNIQUE)&&(info.flags & FLAG::VIEW))||((info.flags & FLAG::MOVE)&&(info.flags & FLAG::COPY))){
            std::cout << "The combination of flags is not supported.";
            std::abort();
        }
        if(is_aligned(info.ptr, alignment) || info.flags & FLAG::COPY)
            std::copy(ptr, &ptr[size_], info.ptr);

        if(info.flags & FLAG::UNIQUE)
        deallocate(info.ptr);
    }
    ~const_align64_memory() noexcept {
        if (ptr && (flag & FLAG::UNIQUE)) {
            deallocate(ptr);
        }
    }

    // Доступ к данным
    T& operator[](uint64_t index) noexcept {
        BOUNDS_CHECK(index, size_);
        return ptr[index];
    }

    const T& operator[](uint64_t index) const noexcept {
        BOUNDS_CHECK(index, size_);
        return ptr[index];
    }

    // Получение указателей
    T* data() noexcept { return ptr; }
    T* const data() const noexcept { return ptr; }

    // Размер
    uint64_t size() const noexcept { return size_; }

    // Передача данных (move-семантика)
    const_align64_memory release_full() noexcept {
        const_align64_memory result{ptr, size_, FLAG::MOVE | FLAG::UNIQUE};
        return result;
    }

    const_align64_memory release_range(uint64_t start, uint64_t count) noexcept {
        BOUNDS_CHECK(start + count - 1, size_);
        const_align64_memory result{ptr + start, count, FLAG::VIEW};
        return result;
    }

    // Передача данных (без move-семантики)
    void copy_full(T* dest) const noexcept {
        std::memcpy(dest, ptr, size_ * sizeof(T));
    }

    void copy_range(T* dest, uint64_t start, uint64_t count) const noexcept {
        BOUNDS_CHECK(start + count - 1, size_);
        std::memcpy(dest, ptr + start, count * sizeof(T));
    }

    // Выравнивание
    static constexpr uint64_t alignment_value() noexcept { return alignment; }

};
}
