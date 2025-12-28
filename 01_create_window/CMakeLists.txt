project(01_create_window CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(01_create_window
  main.cc
)

target_include_directories(01_create_window PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(01_create_window PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(01_create_window PRIVATE ${GTK4_LIBRARIES})
target_compile_options(01_create_window PRIVATE ${GTK4_CFLAGS_OTHER})
