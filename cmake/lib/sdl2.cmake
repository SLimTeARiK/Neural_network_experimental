# Опции SDL2
set(SDL_SHARED OFF CACHE BOOL "Build static lib")
set(SDL_STATIC ON CACHE BOOL "Enable static build")
set(SDL_TESTS OFF CACHE BOOL "Disable tests")

#Enable/Disable

set(SDL_VIDEO ON CACHE BOOL "Enable video subsystem")
set(SDL_AUDIO ON CACHE BOOL "Enable audio subsystem")
set(SDL_EVENTS ON CACHE BOOL "Enable events subsystem")
set(SDL_JOYSTICK ON CACHE BOOL "Enable joystick subsystem")
set(SDL_HAPTIC ON CACHE BOOL "Enable haptic subsystem")
set(SDL_POWER ON CACHE BOOL "Enable power management subsystem")
set(SDL_THREADS ON CACHE BOOL "Enable threads subsystem")
set(SDL_TIMERS ON CACHE BOOL "Enable timers subsystem")
set(SDL_FILE OFF CACHE BOOL "Disable file subsystem")
set(SDL_LOADSO ON CACHE BOOL "Enable loadso subsystem")
set(SDL_CPUINFO ON CACHE BOOL "Enable CPU info subsystem")
set(SDL_FILESYSTEM ON CACHE BOOL "Enable filesystem subsystem")
set(SDL_SENSOR ON CACHE BOOL "Enable sensor subsystem")


set(SDL_DIRECTX OFF CACHE BOOL "Disable DirectX support")
set(SDL_RENDER_D3D OFF CACHE BOOL "Disable Direct3D renderer")
set(SDL_RENDER_D3D11 OFF CACHE BOOL "Disable Direct3D11 renderer")
set(SDL_RENDER_D3D12 OFF CACHE BOOL "Disable Direct3D12 renderer")
set(SDL_DIRECT3D OFF CACHE BOOL "Disable Direct3D support")
set(SDL3_MF OFF CACHE BOOL "Disable Media Foundation support")
set(SDL_VULKAN OFF CACHE BOOL "Disable Vulkan support")
set(SDL_OPENGL ON CACHE BOOL "Enable OpenGL support")
set(SDL_OPENGLES OFF CACHE BOOL "Disable OpenGL ES support")


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



