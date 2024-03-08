cmake_minimum_required(VERSION 3.11)

project(
    dns
    VERSION      0.0.1
    DESCRIPTION  "DNS library"
    HOMEPAGE_URL "https://github.com/eliogovea/dns"
    LANGUAGES    CXX
)


option(DNS_ENABLE_COVERAGE        "Enable coverage"        OFF)
option(DNS_ENABLE_FORMATTING      "Enable formatting"      OFF)
option(DNS_ENABLE_FUZZING         "Enable fuzzing"         OFF)
option(DNS_ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)


list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

enable_testing()

add_subdirectory(sources)

# https://cmake.org/cmake/help/latest/module/CPack.html

set(CPACK_PACKAGE_NAME                 ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR               "")
set(CPACK_PACKAGE_DIRECTORY            ${CMAKE_BINARY_DIR})
set(CPACK_PACKAGE_VERSION_MAJOR        ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR        ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH        ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION          ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY  ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_HOMEPAGE_URL         ${PROJECT_HOMEPAGE_URL})
set(CPACK_PACKAGE_INSTALL_DIRECTORY    ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_CHECKSUM             SHA1)
set(CPACK_MONOLITHIC_INSTALL           FALSE)
set(CPACK_STRIP_FILES                  TRUE)
set(CPACK_VERBATIM_VARIABLES           TRUE)
set(CPACK_SOURCE_STRIP_FILES           TRUE)
set(CPACK_SOURCE_GENERATOR             "TGZ")
set(CPACK_SOURCE_IGNORE_FILES          "build/;\\.devcontainer/;\\.git*;\\.vscode/")

include(CPack)

if (DNS_ENABLE_COVERAGE)
    if ("${COVERAGE_LCOV}" STREQUAL "")
        find_program(COVERAGE_LCOV lcov)
    endif()

    if ("${COVERAGE_GCOV}" STREQUAL "")
        find_program(COVERAGE_GCOV gcov)
    endif()

    if ("${COVERAGE_GENHTML}" STREQUAL "")
        find_program(COVERAGE_GENHTML genhtml)
    endif()

    if ("${COVERAGE_LCOV}" STREQUAL "COVERAGE_LCOV-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_LCOV not found")
    endif()

    if ("${COVERAGE_GCOV}" STREQUAL "COVERAGE_GCOV-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_GCOV not found")
    endif()

    if ("${COVERAGE_GENHTML}" STREQUAL "COVERAGE_GENHTML-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_GENHTML not found")
    endif()

    add_custom_target(
        coverage_lcov
        COMMAND ${COVERAGE_LCOV} --gcov-tool ${COVERAGE_GCOV}
                                 --capture
                                 --no-external
                                 --exclude *_test.cpp
                                 --directory ${CMAKE_CURRENT_LIST_DIR}
                                 --output-file ${CMAKE_CURRENT_BINARY_DIR}/lcov.info
    )
    add_custom_target(
        coverage_html
        COMMAND ${COVERAGE_GENHTML} --output-directory ${CMAKE_CURRENT_BINARY_DIR}/lcov
                                    --keep-descriptions
                                    --function-coverage
                                    --branch-coverage
                                    --highlight
                                    --legend
                                    --missed
                                    ${CMAKE_CURRENT_BINARY_DIR}/lcov.info
        DEPENDS coverage_lcov
    )
endif(DNS_ENABLE_COVERAGE)

if (DNS_ENABLE_FORMATTING)
    if ("${CLANG_FORMAT}" STREQUAL "")
        find_program(CLANG_FORMAT clang-format)
    endif()

    if ("${CLANG_FORMAT}" STREQUAL "CLANG_FORMAT-NOTFOUND")
        message(FATAL_ERROR "clang-format not found")
    endif()

    add_custom_target(
        format
        COMMAND find ${CMAKE_CURRENT_LIST_DIR}/sources
                     -type f
                     -regextype posix-egrep
                     -regex "\".+\\\.(h|c|hh|cc|hpp|cpp|hxx|cxx|h++|c++|H|C)$$\""
                     -exec ${CLANG_FORMAT} -i --verbose {} \\\;
    )
endif(DNS_ENABLE_FORMATTING)


if (DNS_ENABLE_STATIC_ANALYSIS)
    if ("${STATIC_ANALYSIS_RUN_CLANG_TIDY}" STREQUAL "")
        find_program(STATIC_ANALYSIS_RUN_CLANG_TIDY run-clang-tidy)
    endif()

    if ("${STATIC_ANALYSIS_RUN_CLANG_TIDY}" STREQUAL "STATIC_ANALYSIS_RUN_CLANG_TIDY-NOTFOUND")
        message(FATAL_ERROR "run-clang-tidy not found")
    endif()

    add_custom_target(
        static-analysis-clang-tidy
        COMMAND ${STATIC_ANALYSIS_RUN_CLANG_TIDY} -p ${CMAKE_BINARY_DIR}
    )
endif(DNS_ENABLE_STATIC_ANALYSIS)
