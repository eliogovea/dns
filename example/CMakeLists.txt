add_executable(resolve resolve.cpp)
target_link_libraries(resolve PRIVATE dns::dns)