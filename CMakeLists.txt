cmake_minimum_required(VERSION 3.29.2)
project(neural_network_SDL)

#set(CMAKE_C_COMPILER "clang.exe")


set(CLANG_COMPILER OFF)
set(GCC_COMPILER ON)

set(LLVM_CLANG_PATH "C:/Users/user/Desktop/Programming/Compilers/llvm-mingw-20250528-msvcrt-x86_64/bin")
# Установка компиляторов по умолчанию
#if(CMAKE_C_COMPILER STREQUAL "gcc.exe" OR CMAKE_CXX_COMPILER STREQUAL "g++.exe")
    #set(CMAKE_C_COMPILER gcc.exe)
    #set(CMAKE_CXX_COMPILER g++.exe)
    #set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/mingww64/${CMAKE_BUILD_TYPE}")
#elseif(CMAKE_C_COMPILER STREQUAL "clang.exe" OR CMAKE_CXX_COMPILER STREQUAL "clang++.exe")
    set(CMAKE_C_COMPILER "${LLVM_CLANG_PATH}/clang.exe")
    set(CMAKE_CXX_COMPILER "${LLVM_CLANG_PATH}/clang++.exe")
    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/clang/${CMAKE_BUILD_TYPE}")
#endif()


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack=16777216")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/paths.cmake)

# Добавляем исполняемый файл и исходные файлы
add_executable(neural_network_SDL
    src/main.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)



# Используем target_compile_definitions для передачи макросов
target_compile_definitions(neural_network_SDL PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(neural_network_SDL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
