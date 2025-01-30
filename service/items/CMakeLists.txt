cmake_minimum_required(VERSION 3.28)

include(GenerateExportHeader)

project(
    service-items
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

add_library(${PROJECT_NAME}-proto)

target_link_libraries(${PROJECT_NAME}-proto PUBLIC Monitor::ServiceCommon)

file(GLOB _items_service_protos ${monitor-protos_SOURCE_DIR}/item-proto/*.proto)

asio_grpc_protobuf_generate(
    GENERATE_GRPC
    GENERATE_MOCK_CODE
    TARGET
    ${PROJECT_NAME}-proto
    OUT_DIR
    ${PROJECT_BINARY_DIR}/include/generated/items/
    PROTOS
    "${_items_service_protos}")

file(GLOB _items_service_module_units ${PROJECT_SOURCE_DIR}/module/items/*.cppm)
file(GLOB _items_service_generated_headers ${PROJECT_BINARY_DIR}/include/generated/items/*.h
     ${PROJECT_BINARY_DIR}/include/generated/items/*.hpp)

add_library(${PROJECT_NAME})

# ---- Generate export header

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/generated/exports/exports.hpp)

list(APPEND _items_service_generated_headers ${PROJECT_BINARY_DIR}/include/generated/exports/exports.hpp)
message("${_items_service_generated_headers}")
target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET
           CXX_MODULES
           TYPE
           CXX_MODULES
           FILES
           ${_items_service_module_units}
           FILE_SET
           HEADERS
           BASE_DIRS
           ${PROJECT_BINARY_DIR}/include
           TYPE
           FILES
           ${_items_service_generated_headers})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}-proto)

# ---- Configure linker

target_configure_linker(${PROJECT_NAME})

# ---- Clang-tidy target

target_clangtidy(${PROJECT_NAME} "${_items_service_protos}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}" USE_ON_BUILD
                 "${PROJECT_NAME}_ENABLE_CLANGTIDY_ON_BUILD")

# ---- Cppcheck target

target_cppcheck(${PROJECT_NAME} "${_items_service_protos}" WARNINGS_AS_ERRORS "${WARNINGS_AS_ERRORS}" USE_ON_BUILD
                "${PROJECT_NAME}_ENABLE_CPPCHECK_ON_BUILD")

# ---- IWYU target

target_include_what_you_use(${PROJECT_NAME} "${_items_service_protos}" USE_ON_BUILD
                            "${PROJECT_NAME}_ENABLE_IWYU_ON_BUILD")

# ---- Clang-format target

target_clang_format(${PROJECT_NAME} "${_items_service_protos}")

# ---- Add coverage support

if(${PROJECT_NAME}_ENABLE_COVERAGE)
    target_add_coverage_flags(${PROJECT_NAME})
endif()

# ---- Doxygen

if(${PROJECT_NAME}_BUILD_DOCS)
    include(../cmake/docs.cmake)
    wrap_doxygen_add_docs(${PROJECT_NAME} SOURCE_DIRS "${_items_service_protos}" OUTPUT "doxygen")
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
