# Определение компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMPILER_IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_IS_GCC TRUE)
endif()

# ================================================================
# ОБЩИЕ ФЛАГИ ДЛЯ ВСЕХ КОНФИГУРАЦИЙ И КОМПИЛЯТОРОВ
# ================================================================

# Специфичные флаги для Windows
set(WINDOWS_COMPILE_FLAGS
    -mavx                   # Включение AVX инструкций
    -mavx2                  # Включение AVX2 инструкций
    -fexceptions            # Включение обработки исключений
    -fdiagnostics-color=always # Цветные диагностические сообщения
)

# Флаги линковки для Windows
set(WINDOWS_LINK_FLAGS
    -lmingw32               # Библиотека MinGW
    -mwindows               # Создание GUI-приложения (не консольного)
    -static                 # Статическая линковка всех библиотек
    -static-libgcc          # Статическая линковка libgcc
    -static-libstdc++       # Статическая линковка libstdc++
    -lstdc++                # Явная линковка стандартной библиотеки C++
)

# Оптимизация на этапе линковки (LTO)
set(LTO_FLAGS -flto)

# ================================================================
# СПЕЦИФИЧНЫЕ ФЛАГИ ПО КОМПИЛЯТОРАМ
# ================================================================

# Диагностика и предупреждения
set(DIAGNOSTICS_GCC
    -Winvalid-pch           # Проверка прекомпилированных заголовков (GCC)
)

set(DIAGNOSTICS_CLANG
    -Winvalid-pch           # Проверка прекомпилированных заголовков (Clang)
)

# Отчеты об оптимизации
set(OPT_REPORT_GCC
    -fopt-info-vec          # Информация о векторизации
    -fopt-info-vec-missed   # Причины провала векторизации
    -fopt-info-vec-optimized# Отчет об успешной векторизации
    -ftime-report           # Время этапов компиляции (GCC)
)

set(OPT_REPORT_CLANG
    -Rpass=.*               # Отчет об успешных оптимизациях (Clang)
    -Rpass-missed=.*        # Отчет о пропущенных оптимизациях (Clang)
    -ftime-report           # Время этапов компиляции (Clang)
)

set(SUPPRESS_WARNINGS_CLANG
    -Wno-invalid-utf8
    -Wno-comment
)

# ================================================================
# КОНФИГУРАЦИЯ DEBUG (мин оптимизация + макс анализ)
# ================================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Базовые настройки
    set(OPTIMIZATION_DEBUG
        -O0                  # Без оптимизаций
    )

    set(DEBUG_INFO
        -g                   # Базовая отладочная информация
        -ggdb3               # Расширенная отладочная информация для GDB
        -fno-omit-frame-pointer # Сохранение указателя фрейма
    )

    # Диагностика
    set(DIAGNOSTICS
        -Wall                # Все стандартные предупреждения
        -Wextra              # Дополнительные предупреждения
        -pedantic            # Строгое соответствие стандарту
    )

    # Санитайзер
    set(SANITIZERS -fsanitize=address)

    # Сборка флагов в зависимости от компилятора
    if(COMPILER_IS_GCC)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_GCC}
            ${OPT_REPORT_GCC}
        )
    elseif(COMPILER_IS_CLANG)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_CLANG}
            ${OPT_REPORT_CLANG}
            ${SUPPRESS_WARNINGS_CLANG}
        )
    endif()

    # Итоговые флаги для Debug
    target_compile_options(neural_network_SDL PUBLIC
        ${OPTIMIZATION_DEBUG}
        ${DEBUG_INFO}
        ${DIAGNOSTICS}
        ${SUPPRESS_WARNINGS}
        ${COMPILER_SPECIFIC}
        ${SANITIZERS}
    )

# ================================================================
# КОНФИГУРАЦИЯ RELEASE (макс оптимизация + мин анализ)
# ================================================================
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Агрессивная оптимизация
    set(OPTIMIZATION_RELEASE
        -O3                  # Максимальная оптимизация скорости
        -march=native        # Использование инструкций процессора
        -DNDEBUG             # Отключение assert и отладочного кода
    )

    # Безопасность
    set(SECURITY
        -fstack-protector-strong # Защита от переполнения стека
    )

    target_compile_options(neural_network_SDL PUBLIC
        ${OPTIMIZATION_RELEASE}
        ${SECURITY}
    )

# ================================================================
# КОНФИГУРАЦИЯ TEST (макс оптимизация + макс анализ)
# ================================================================
elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
    # Гибридные настройки
    set(OPTIMIZATION_TEST
        -O3                  # Агрессивная оптимизация
        -march=native        # Использование инструкций процессора
        -DDEBUG              # Активация отладочного кода
    )

    set(DEBUG_INFO
        -g
        -ggdb3
        -fno-omit-frame-pointer
    )

    set(DIAGNOSTICS
        -Wall
        -Wextra
        -pedantic
    )

    set(SECURITY
        -fstack-protector-strong
    )

    # Сборка флагов в зависимости от компилятора
    if(COMPILER_IS_GCC)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_GCC}
            ${OPT_REPORT_GCC}
        )
    elseif(COMPILER_IS_CLANG)
        set(COMPILER_SPECIFIC
            ${DIAGNOSTICS_CLANG}
            ${OPT_REPORT_CLANG}
        )
    endif()

    target_compile_options(neural_network_SDL PUBLIC
        ${OPTIMIZATION_TEST}
        ${DEBUG_INFO}
        ${DIAGNOSTICS}
        ${SECURITY}
        ${COMPILER_SPECIFIC}
    )
endif()

# ================================================================
# ОБЩИЕ ФЛАГИ ДЛЯ ВСЕХ КОНФИГУРАЦИЙ
# ================================================================
target_compile_options(neural_network_SDL PUBLIC
    ${LTO_FLAGS}            # Оптимизация на этапе линковки
    ${WINDOWS_COMPILE_FLAGS}
)

target_link_options(neural_network_SDL PUBLIC
    ${WINDOWS_LINK_FLAGS}
)

# ================================================================
# ДОПОЛНИТЕЛЬНЫЕ НАСТРОЙКИ
# ================================================================
find_package(NUMA)
if(NUMA_FOUND)
    target_compile_definitions(neural_network_SDL PUBLIC -DHAVE_NUMA=1)
else()
    target_compile_definitions(neural_network_SDL PUBLIC -DHAVE_NUMA=0)
endif()
