add_executable(astra main.c)

target_include_directories(astra PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(astra PRIVATE
    zsainternal::astra
    zsainternal::astra_core
    zsainternal::astra_core_api
)
