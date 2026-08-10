cmake_minimum_required(VERSION 3.25)

project(VRender 
    VERSION 1.0.0 
    LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/cmake/Configuration.cmake)

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} SHARED)

setup_cpp_target(${PROJECT_NAME} 20)

set(INTER_CONTEXT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/internal/context)
set(INTER_BUFFER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/internal/buffer)
set(INTER_UTILS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/internal/utils)

set(PRIVATE_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/VInstance.cpp

    ${INTER_CONTEXT_SOURCE}/VGpuImpl.cpp
    ${INTER_CONTEXT_SOURCE}/VGpuImpl.h
    ${INTER_CONTEXT_SOURCE}/VInstanceImpl.cpp
    ${INTER_CONTEXT_SOURCE}/VInstanceImpl.h
    ${INTER_CONTEXT_SOURCE}/VPhyDeviceImpl.h
    ${INTER_CONTEXT_SOURCE}/VPhyDeviceImpl.cpp

    ${INTER_UTILS_SOURCE}/QueueFamily.h
    ${INTER_UTILS_SOURCE}/QueueFamily.cpp
    ${INTER_UTILS_SOURCE}/DeviceCapability.h
    ${INTER_UTILS_SOURCE}/DeviceCapability.cpp
    ${INTER_UTILS_SOURCE}/SwapchainUtils.h
    ${INTER_UTILS_SOURCE}/SwapchainUtils.cpp

    ${INTER_BUFFER_SOURCE}/VBufferAllocImpl.h
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VInstance.h
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${PRIVATE_SOURCE}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            ${PUBLIC_HEADER}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/internal>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan Utils)

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
  "${CMAKE_CURRENT_BINARY_DIR}/VRenderVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/VRenderConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VRender
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/VRenderConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/VRenderVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/VRender
)