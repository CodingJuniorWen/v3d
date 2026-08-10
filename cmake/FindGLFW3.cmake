find_path(GLFW3_INCLUDE_DIR 
    NAMES GLFW/glfw3.h
    PATHS 
        ${CMAKE_SOURCE_DIR}/package/glfw3
    PATH_SUFFIXES 
        include
)

find_library(GLFW3_LIBRARY 
    NAMES glfw3
    PATHS ${CMAKE_SOURCE_DIR}/package/glfw3
    PATH_SUFFIXES lib-vc2022
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3
    REQUIRED_VARS GLFW3_INCLUDE_DIR GLFW3_LIBRARY
    VERSION_VAR glfw3_VERSION
)

if(GLFW3_FOUND AND NOT TARGET GLFW3)
    set(GLFW3_FOUND TRUE)
    add_library(GLFW3 UNKNOWN IMPORTED)
    set_target_properties(GLFW3 PROPERTIES
        IMPORTED_LOCATION "${GLFW3_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${GLFW3_INCLUDE_DIR}"
    )
    message(STATUS "Found GLFW3: ${GLFW3_INCLUDE_DIR} (version ${glfw3_VERSION})")
else()
    message(FATAL_ERROR "Could not find GLFW3")
endif()