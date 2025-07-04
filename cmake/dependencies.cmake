
include(${CMAKE_SOURCE_DIR}/cmake/lib/highway.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/lib/sdl3.cmake)
#include(${CMAKE_SOURCE_DIR}/lib/raylib.cmake)


include_directories(
    C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/include
    C:/Users/user/Desktop/Programming/lib/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include
    C:/Users/user/Desktop/Programming/lib/boost_1_86_0/bin/include/boost-1_86
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    #C:/Users/user/Desktop/Programming/lib/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib/libSDL2_ttf.a
    C:/Users/user/Desktop/Programming/lib/freetype-2.12.0/build/libfreetype.a
    shlwapi
    ole32
    oleaut32
    imm32
    wsock32
)

