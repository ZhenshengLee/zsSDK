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
    SET(FAST_RTPS_INSTALL_PREFIX  "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-install" CACHE INTERNAL "FAST_RTPS_INSTALL_PREFIX")
    ExternalProject_Add(FastRTPS
        CMAKE_GENERATOR "${CMAKE_GENERATOR}"
        # EXCLUDE_FROM_ALL true
        BUILD_ALWAYS false
        SOURCE_DIR "${PROJECT_SOURCE_DIR}/extern/fastrtps/src"
        BINARY_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-build"
        STAMP_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-stamp"
        INSTALL_DIR "${FAST_RTPS_INSTALL_PREFIX}"
        TMP_DIR "${PROJECT_BINARY_DIR}/thirdparty/fastrtps-tmp"
        DOWNLOAD_COMMAND ""
        #UPDATE_COMMAND "${CMAKE_COMMAND}" -E chdir "${PROJECT_SOURCE_DIR}" git submodule update --recursive --init "extern/fastrtps/src"
        UPDATE_COMMAND ""
        PATCH_COMMAND ${APPLE_PATCH_COMMAND}
        CMAKE_ARGS
            "-DTHIRDPARTY=ON"
            "-DBUILD_DOCUMENTATION=OFF"
            "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
            "-DCOMPILE_EXAMPLES=OFF"
            "-DINTERNAL_DEBUG=ON"
            "-DCMAKE_INSTALL_PREFIX=${FAST_RTPS_INSTALL_PREFIX}"
            "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
            # failure---assuming bugs
            # "-DEPROSIMA_BUILD=ON"
            # "-DBUILD_SHARED_LIBS=OFF"
            # "-DBUILD_JAVA=ON"
    )
    IF(WIN32)
        set(FastRTPS_LIBRARIES libfastcdr-1.0 libfastrtps-1.5 Iphlpapi Shlwapi)
    ELSE()
        SET(FastRTPS_LIBRARIES  "pthread fastcdr fastrtps" CACHE INTERNAL "FastRTPS_LIBRARIES")
    ENDIF()
###############################################################################
else()
    message(STATUS "fastrtps is already a target. Skipping adding it twice")
endif()


