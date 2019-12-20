add_executable(zsa_astra_test test.cpp)
target_include_directories(zsa_astra_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(zsa_astra_test PRIVATE
    zsainternal::astra
    zsainternal::astra_core
    zsainternal::astra_core_api
    gtest::gtest
)

zsa_add_tests(TARGET zsa_astra_test HARDWARE_REQUIRED TEST_TYPE FUNCTIONAL)
