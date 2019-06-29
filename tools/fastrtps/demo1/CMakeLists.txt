message(STATUS "Configuring HelloWorld example...")
file(GLOB HELLOWORLD_EXAMPLE_SOURCES_CXX "*.cxx")
file(GLOB HELLOWORLD_EXAMPLE_SOURCES_CPP "*.cpp")

set(SOURCE_FILES
    # main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

# Include ${CMAKE_CURRENT_BINARY_DIR}/version.rc in the target's sources
# to embed version information
set(ZSA_FILEDESCRIPTION "fastrtps demo1")
set(ZSA_ORIGINALFILENAME "fasttrpsdemo1.exe")
set(ZSA_USE_ICON 0)
configure_file(
    ${ZSA_VERSION_RC}
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
    )

set(EXTERNAL_LIBRARIES
    fastcdr
    fastrtps
)

# On Windows, we need to
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    list(APPEND SOURCE_FILES
        # platform/windows/filesystem17.cpp
    )

    list(APPEND EXTERNAL_LIBRARIES
        # setupapi.lib
    )
else()
    list(APPEND SOURCE_FILES
        # platform/linux/filesystem17.cpp
    )

endif()

add_executable(fasttrpsdemo1 ${SOURCE_FILES} ${HELLOWORLD_EXAMPLE_SOURCES_CXX} ${HELLOWORLD_EXAMPLE_SOURCES_CPP})
target_link_libraries(fasttrpsdemo1 PRIVATE ${EXTERNAL_LIBRARIES})

# Setup install
include(GNUInstallDirs)

install(
    TARGETS
        fasttrpsdemo1
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    COMPONENT
        tools
)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    install(
        FILES
            $<TARGET_PDB_FILE:fasttrpsdemo1>
        DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        COMPONENT
            tools
        OPTIONAL
    )
endif()