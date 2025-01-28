cmake_minimum_required(VERSION 3.28)

project(service CXX)

add_library(${PROJECT_NAME}-common INTERFACE)
target_link_libraries(${PROJECT_NAME}-common INTERFACE asio-grpc::asio-grpc grpc::grpc Boost::headers)

# FetchContent for monitor-protos because it is a submodule and rule how to find it can not be modified directly
FetchContent_Declare(
    monitor-protos
    GIT_REPOSITORY https://github.com/VsevolodChaplanov/monitor-grpc-api.git
    GIT_TAG master)
FetchContent_MakeAvailable(monitor-protos)

add_library(${PROJECT_NAME}-items-proto)

target_link_libraries(${PROJECT_NAME}-items-proto PUBLIC ${PROJECT_NAME}-common)

file(GLOB _items_service_protos ${monitor-protos_SOURCE_DIR}/item-proto/*.proto)

asio_grpc_protobuf_generate(
    GENERATE_GRPC
    GENERATE_MOCK_CODE
    TARGET
    ${PROJECT_NAME}-items-proto
    OUT_DIR
    "${PROJECT_BINARY_DIR}/item-proto-generated/"
    PROTOS
    "${_items_service_protos}")

file(GLOB _items_service_header_module_units ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/items/*.cppm)
file(GLOB _items_service_source_module_units ${PROJECT_SOURCE_DIR}/src/items/*.cpp)

add_library(${PROJECT_NAME}-items)
target_sources(
    ${PROJECT_NAME}-items
    PUBLIC FILE_SET
           CXX_MODULES
           TYPE
           CXX_MODULES
           FILES
           ${_items_service_source_module_units}
           ${_items_service_header_module_units})
target_include_directories(
    ${PROJECT_NAME}-items
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/item-proto-generated>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
