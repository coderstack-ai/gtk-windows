cmake_minimum_required(VERSION 3.20)
project(gtk4_app)

set(CMAKE_C_STANDARD 11)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/install)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_subdirectory(01_create_window)
add_subdirectory(02_create_single_window)
add_subdirectory(03_button)
add_subdirectory(04_new_window)
add_subdirectory(05_menu_window)
add_subdirectory(06_open_save_file)
add_subdirectory(07_show_image)
add_subdirectory(08_menu)
add_subdirectory(09_full_screen)