project(06_open_save_file CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(06_open_save_file
  main.cc
  windows/main_window.cc
)

target_include_directories(06_open_save_file PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(06_open_save_file PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(06_open_save_file PRIVATE ${GTK4_LIBRARIES})
target_compile_options(06_open_save_file PRIVATE ${GTK4_CFLAGS_OTHER})
