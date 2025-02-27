cmake_minimum_required(VERSION 3.14)

set(PARENT_PROJECT_NAME ${PROJECT_NAME})

project(utility-tests LANGUAGES CXX)

include(../../cmake/project-is-top-level.cmake)
include(../../cmake/folders.cmake)
include(../../cmake/static-analyzers.cmake)
include(../../cmake/memory.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
    find_package(utility REQUIRED)
endif()

# ---- Find testing frameworks

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Collect test files

file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# ---- Tests ----
add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PARENT_PROJECT_NAME} Catch2::Catch2WithMain)

target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_SCAN_FOR_MODULES ON)

# ---- Discover Catch2 tests

catch_discover_tests(${PROJECT_NAME})

# ---- Add coverage

if(utility_ENABLE_COVERAGE)
    add_coverage_lcov_target(${PROJECT_NAME})
    add_coverage_llvm_target(${PROJECT_NAME})
endif()

# ---- Add valgrind target

option(utility_VALGRIND "Add valgrind run target" ON)
if(utility_VALGRIND)
    valgrind_target(${PROJECT_NAME})
    memcheck_target(${PROJECT_NAME})
endif()

# ---- End-of-file commands ----

add_folders(Test)
