if (NOT TARGET fastrtps)
###############################################################################
    IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        SET(CMAKE_INSTALL_PREFIX
            ${CMAKE_SOURCE_DIR} CACHE PATH "zsa install prefix" FORCE
        )
    ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    IF(APPLE)
        SET(APPLE_PATCH_COMMAND patch -N -d ${PROJECT_SOURCE_DIR}/thirdparty/fastrtps/src/thirdparty/fastcdr/src/cpp < ${PROJECT_SOURCE_DIR}/fastcdr.patch || "${CMAKE_COMMAND}" -E echo "Patch skipped")
    ELSE()
        SET(APPLE_PATCH_COMMAND "")
    ENDIF()
    include(ExternalProject)
    # for global-like variable
    SET(FAST_RTPS_INSTALL_PREFIX  "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-install" CACHE INTERNAL "FAST_RTPS_INSTALL_PREFIX")
    # SET(FAST_RTPS_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-install")
    ExternalProject_Add(FastRTPS
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/fastrtps/src"
        BINARY_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-build"
        STAMP_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-stamp"
        DOWNLOAD_COMMAND ""
        #UPDATE_COMMAND "${CMAKE_COMMAND}" -E chdir "${PROJECT_SOURCE_DIR}" git submodule update --recursive --init "extern/fastrtps/src"
        UPDATE_COMMAND ""
        PATCH_COMMAND ${APPLE_PATCH_COMMAND}
        INSTALL_DIR "${FAST_RTPS_INSTALL_PREFIX}"
        TMP_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-tmp"
        CMAKE_GENERATOR "${CMAKE_GENERATOR}"
        CMAKE_ARGS
            "-DTHIRDPARTY=ON"
            # "-DEPROSIMA_BUILD=ON"
            "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
            "-DCOMPILE_EXAMPLES=OFF"
            "-DBUILD_JAVA=ON"
            "-DINTERNAL_DEBUG=ON"
            "-DBUILD_SHARED_LIBS=OFF"
            "-DCMAKE_INSTALL_PREFIX=${FAST_RTPS_INSTALL_PREFIX}"
            "-DCMAKE_POSITION_INDEPENDENT_CODE=true"
    )
    IF(WIN32)
        set(FastRTPS_LIBRARIES libfastcdr-1.0 libfastrtps-1.5 Iphlpapi Shlwapi)
    ELSE()
        # for global-like variable
        SET(FastRTPS_LIBRARIES  "fastcdr fastrtps" CACHE INTERNAL "FastRTPS_LIBRARIES")
        # set(FastRTPS_LIBRARIES fastcdr fastrtps)
    ENDIF()
###############################################################################
else()
    message(STATUS "fastrtps is already a target. Skipping adding it twice")
endif()


