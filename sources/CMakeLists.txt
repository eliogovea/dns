set(
    DNS_PUBLIC_HEADERS
    dns_record.hpp
    dns_encode.hpp
    dns_decode.hpp
    dns_name.hpp
    dns.hpp
)

set(
    DNS_PRIVATE_SOURCES
    dns_decode.cpp
    dns_encode.cpp
    dns_record.cpp
    dns.cpp
    dns_name.cpp
)

# shared library

add_library(${PROJECT_NAME}_shared SHARED)

target_sources(
    ${PROJECT_NAME}_shared
    PRIVATE ${DNS_PRIVATE_SOURCES}
)

set_target_properties(
    ${PROJECT_NAME}_shared
    PROPERTIES OUTPUT_NAME                ${PROJECT_NAME}
               POSITION_INDEPENDENT_CODE ON
               PUBLIC_HEADER             "${DNS_PUBLIC_HEADERS}"
)

target_include_directories(
    ${PROJECT_NAME}_shared
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

)

target_compile_features(
    ${PROJECT_NAME}_shared
    PUBLIC cxx_std_20
)

add_library(${PROJECT_NAME}::${PROJECT_NAME}_shared ALIAS ${PROJECT_NAME}_shared)

# static library

add_library(${PROJECT_NAME}_static STATIC)

target_sources(
    ${PROJECT_NAME}_static
    PRIVATE ${DNS_PRIVATE_SOURCES}
)

set_target_properties(
    ${PROJECT_NAME}_static
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
               PUBLIC_HEADER "${DNS_PUBLIC_HEADERS}"
)

target_include_directories(
    ${PROJECT_NAME}_static
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

)

target_compile_features(
    ${PROJECT_NAME}_static
    PUBLIC cxx_std_20
)

add_library(${PROJECT_NAME}::${PROJECT_NAME}_static ALIAS ${PROJECT_NAME}_static)

# tests

find_package(GTest REQUIRED)

add_executable(
    ${PROJECT_NAME}_unittest
)

target_sources(
    ${PROJECT_NAME}_unittest
    PRIVATE dns_decode_test.cpp
            dns_encode_test.cpp
            dns_name_test.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_unittest
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}_static
            GTest::gtest_main
)

add_test(
    NAME ${PROJECT_NAME}_unittest
    COMMAND ${PROJECT_NAME}_unittest
)

if (DNS_ENABLE_FUZZING)
    function(dns_add_fuzzer fuzzer_name fuzzer_source)
        add_executable(${fuzzer_name})

        target_sources(
            ${fuzzer_name}
            PRIVATE ${fuzzer_source}
        )

        target_link_libraries(
            ${fuzzer_name}
            PRIVATE ${PROJECT_NAME}_static
                    -coverage
                    -fsanitize=fuzzer,undefined,address
        )

        target_compile_options(
            ${fuzzer_name}
            PRIVATE -fsanitize=fuzzer,undefined,address
        )
    endfunction()

    dns_add_fuzzer(${PROJECT_NAME}_fuzzer_decode dns_fuzzer_decode.cpp)
    dns_add_fuzzer(${PROJECT_NAME}_fuzzer_name   dns_fuzzer_name.cpp)

endif (DNS_ENABLE_FUZZING)

# apps

add_executable(${PROJECT_NAME}_testapp_resolver)

target_sources(
    ${PROJECT_NAME}_testapp_resolver
    PRIVATE dns_testapp_resolver.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_testapp_resolver
    PRIVATE ${PROJECT_NAME}_static
)

# install

include(GNUInstallDirs)

install(
    TARGETS                   ${PROJECT_NAME}_shared
                              ${PROJECT_NAME}_static
                              ${PROJECT_NAME}_testapp_resolver
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   ${PROJECT_NAME}_unittest
                              ${PROJECT_NAME}_fuzzer_decode
                              ${PROJECT_NAME}_fuzzer_name
    COMPONENT                 ${PROJECT_NAME}
    CONFIGURATIONS            Debug
    EXPORT                    ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT      ${PROJECT_NAME}
    NAMESPACE   ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
)
