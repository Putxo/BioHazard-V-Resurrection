cmake_minimum_required(VERSION 3.20)
project(BioHazardVResurrection LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(re5_recovered STATIC
    src/runtime/bootstrap.cpp
    src/runtime/host_runtime.cpp
    src/recovered/pilot_bb9070.cpp
    src/recovered/region_00401000.cpp
    src/recovered/fun_004014c0.cpp
    src/recovered/fun_004015d0.cpp
    src/recovered/fun_00401640.cpp
    src/recovered/fun_00401650.cpp
    src/recovered/fun_00401670.cpp
    src/recovered/fun_004016c0.cpp
    src/recovered/fun_00401730.cpp
    src/recovered/fun_004017b0.cpp
    src/recovered/fun_00401830.cpp
    src/recovered/fun_00401860.cpp
    src/recovered/fun_00401880.cpp
    src/recovered/fun_004018c0.cpp
    src/recovered/fun_004018f0.cpp
    src/recovered/fun_00401910.cpp
    src/recovered/fun_00401940.cpp
    src/recovered/fun_00401950.cpp
    src/recovered/fun_00401b50.cpp
    src/recovered/fun_00401d10.cpp
    src/recovered/fun_00401d50.cpp
)
target_include_directories(re5_recovered PUBLIC include)

if(MSVC)
    target_compile_options(re5_recovered PRIVATE /W4)
else()
    target_compile_options(re5_recovered PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
add_executable(re5_tests
    tests/unit/test_smoke.cpp
    tests/unit/test_pilot_bb9070.cpp
    tests/unit/test_region_00401000.cpp
    tests/unit/test_fun_004014c0.cpp
    tests/unit/test_fun_004015d0.cpp
    tests/unit/test_fun_00401640.cpp
    tests/unit/test_fun_00401650.cpp
    tests/unit/test_fun_00401670.cpp
    tests/unit/test_fun_004016c0.cpp
    tests/unit/test_fun_00401730.cpp
    tests/unit/test_fun_004017b0.cpp
    tests/unit/test_fun_00401830.cpp
    tests/unit/test_fun_00401860.cpp
    tests/unit/test_fun_00401880.cpp
    tests/unit/test_fun_004018c0.cpp
    tests/unit/test_fun_004018f0.cpp
    tests/unit/test_fun_00401910.cpp
    tests/unit/test_fun_00401940.cpp
    tests/unit/test_fun_00401950.cpp
    tests/unit/test_fun_00401b50.cpp
    tests/unit/test_fun_00401d10.cpp
    tests/unit/test_fun_00401d50.cpp
)
target_link_libraries(re5_tests PRIVATE re5_recovered)
if(MSVC)
    target_compile_options(re5_tests PRIVATE /W4)
else()
    target_compile_options(re5_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
add_test(NAME smoke COMMAND re5_tests)

add_custom_target(metadata_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Metadata tests are executed by Python validation tooling"
)
