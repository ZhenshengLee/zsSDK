add_library(zsa_astra SHARED IMPORTED GLOBAL)
add_library(zsa_astra_core SHARED IMPORTED GLOBAL)
add_library(zsa_astra_core_api SHARED IMPORTED GLOBAL)

set(ASTRA_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/)

set_target_properties(zsa_astra PROPERTIES IMPORTED_LOCATION ${ASTRA_LIB_DIR}/libastra.so )
set_target_properties(zsa_astra_core PROPERTIES IMPORTED_LOCATION ${ASTRA_LIB_DIR}/libastra_core.so )
set_target_properties(zsa_astra_core_api PROPERTIES IMPORTED_LOCATION ${ASTRA_LIB_DIR}/libastra_core_api.so )

# Define alias for other targets to link against
add_library(zsainternal::astra ALIAS zsa_astra)
add_library(zsainternal::astra_core ALIAS zsa_astra_core)
add_library(zsainternal::astra_core_api ALIAS zsa_astra_core_api)

#copy Astra SDK binaries to lib dir
set(ASTRA_LIB_COPY_TARGET ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
add_custom_target(copy-astra-libs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASTRA_LIB_DIR}/
    ${ASTRA_LIB_COPY_TARGET})
set_target_properties(copy-astra-libs PROPERTIES FOLDER CMakeCopyTargets)
