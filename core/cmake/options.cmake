option(${PROJECT_NAME}_BUILD_EXAMPLES "Build examples for core part of project"
       ${${MAIN_PROJECT_NAME}_BUILD_EXAMPLES})
option(${PROJECT_NAME}_BUILD_TESTS "Build tests for core part of project" ${${MAIN_PROJECT_NAME}_BUILD_TESTS})

option(${PROJECT_NAME}_ENABLE_COVERAGE "Enable coverage flags" ${${MAIN_PROJECT_NAME}_ENABLE_COVERAGE})

option(${PROJECT_NAME}_ENABLE_CLANGTIDY_ON_BUILD "Enable clang-tidy during the build"
       ${${MAIN_PROJECT_NAME}_ENABLE_CLANGTIDY_ON_BUILD})
option(${PROJECT_NAME}_ENABLE_CPPCHECK_ON_BUILD "Enable cppcheck during the build"
       ${${MAIN_PROJECT_NAME}_ENABLE_CPPCHECK_ON_BUILD})
option(${PROJECT_NAME}_ENABLE_IWYU_ON_BUILD "Enable include-what-you-use during the build"
       ${${MAIN_PROJECT_NAME}_ENABLE_IWYU_ON_BUILD})

option(${PROJECT_NAME}_SANITIZER_ADDRESS "Enable SANITIZER_ADDRESS for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_SANITIZER_ADDRESS})
option(${PROJECT_NAME}_SANITIZER_LEAK "Enable SANITIZER_LEAK for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_SANITIZER_LEAK})
option(${PROJECT_NAME}_SANITIZER_UNDEFINED_BEHAVIOR
       "Enable SANITIZER_UNDEFINED_BEHAVIOR for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_SANITIZER_UNDEFINED_BEHAVIOR})
option(${PROJECT_NAME}_SANITIZER_THREAD "Enable SANITIZER_THREAD for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_SANITIZER_THREAD})
option(${PROJECT_NAME}_SANITIZER_MEMORY "Enable SANITIZER_MEMORY for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_SANITIZER_MEMORY})

option(${PROJECT_NAME}_BUILD_DOCS "Enable SANITIZER_MEMORY for the ${PROJECT_NAME} project"
       ${${MAIN_PROJECT_NAME}_BUILD_DOCS})

option(${PROJECT_NAME}_SKIP_INSTALL_RULES "Skip install rules for the ${PROJECT_NAME} project"
       OFF)

option(${PROJECT_NAME}_ENABLE_PCH "Enable precompiled headers for the ${PROJECT_NAME} project" ON)
option(${PROJECT_NAME}_BUILD_SHARED_LIBS "Select library type for the ${PROJECT_NAME} project" ${BUILD_SHARED_LIBS})
