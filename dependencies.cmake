set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cpm_packages" CACHE STRING "Directory to download CPM packages")

include(cmake/get-cpm.cmake)

# required project dependencies here

cpmaddpackage("gh:fmtlib/fmt#11.0.2") 

cpmaddpackage(
    GITHUB_REPOSITORY gabime/spdlog 
    VERSION 1.15.0 
    OPTIONS "SPDLOG_FMT_EXTERNAL 1" 
    SYSTEM)

cpmaddpackage(
    NAME Boost
    VERSION 1.86.0
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.zip
    URL_MD5 86c12489083a48878f8976f9d3e29cb0
    OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_INCLUDE_LIBRARIES asio\\\;beast\\\;redis\\\;url\\\;cobalt\\\;program_options\\\;uuid"
    SYSTEM)