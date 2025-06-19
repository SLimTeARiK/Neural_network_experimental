#pragma once
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>

#if defined(_WIN32)
    #include <malloc.h>
#endif


// Функции для работы с выровненной памятью
namespace aligned {
    // Внутренняя функция выделения памяти
    inline void* alloc_impl(uint64_t size, uint64_t alignment) noexcept {
        #if defined(_WIN32)
            #if defined(__MINGW32__) || defined(__MINGW64__)
                return __mingw_aligned_malloc(size, alignment);
            #else
                return _aligned_malloc(size, alignment);
            #endif
        #elif defined(__linux__) || defined(__APPLE__)
            void* ptr = nullptr;
            return posix_memalign(&ptr, alignment, size) == 0 ? ptr : nullptr;
        #else
        // Универсальная реализация без исключений
            if (alignment < sizeof(void*)) alignment = sizeof(void*);
            uint64_t extra = alignment - 1 + sizeof(void*);
            void* original = malloc(size + extra);
            if (!original) return nullptr;
            void* aligned = reinterpret_cast<void*>(
                (reinterpret_cast<uintptr_t>(original) + extra) & ~(alignment - 1)
            );
            reinterpret_cast<void**>(aligned)[-1] = original;
            return aligned;
        #endif
    }
    // Внутренняя функция выделения памяти
    inline void* alloc_impl(uint32_t size, uint32_t alignment) noexcept {
        #if defined(_WIN32)
            #if defined(__MINGW32__) || defined(__MINGW64__)
                return __mingw_aligned_malloc(size, alignment);
            #else
                return _aligned_malloc(size, alignment);
            #endif
        #elif defined(__linux__) || defined(__APPLE__)
            void* ptr = nullptr;
            return posix_memalign(&ptr, alignment, size) == 0 ? ptr : nullptr;
        #else
        // Универсальная реализация без исключений
            if (alignment < sizeof(void*)) alignment = sizeof(void*);
            uint32_t extra = alignment - 1 + sizeof(void*);
            void* original = malloc(size + extra);
            if (!original) return nullptr;
            void* aligned = reinterpret_cast<void*>(
                (reinterpret_cast<uintptr_t>(original) + extra) & ~(alignment - 1)
            );
            reinterpret_cast<void**>(aligned)[-1] = original;
            return aligned;
        #endif
    }

    inline void free_impl(void* ptr) noexcept {
        #if defined(_WIN32)
            #if defined(__MINGW32__) || defined(__MINGW64__)
                __mingw_aligned_free(ptr);
            #else
                _aligned_free(ptr);
            #endif
        #elif defined(__linux__) || defined(__APPLE__)
            free(ptr);
        #else
            if (ptr) {
                void* original = reinterpret_cast<void**>(ptr)[-1];
                free(original);
            }
        #endif
    }

    // Универсальная функция выделения памяти
    template <typename T>
    T* allocate(T count, T alignment = 64) noexcept {
    static_assert(std::is_standard_layout<T>::value,
                 "aligned::allocate only supports standard layout types");
        return static_cast<T*>(alloc_impl(count * sizeof(T), alignment));
    }

    // Основная функция освобождения
    template <typename T>
    void deallocate(T* const ptr) noexcept {
        if (!ptr) return;
        free_impl(const_cast<T*>(ptr));
    }
}

// Макросы для удобства
//#define my_aligned_malloc(size, alignment) ::aligned::alloc_impl(size, alignment)
//#define my_aligned_free(ptr) ::aligned::free_impl(ptr)

