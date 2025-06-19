

# Highway configuration
set(HWY_ENABLE_EXAMPLES OFF CACHE BOOL "Disable examples")
set(HWY_ENABLE_TESTS OFF CACHE BOOL "Disable tests")
set(HWY_WARNINGS_ARE_ERRORS OFF CACHE BOOL "Disable warnings as errors")

# Подключение Highway
add_subdirectory(lib/highway)

# Ваша цель (исполняемый файл/библиотека)
add_executable(neural_network_experimental ...)

# Связывание с Highway
target_link_libraries(neural_network_experimental PRIVATE hwy hwy_contrib)
