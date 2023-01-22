find_package(GTest REQUIRED)

add_executable(unit_tests name_test.cpp decode_test.cpp)

target_link_libraries(
    unit_tests 
    PRIVATE GTest::gtest_main
            dns::dns 
)

target_compile_options(
    unit_tests 
    PRIVATE -Wall -Wextra -Wpedantic
)

add_test(
  NAME unit_tests 
  COMMAND unit_tests
)

add_executable(name_fuzzer name_fuzzer.cpp)

target_link_libraries(
  name_fuzzer
    PRIVATE dns::dns
            -coverage
            -fsanitize=fuzzer,undefined,address
)

target_compile_options(
  name_fuzzer 
    PRIVATE -fsanitize=fuzzer,undefined,address
            -Wall 
            -Wextra 
            -Wpedantic
)

add_executable(decode_fuzzer decode_fuzzer.cpp)

target_link_libraries(
  decode_fuzzer
    PRIVATE dns::dns
            -coverage
            -fsanitize=fuzzer,undefined,address
)

target_compile_options(
  decode_fuzzer 
    PRIVATE -fsanitize=fuzzer,undefined,address
            -Wall 
            -Wextra 
            -Wpedantic
)
