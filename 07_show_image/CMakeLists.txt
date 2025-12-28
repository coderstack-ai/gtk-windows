project(07_show_image CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(07_show_image
  main.cc
  windows/main_window.cc
)

target_include_directories(07_show_image PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(07_show_image PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(07_show_image PRIVATE ${GTK4_LIBRARIES})
target_compile_options(07_show_image PRIVATE ${GTK4_CFLAGS_OTHER})
