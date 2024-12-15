cmake_minimum_required(VERSION 3.28)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/prelude.cmake)
include(cmake/common_options.cmake)
include(cmake/vsenvironment.cmake)
include(cmake/conan.cmake)

download_conan_provider()

message("CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_PROJECT_TOP_LEVEL_INCLUDES}")
run_vcvarsall()

project(
    ${MAIN_PROJECT_NAME}
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(cmake/ccache.cmake)
include(cmake/linker.cmake)
include(cmake/ipo.cmake)
include(cmake/coverage.cmake)

include(options.cmake)
include(dependencies.cmake)

if(${MAIN_PROJECT_NAME}_ENABLE_CCACHE)
    enable_ccache()
endif()

enable_ipo()

add_subdirectory(utility)
add_subdirectory(core)

add_subdirectory(app)

# ---- Target to link with all library modules

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}::core ${PROJECT_NAME}::utility)

# ---- Install rules for project interface library

include(cmake/install-rules.cmake)

# ---- Developer mode ----

if(NOT ${MAIN_PROJECT_NAME}_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of ${MAIN_PROJECT_NAME}")
endif()

include(cmake/dev-mode.cmake)
