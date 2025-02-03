cmake_minimum_required(VERSION 3.15)

include(GenerateExportHeader)

include(../cmake/folders.cmake)
include(../cmake/linker.cmake)
include(../cmake/sanitizers.cmake)
include(../cmake/static-analyzers.cmake)
include(../cmake/lint-targets.cmake)

set(PARENT_PROJECT_NAME ${PROJECT_NAME})

project(
    utility
    VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}
    LANGUAGES CXX)

include(cmake/options.cmake)
include(cmake/dependencies.cmake)

# ---- Project options target

add_library(${PROJECT_NAME}-options INTERFACE)

# ---- Set compile features

target_link_libraries(${PROJECT_NAME}-options INTERFACE ${PARENT_PROJECT_NAME}-options)

# ---- Add sanitizers

target_sanitizers(
    ${PROJECT_NAME}-options
    SANITIZER_ADDRESS
    ${${PROJECT_NAME}_SANITIZER_ADDRESS}
    SANITIZER_LEAK
    ${${PROJECT_NAME}_SANITIZER_LEAK}
    SANITIZER_UNDEFINED_BEHAVIOR
    ${${PROJECT_NAME}_SANITIZER_UNDEFINED_BEHAVIOR}
    SANITIZER_THREAD
    ${${PROJECT_NAME}_SANITIZER_THREAD}
    SANITIZER_MEMORY
    ${${PROJECT_NAME}_SANITIZER_MEMORY})

# ---- Declare library precompiled headers

add_library(${PROJECT_NAME}-pch INTERFACE) # declare library if pch is off prevent install rule error for unknown target

if(${PROJECT_NAME}_ENABLE_PCH)
    target_precompile_headers(${PROJECT_NAME}-pch INTERFACE <string> <vector>)
    target_link_libraries(${PROJECT_NAME}-pch INTERFACE ${PROJECT_NAME}-options)
endif()

# ---- Declare library ----

if(${PROJECT_NAME}_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED)
else()
    add_library(${PROJECT_NAME} SHARED)
endif()
add_library(${PARENT_PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ---- Collect sources

file(GLOB _utility_cxx_modules ${PROJECT_SOURCE_DIR}/module/${PROJECT_NAME}/*.cppm)

# ---- Set target sources

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           ${PROJECT_NAME}_module
           TYPE
           CXX_MODULES
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/module
           FILES
           ${_utility_cxx_modules})

# ---- Add libraries

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_NAME}-options>
    PRIVATE $<BUILD_INTERFACE:${PROJECT_NAME}-pch>)

# ---- Export

export(
    TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-options ${PROJECT_NAME}-pch
    NAMESPACE ${PARENT_PROJECT_NAME}::
    FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

# ---- Configure linker

target_configure_linker(${PROJECT_NAME})

# ---- Clang-tidy target

if(${PROJECT_NAME}_ENABLE_CLANGTIDY_ON_BUILD)
    target_clangtidy(${PROJECT_NAME} "${_utility_cxx_modules}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}")
endif()

# ---- Cppcheck target

if(${PROJECT_NAME}_ENABLE_CPPCHECK_ON_BUILD)
    target_cppcheck(${PROJECT_NAME} "${_utility_cxx_modules}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}")
endif()

# ---- IWYU target

if(${PROJECT_NAME}_ENABLE_IWYU_ON_BUILD)
    target_include_what_you_use(${PROJECT_NAME} "${_utility_cxx_modules}")
endif()

# ---- Clang-format target

target_clang_format(${PROJECT_NAME} "${_utility_cxx_modules}")

# ---- Doxygen

if(${PROJECT_NAME}_BUILD_DOCS)
    include(../cmake/docs.cmake)
    wrap_doxygen_add_docs(${PROJECT_NAME} SOURCE_DIRS "${_utility_cxx_modules}" OUTPUT "doxygen")
endif()

# ---- Build examples

if(${PROJECT_NAME}_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

# ---- Build tests

include(CTest)
if(BUILD_TESTING AND ${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif()

# ---- Configure install rules

if(NOT CMAKE_SKIP_INSTALL_RULES AND NOT ${PROJECT_NAME}_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif()

# ---- Project outline

add_folders(${PROJECT_NAME})
