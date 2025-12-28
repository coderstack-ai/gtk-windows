project(04_new_window CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(04_new_window
  main.cc
  gtk4.cc
)

target_include_directories(04_new_window PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(04_new_window PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(04_new_window PRIVATE ${GTK4_LIBRARIES})
target_compile_options(04_new_window PRIVATE ${GTK4_CFLAGS_OTHER})
