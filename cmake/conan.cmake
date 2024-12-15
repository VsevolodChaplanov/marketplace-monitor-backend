macro(download_conan_provider)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan_provider.cmake")
        message(STATUS "Downloading conan_provider.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/refs/heads/develop2/conan_provider.cmake"
             "${CMAKE_BINARY_DIR}/conan_provider.cmake" # TLS_VERIFY ON # fails on some systems
             SHOW_PROGRESS)
    endif()

    list(APPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_BINARY_DIR}/conan_provider.cmake")
endmacro()
