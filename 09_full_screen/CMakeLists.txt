project(09_full_screen CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(09_full_screen
  main.cc
  windows/main_window.cc
)

target_include_directories(09_full_screen PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(09_full_screen PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(09_full_screen PRIVATE ${GTK4_LIBRARIES})
target_compile_options(09_full_screen PRIVATE ${GTK4_CFLAGS_OTHER})
