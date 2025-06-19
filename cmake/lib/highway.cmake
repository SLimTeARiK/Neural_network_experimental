

# Highway configuration
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "Disable building examples to reduce compilation time")
set(HWY_ENABLE_TESTS OFF CACHE BOOL "Disable tests to reduce compilation time")
set(HWY_ENABLE_CONTRIB OFF CACHE BOOL "Disable contrib if not needed")
set(HWY_ENABLE_INSTALL OFF CACHE BOOL "Disable installation if using as subproject")

# Оптимизации и целевые архитектуры# ================================================================
# НАСТРОЙКИ ЦЕЛЕВОЙ АРХИТЕКТУРЫ И ИНСТРУКЦИЙ
# ================================================================

# Основные архитектуры
set(HWY_CMAKE_X86 ON CACHE BOOL "Enable x86/x64 architecture optimizations")
set(HWY_CMAKE_ARM OFF CACHE BOOL "Disable ARM architecture optimizations")
set(HWY_CMAKE_PPC OFF CACHE BOOL "Disable PowerPC optimizations")
set(HWY_CMAKE_WASM OFF CACHE BOOL "Disable WebAssembly optimizations")

# x86/x64 инструкции
set(HWY_CMAKE_SSE2 ON CACHE BOOL "Enable SSE2 instructions")
set(HWY_CMAKE_SSE3 ON CACHE BOOL "Enable SSE3 instructions")
set(HWY_CMAKE_SSSE3 ON CACHE BOOL "Enable SSSE3 instructions")
set(HWY_CMAKE_SSE4 ON CACHE BOOL "Enable SSE4.1/4.2 instructions")
set(HWY_CMAKE_AVX ON CACHE BOOL "Enable AVX instructions")
set(HWY_CMAKE_AVX2 ON CACHE BOOL "Enable AVX2 instructions")
set(HWY_CMAKE_AVX3 ON CACHE BOOL "Enable AVX-512 (AVX3) instructions")
set(HWY_CMAKE_AVX3_DL ON CACHE BOOL "Enable AVX-512 Double & Quad Precision")
set(HWY_CMAKE_AVX3_VNNI ON CACHE BOOL "Enable AVX-512 VNNI instructions")
set(HWY_CMAKE_AVX3_BF16 ON CACHE BOOL "Enable AVX-512 BF16 instructions")
set(HWY_CMAKE_FMA ON CACHE BOOL "Enable FMA3 instructions")
set(HWY_CMAKE_BMI2 ON CACHE BOOL "Enable BMI2 instructions")

# ARM инструкции
set(HWY_CMAKE_NEON OFF CACHE BOOL "Disable NEON instructions")
set(HWY_CMAKE_NEON_DOTPROD OFF CACHE BOOL "Disable NEON dot-product instructions")
set(HWY_CMAKE_NEON_FP16 OFF CACHE BOOL "Disable NEON half-precision float")
set(HWY_CMAKE_SVE OFF CACHE BOOL "Disable ARM SVE instructions")
set(HWY_CMAKE_SVE2 OFF CACHE BOOL "Disable ARM SVE2 instructions")
set(HWY_CMAKE_SVE_AES OFF CACHE BOOL "Disable ARM SVE AES instructions")

# PowerPC инструкции
set(HWY_CMAKE_VSX OFF CACHE BOOL "Disable PowerPC VSX instructions")
set(HWY_CMAKE_VSX3 OFF CACHE BOOL "Disable PowerPC VSX3 instructions")

# WebAssembly инструкции
set(HWY_CMAKE_WASM_SIMD OFF CACHE BOOL "Disable WASM SIMD instructions")

# ================================================================
# ДОПОЛНИТЕЛЬНЫЕ ОПЦИИ ОПТИМИЗАЦИИ
# ================================================================

# Настройки диспетчеризации
set(HWY_ENABLE_DYNAMIC_DISPATCH ON CACHE BOOL "Enable runtime CPU feature detection")
set(HWY_DISABLE_DYNAMIC_DISPATCH OFF CACHE BOOL "Force static dispatch if needed")

# Настройки для конкретных целей
set(HWY_CMAKE_NATIVE ON CACHE BOOL "Optimize for the native CPU architecture")
set(HWY_CMAKE_COMPACT OFF CACHE BOOL "Prefer smaller code size over speed")
set(HWY_CMAKE_FAST ON CACHE BOOL "Prefer maximum speed over code size")

# Специальные режимы
set(HWY_CMAKE_EMU128 OFF CACHE BOOL "Disable 128-bit emulation mode")
set(HWY_CMAKE_DEBUG_ONLY OFF CACHE BOOL "Disable debug-only optimizations")

# ================================================================
# НАСТРОЙКИ КОМПИЛЯЦИИ
# ================================================================

# Настройки компиляции
set(HWY_WARNINGS_ARE_ERRORS OFF CACHE BOOL "Disable treating warnings as errors")
set(HWY_ENABLE_DYNAMIC_DISPATCH ON CACHE BOOL "Enable runtime CPU detection for optimal SIMD")
set(HWY_EMULATE_SVE OFF CACHE BOOL "Disable SVE emulation on non-SVE hardware")

# Подключение Highway
add_subdirectory(lib/highway)

# Связывание с Highway
target_link_libraries(neural_network_experimental PRIVATE hwy hwy_contrib)
target_include_directories(neural_network_SDL PRIVATE ${HWY_INCLUDE_DIRS})
