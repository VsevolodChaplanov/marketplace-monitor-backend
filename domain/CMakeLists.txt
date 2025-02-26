cmake_minimum_required(VERSION 3.28)

set(PARENT_PROJECT_NAME ${PROJECT_NAME})

include(GenerateExportHeader)

include(../cmake/folders.cmake)
include(../cmake/linker.cmake)
include(../cmake/sanitizers.cmake)
include(../cmake/static-analyzers.cmake)

project(
    domain
    LANGUAGES CXX
    VERSION 0.1.0)

include(cmake/options.cmake)

# ---- Project options target

add_library(${PROJECT_NAME}-options INTERFACE)

# ---- Features

target_compile_features(${PROJECT_NAME}-options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

# ---- Options

target_compile_options(${PROJECT_NAME}-options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)

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

add_library(${PROJECT_NAME})

# ---- Generate export header

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME
                       ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/exports/exports.hpp)

file(GLOB _domain_cxx_modules ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}/*.cppm)

target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           ${PROJECT_NAME}_module
           TYPE
           CXX_MODULES
           BASE_DIRS
           ${PROJECT_SOURCE_DIR}/src
           FILES
           ${_domain_cxx_modules}
           FILE_SET
           ${PROJECT_NAME}_headers
           TYPE
           HEADERS
           BASE_DIRS
           ${PROJECT_BINARY_DIR}/include
           FILES
           ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/exports/exports.hpp)

# ---- Include directories

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# ---- Scan for modules explicitly

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_SCAN_FOR_MODULES ON)

# ---- Configure linker

target_configure_linker(${PROJECT_NAME})

# ---- Clang-tidy target

target_clangtidy(${PROJECT_NAME} "${_domain_cxx_modules}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}" USE_ON_BUILD
                 "${PROJECT_NAME}_ENABLE_CLANGTIDY_ON_BUILD")

# ---- Cppcheck target

target_cppcheck(${PROJECT_NAME} "${_domain_cxx_modules}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}" USE_ON_BUILD
                "${PROJECT_NAME}_ENABLE_CPPCHECK_ON_BUILD")

# ---- IWYU target

target_include_what_you_use(${PROJECT_NAME} "${_domain_cxx_modules}" USE_ON_BUILD
                            "${PROJECT_NAME}_ENABLE_IWYU_ON_BUILD")

# ---- Clang-format target

target_clang_format(${PROJECT_NAME} "${_domain_cxx_modules}")

# ---- Add coverage support

if(${PROJECT_NAME}_ENABLE_COVERAGE)
    target_add_coverage_flags(${PROJECT_NAME})
endif()

# ---- Doxygen

if(${PROJECT_NAME}_BUILD_DOCS)
    include(../cmake/docs.cmake)
    wrap_doxygen_add_docs(${PROJECT_NAME} SOURCE_DIRS "${_domain_cxx_modules}" OUTPUT "doxygen")
endif()

# ---- Exporting

export(
    TARGETS ${PROJECT_NAME}
    NAMESPACE ${PARENT_PROJECT_NAME}::
    FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

# ---- Configure install rules

if(NOT CMAKE_SKIP_INSTALL_RULES AND NOT ${PROJECT_NAME}_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif()

# ---- Project outline

add_folders(${PROJECT_NAME})
