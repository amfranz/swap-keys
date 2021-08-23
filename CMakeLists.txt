cmake_minimum_required(VERSION 3.0)

project(swap-keys)

find_package(PkgConfig)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

add_executable(swap-keys main.c)
target_compile_options(swap-keys PRIVATE -Wall -Wextra)
target_include_directories(swap-keys PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(swap-keys ${LIBEVDEV_LIBRARIES})

install(TARGETS swap-keys RUNTIME DESTINATION bin)
