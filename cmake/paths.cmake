
set(LIB_PATH "C:/Users/user/Desktop/Programming/lib")


set(LLVM_CLANG_PATH "C:/Users/user/Desktop/Programming/Compilers/LLVM_Clang_compiler/clang+llvm-19.1.0-x86_64-pc-windows-msvc")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(PROJECT_SDL_DIRECTORY "build_SDL3_debug")
#set(PROJECT_SDL2_TTF_DIRECTORY "build_SDL2_ttf_debug")
#set(PROJECT_freetype_DIRECTORY "build_freetype_debug")



elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
set(PROJECT_SDL_DIRECTORY "build_SDL2_release")
#set(PROJECT_SDL2_TTF_DIRECTORY "build_SDL2_ttf_release")
#set(PROJECT_freetype_DIRECTORY "build_freetype_release")


elseif(CMAKE_BUILD_TYPE STREQUAL "Test")

endif()
