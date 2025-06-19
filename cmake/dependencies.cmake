
#include_directories(${LLVM_CLANG_PATH}/include)
#file(GLOB LLVM_CLANG_LIBS "C:/Users/user/Desktop/Programming/Compilers/LLVM_Clang_compiler/clang+llvm-19.1.0-x86_64-pc-windows-msvc/lib/*.lib")
#target_link_libraries(neural_network_SDL PUBLIC ${LLVM_CLANG_LIBS})


# »щем SDL2
set(SDL_DIRECTX, OFF)
find_package(SDL2)
if(SDL2_FOUND)
    target_link_libraries(neural_network_SDL PUBLIC ${SDL2_LIBRARIES})
else()
    add_subdirectory("${LIB_PATH}/SDL-release-2.30.11" "${PROJECT_SDL_DIRECTORY}" EXCLUDE_FROM_ALL)
    target_link_libraries(neural_network_SDL PUBLIC SDL2::SDL2main SDL2::SDL2)
endif()
include_directories(C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/build_SDL3_debug/include)#C:/Users/user/Desktop/Programming/lib/SDL-release-2.30.11/include

# »щем SDL3
#set(SDL_STATIC, ON)
#find_package(SDL3)
#if(SDL3_FOUND)
#    target_link_libraries(neural_network_SDL PUBLIC SDL3::SDL3 SDL3::SDL3-static)
#else()
#    add_subdirectory("${LIB_PATH}/SDL3-main" "${PROJECT_SDL_DIRECTORY}" EXCLUDE_FROM_ALL)
#    target_link_libraries(neural_network_SDL PUBLIC SDL3::SDL3 SDL3::SDL3)
#endif()
#include_directories(C:/Users/user/Desktop/Programming/lib/SDL3-main/include/SDL3/SDL.h)

# Highway configuration
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "Disable examples")
set(HWY_ENABLE_TESTS OFF CACHE BOOL "Disable tests")
set(HWY_WARNINGS_ARE_ERRORS OFF CACHE BOOL "Disable warnings as errors")

# Try to find system Highway first
find_package(hwy QUIET)

if(hwy_FOUND)
    message(STATUS "Found system Highway: ${hwy_VERSION}")
    target_link_libraries(neural_network_SDL PUBLIC hwy::hwy)
else()
    message(STATUS "Highway not found, using bundled version")

    include(FetchContent)
    FetchContent_Declare(
        highway
        GIT_REPOSITORY https://github.com/google/highway.git
        GIT_TAG 1.0.7  # Stable version
    )
    FetchContent_MakeAvailable(highway)

    target_link_libraries(neural_network_SDL PUBLIC hwy hwy_contrib)
    target_link_libraries(neural_network_SDL PUBLIC
        hwy
        hwy_contrib
    )
endif()


include_directories(
    C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/include
    C:/Users/user/Desktop/Programming/lib/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include
    C:/Users/user/Desktop/Programming/lib/boost_1_86_0/bin/include/boost-1_86
)


target_link_libraries(neural_network_SDL PUBLIC
    #C:/Users/user/Desktop/Programming/lib/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib/libSDL2_ttf.a
    C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/build/libfreetype.a
    shlwapi
    ole32
    oleaut32
    imm32
    wsock32
)

