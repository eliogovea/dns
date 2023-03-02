set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS_INIT "" CACHE INTERNAL "")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html#index-O0
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -O0")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Debugging-Options.html#index-g
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -g")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wall
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wall")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wextra
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wextra")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wpedantic
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wpedantic")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Werror
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Werror")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Instrumentation-Options.html#index-coverage
# set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} --coverage")

set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -fprofile-arcs")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -ftest-coverage")