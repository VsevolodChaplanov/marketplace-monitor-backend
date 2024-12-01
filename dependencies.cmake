include(cmake/get-cpm.cmake)

# Force to use conan packages, in other case try to download them
set(CPM_USE_LOCAL_PACKAGES ON)
set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)

# required project dependencies here

cpmaddpackage("gh:fmtlib/fmt#10.2.1") 

cpmaddpackage(
    GITHUB_REPOSITORY gabime/spdlog 
    VERSION 1.14.1 
    OPTIONS "SPDLOG_FMT_EXTERNAL 1" )

cpmaddpackage(
    NAME Boost
    VERSION 1.86.0
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.zip
    URL_MD5 86c12489083a48878f8976f9d3e29cb0
    OPTIONS "BOOST_ENABLE_CMAKE ON")