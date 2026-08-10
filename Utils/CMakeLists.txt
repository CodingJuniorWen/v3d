cmake_minimum_required(VERSION 3.25)

project(Utils
    VERSION 1.0.0
    LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/Configuration.cmake)

find_package(spdlog REQUIRED)

# package/spdlog currently only ships RelWithDebInfo binaries
set_target_properties(spdlog::spdlog PROPERTIES
    MAP_IMPORTED_CONFIG_DEBUG RelWithDebInfo
    MAP_IMPORTED_CONFIG_RELEASE RelWithDebInfo
    MAP_IMPORTED_CONFIG_MINSIZEREL RelWithDebInfo
)

add_library(${PROJECT_NAME} SHARED)
add_library(VUtils ALIAS ${PROJECT_NAME})

setup_cpp_target(${PROJECT_NAME} 20)

set(INTER_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/LoggerImpl.cpp
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Logger.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${INTER_SOURCE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            ${PUBLIC_HEADER}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Keep spdlog private so consumers (e.g. VRender) do not depend on it.
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

include(GenerateExportHeader)
generate_export_header(
    ${PROJECT_NAME}
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/Export.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_EXPORTS)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

export(EXPORT ${PROJECT_NAME}Targets
    FILE "${CMAKE_BINARY_DIR}/cmake/${PROJECT_NAME}Targets.cmake"
)
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Utils
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/UtilsVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Utils
)
