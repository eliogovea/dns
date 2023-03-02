set(CMAKE_CXX_COMPILER g++)

set(GCC_WARNING_FLAGS "")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wall
set(GCC_WARNING_FLAGS "${GCC_WARNING_FLAGS} -Wall")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wextra
set(GCC_WARNING_FLAGS "${GCC_WARNING_FLAGS} -Wextra")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Wpedantic
set(GCC_WARNING_FLAGS "${GCC_WARNING_FLAGS} -Wpedantic")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Warning-Options.html#index-Werror
set(GCC_WARNING_FLAGS "${GCC_WARNING_FLAGS} -Werror")

# CMAKE_BUILD_TYPE=Debug

set(CMAKE_CXX_FLAGS_DEBUG "" CACHE INTERNAL "")

# Enable Warnings
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_WARNING_FLAGS}")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html#index-O0
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Debugging-Options.html#index-g
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# CMAKE_BUILD_TYPE=Release

set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "")

# Enable Warnings
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${GCC_WARNING_FLAGS}")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html#index-O2
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# CMAKE_BUILD_TYPE=RelWithDebInfo

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "")

# Enable Warnings
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${GCC_WARNING_FLAGS}")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html#index-O2
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Debugging-Options.html#index-g
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")

# CMAKE_BUILD_TYPE=MinSizeRel

set(CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE INTERNAL "")

# Enable Warnings
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${GCC_WARNING_FLAGS}")

# https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html#index-Os
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")