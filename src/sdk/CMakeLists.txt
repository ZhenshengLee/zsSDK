# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

# Create ZSA library
add_library(zsa SHARED
            zsa.c
            dll_main.c
            ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

# Generate zsa_export.h
# This is a platform specific header defining the macros for the export functions
# of the shared library. This header is referenced by zsa.h and needs to be installed
# with the SDK
include(GenerateExportHeader)
generate_export_header(zsa
    EXPORT_FILE_NAME "include/zsa/zsa_export.h")

configure_file(
    "${ZSA_INCLUDE_DIR}/zsa/zsaversion.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/zsa/zsaversion.h"
    )

# Include ${CMAKE_CURRENT_BINARY_DIR}/version.rc in the target's sources
# to embed version information
set(ZSA_FILEDESCRIPTION "zsa SDK")
set(ZSA_ORIGINALFILENAME "zsa.dll")
configure_file(
    ${ZSA_VERSION_RC}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )

# ${ZSA_INCLUDE_DIR} contains zsa/zsa.h which is the primary header consumers should include
# ${CMAKE_CURRENT_BINARY_DIR}/include contains the generated zsa/zsa_export.h
target_include_directories(zsa PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${ZSA_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
    )

# Link in libraries
target_link_libraries(zsa PRIVATE
)

# Define alias for zsa
add_library(zsa::zsa ALIAS zsa)

set_target_properties(
    zsa
    PROPERTIES
        VERSION
            "${ZSA_VERSION_MAJOR}.${ZSA_VERSION_MINOR}.${ZSA_VERSION_PATCH}"
        SOVERSION
            "${ZSA_VERSION_MAJOR}.${ZSA_VERSION_MINOR}")

set(NAMELINK_IF_AVAILABLE)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
    set(NAMELINK_IF_AVAILABLE NAMELINK_COMPONENT development)
endif()

# Setup install
include(GNUInstallDirs)
install(
    TARGETS
        zsa
    EXPORT
        zsaTargets
    LIBRARY
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        COMPONENT
            runtime
        ${NAMELINK_IF_AVAILABLE}
    ARCHIVE
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        COMPONENT
            development
    RUNTIME
        DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        COMPONENT
            runtime
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    zsaConfigVersion.cmake
    VERSION ${ZSA_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

install(
    EXPORT
        zsaTargets
    FILE
        zsaTargets.cmake
    NAMESPACE
        zsa::
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/zsa
    COMPONENT
        development
)

configure_file("${PROJECT_SOURCE_DIR}/cmake/zsaConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/zsaConfig.cmake" COPYONLY)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/zsaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/zsaConfigVersion.cmake"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/zsa
    COMPONENT
        development
)

install(
    FILES
        ${ZSA_INCLUDE_DIR}/zsa/zsa.h
        ${ZSA_INCLUDE_DIR}/zsa/zsa.hpp
        ${ZSA_INCLUDE_DIR}/zsa/zsatypes.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/zsa/zsaversion.h
        ${CMAKE_CURRENT_BINARY_DIR}/include/zsa/zsa_export.h
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/zsa
    COMPONENT
        development
)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(
        FILES
            $<TARGET_PDB_FILE:zsa>
        DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        COMPONENT
            runtime
        OPTIONAL
    )
endif()
