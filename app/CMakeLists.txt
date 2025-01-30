project(app LANGUAGES CXX)

# ---- Project options target

add_library(${PROJECT_NAME}-options INTERFACE)

target_compile_features(${PROJECT_NAME}-options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

target_compile_options(${PROJECT_NAME}-options INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/bigobj>)

add_library(${PROJECT_NAME}-lib OBJECT)

file(GLOB _app_cxx_modules ${PROJECT_SOURCE_DIR}/module/*.cppm)

target_sources(${PROJECT_NAME}-lib PUBLIC FILE_SET ${PROJECT_NAME}_module TYPE CXX_MODULES FILES ${_app_cxx_modules})

target_link_libraries(${PROJECT_NAME}-lib PUBLIC ${PROJECT_NAME}-options fmt::fmt Boost::headers Boost::program_options
                                                 openssl::openssl)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib)

target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
