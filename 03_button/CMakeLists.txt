project(03_button CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(03_button
  main.cc
  gtk4.cc
)

target_include_directories(03_button PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_directories(03_button PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(03_button PRIVATE ${GTK4_LIBRARIES})
target_compile_options(03_button PRIVATE ${GTK4_CFLAGS_OTHER})
