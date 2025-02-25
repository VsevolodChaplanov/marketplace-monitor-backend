include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package ${PROJECT_NAME})

install(
    TARGETS ${package}
    EXPORT ${package}Targets
    RUNTIME COMPONENT ${package}_Runtime
    LIBRARY COMPONENT ${package}_Runtime NAMELINK_COMPONENT ${package}_Development
    ARCHIVE COMPONENT ${package}_Development
            FILE_SET cxx_modules_interface
            DESTINATION ${CMAKE_INSTALL_DATADIR}/c++/modules/${package}
            COMPONENT ${package}_Development
            FILE_SET headers
            COMPONENT ${package}_Development)

write_basic_package_version_file(${package}ConfigVersion.cmake COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)

# Allow package maintainers to freely override the path for the configs
set(${package}_INSTALL_CMAKEDIR
    ${CMAKE_INSTALL_DATADIR}/${package}
    CACHE STRING "CMake package config location relative to the install prefix")
set_property(CACHE ${package}_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(${package}_INSTALL_CMAKEDIR)

configure_package_config_file(cmake/install-config.cmake.in ${PROJECT_NAME}Config.cmake
                              INSTALL_DESTINATION ${${package}_INSTALL_CMAKEDIR})

install(FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${${package}_INSTALL_CMAKEDIR})

install(
    FILES ${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake
    DESTINATION ${${package}_INSTALL_CMAKEDIR}
    COMPONENT ${package}_Development)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE Service::
    DESTINATION ${${PROJECT_NAME}_INSTALL_CMAKEDIR}
    COMPONENT ${package}_Development)

if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
