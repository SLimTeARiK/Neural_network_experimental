# Опции SDL2
set(SDL_SHARED OFF CACHE BOOL "Build static lib")
set(SDL_STATIC ON CACHE BOOL "Enable static build")
set(SDL_TESTS OFF CACHE BOOL "Disable tests")

set(SDL_DIRECTXOFF OFF CACHE BOOL "")
set(SDL_RENDER_D3D OFF CACHE BOOL "")
set(SDL_RENDER_D3D11 OFF CACHE BOOL "")
set(SDL_RENDER_D3D12 OFF CACHE BOOL "")
set(SDL_DIRECT3D OFF CACHE BOOL "")
set(SDL3_MF OFF CACHE BOOL "")

# Подключаем SDL2
add_subdirectory(lib/SDL2 EXCLUDE_FROM_ALL)

# Для Windows: явно отключаем DirectX, если не нужен
if(WIN32)
  set(SDL_DIRECTX OFF CACHE BOOL "Disable DirectX")
endif()

target_link_libraries(neural_network_SDL PRIVATE
  SDL2::SDL2
  SDL2::SDL2main
)
target_include_directories(neural_network_SDL PRIVATE
  ${SDL2_INCLUDE_DIRS}
)
# Ищем SDL2
#set(SDL_DIRECTX, OFF)
#find_package(SDL2)
#if(SDL2_FOUND)
#    target_link_libraries(neural_network_experimental PUBLIC ${SDL2_LIBRARIES})
#else()
#    add_subdirectory("${LIB_PATH}/SDL-release-2.30.11" "${PROJECT_SDL_DIRECTORY}" EXCLUDE_FROM_ALL)
#    target_link_libraries(neural_network_experimental PUBLIC SDL2::SDL2main SDL2::SDL2)
#endif()
#include_directories(C:/Users/user/Desktop/Programming/Projects/neural_network_SDL/build/build_SDL3_debug/include)#C:/Users/user/Desktop/Programming/lib/SDL-release-2.30.11/include



