project(08_menu CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(08_menu
  main.cc
  windows/main_window.cc
)

target_include_directories(08_menu PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(08_menu PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(08_menu PRIVATE ${GTK4_LIBRARIES})
target_compile_options(08_menu PRIVATE ${GTK4_CFLAGS_OTHER})
