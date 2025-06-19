cmake_minimum_required(VERSION 3.29.2)
project(neural_network_experimental)


set(LLVM_CLANG_PATH "C:/Users/user/Desktop/Programming/Compilers/llvm-mingw-20250528-msvcrt-x86_64/bin")
    set(CMAKE_C_COMPILER "${LLVM_CLANG_PATH}/clang.exe")
    set(CMAKE_CXX_COMPILER "${LLVM_CLANG_PATH}/clang++.exe")
    set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build/clang/${CMAKE_BUILD_TYPE}")


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack=16777216")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()


# Добавляем исполняемый файл и исходные файлы
add_executable(neural_network_experimental
    src/main.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/paths.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)



# Используем target_compile_definitions для передачи макросов
target_compile_definitions(neural_network_experimental PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(neural_network_experimental PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
