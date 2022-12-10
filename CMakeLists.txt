cmake_minimum_required(VERSION 3.11)

project(dns)

add_library(dns STATIC)

add_library(dns::dns ALIAS dns)

target_include_directories(
    dns
    PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(
    dns
    PRIVATE src/decode.cpp
            src/encode.cpp
            src/name.cpp
            src/record.cpp
)

target_compile_features(
    dns
    PUBLIC cxx_std_20
)

enable_testing()
include(CTest)
add_subdirectory(test)

add_subdirectory(example)
