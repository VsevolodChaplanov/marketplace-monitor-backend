project(app LANGUAGES CXX)

# ---- Project options target

add_library(${PROJECT_NAME}_options INTERFACE)
target_compile_features(${PROJECT_NAME}_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_options(${PROJECT_NAME}_options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_options
                                            fmt::fmt
                                            Boost::headers
                                            openssl::openssl)
target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
