project(app LANGUAGES CXX)


# ---- Project options target

add_library(${PROJECT_NAME}_options INTERFACE)
target_compile_features(${PROJECT_NAME}_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_options
                                            boost::boost)
