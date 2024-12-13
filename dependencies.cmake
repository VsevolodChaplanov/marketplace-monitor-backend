find_package(fmt 11.0.2 CONFIG REQUIRED)
find_package(spdlog 1.15.0 CONFIG REQUIRED)
find_package(OpenSSL 3.3.2 CONFIG REQUIRED)
find_package(libpqxx 7.9.2 CONFIG REQUIRED)
find_package(Boost 1.86.0 CONFIG REQUIRED)

include(cmake/get-cpm.cmake)

# ---- \todo conan provide all this components as Boost::headers, not separately

add_library(Boost::asio ALIAS Boost::headers)
add_library(Boost::assert ALIAS Boost::headers)
add_library(Boost::config ALIAS Boost::headers)
add_library(Boost::pfr ALIAS Boost::headers)
add_library(Boost::core ALIAS Boost::headers)
add_library(Boost::describe ALIAS Boost::headers)
add_library(Boost::endian ALIAS Boost::headers)
add_library(Boost::intrusive ALIAS Boost::headers)    
add_library(Boost::mp11 ALIAS Boost::headers)
add_library(Boost::optional ALIAS Boost::headers)
add_library(Boost::throw_exception ALIAS Boost::headers)
add_library(Boost::variant2 ALIAS Boost::headers)

cpmaddpackage(
    NAME boost_mysql
    GITHUB_REPOSITORY boostorg/mysql
    GIT_TAG boost-1.86.0
    SYSTEM
)