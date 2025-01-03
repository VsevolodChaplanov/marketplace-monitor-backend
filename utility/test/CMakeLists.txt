cmake_minimum_required(VERSION 3.14)

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
target_link_libraries(${PROJECT_NAME} PRIVATE ${MAIN_PROJECT_NAME}::utility Catch2::Catch2WithMain)
target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})

# ---- Static analyzers for test target

option(LINT_TESTS "enable static analyzers for tests" OFF)

if(LINT_TESTS)
    target_cppcheck(${PROJECT_NAME} "${TEST_SOURCES}")
    target_clangtidy(${PROJECT_NAME} "${TEST_SOURCES}")
    target_include_what_you_use(${PROJECT_NAME} "${TEST_SOURCES}")
endif()

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
