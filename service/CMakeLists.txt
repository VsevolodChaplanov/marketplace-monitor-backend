cmake_minimum_required(VERSION 3.28)

include(GenerateExportHeader)

include(../cmake/folders.cmake)
include(../cmake/linker.cmake)
include(../cmake/sanitizers.cmake)
include(../cmake/static-analyzers.cmake)

project(service CXX)

add_library(${PROJECT_NAME}-common INTERFACE)
add_library(Monitor::ServiceCommon ALIAS ${PROJECT_NAME}-common)
target_link_libraries(${PROJECT_NAME}-common INTERFACE asio-grpc::asio-grpc grpc::grpc Boost::headers)

# FetchContent for monitor-protos because it is a submodule and rule how to find it can not be modified directly
FetchContent_Declare(
    monitor-protos
    GIT_REPOSITORY https://github.com/VsevolodChaplanov/monitor-grpc-api.git
    GIT_TAG master)
FetchContent_MakeAvailable(monitor-protos)

add_subdirectory(items)
