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
    src/recovered/fun_00401e30.cpp
    src/recovered/fun_00401e60.cpp
    src/recovered/fun_00401fe0.cpp
    src/recovered/fun_00402000.cpp
    src/recovered/fun_00402020.cpp
    src/recovered/fun_004020e0.cpp
    src/recovered/fun_00402110.cpp
    src/recovered/fun_00402120.cpp
    src/recovered/fun_00402250.cpp
    src/recovered/fun_004022f0.cpp
    src/recovered/fun_00402480.cpp
    src/recovered/fun_00402560.cpp
    src/recovered/fun_00402b10.cpp
    src/recovered/fun_00403150.cpp
    src/recovered/fun_00403640.cpp
    src/recovered/fun_00403af0.cpp
    src/recovered/fun_00403f50.cpp
    src/recovered/fun_00404e60.cpp
    src/recovered/fun_00404f90.cpp
    src/recovered/fun_00405710.cpp
    src/recovered/fun_00405930.cpp
    src/recovered/fun_00406010.cpp
    src/recovered/fun_00405a60.cpp
    src/recovered/fun_00405bd0.cpp
    src/recovered/fun_00405df0.cpp
    src/recovered/fun_004061f0.cpp
    src/recovered/fun_004064e0.cpp
    src/recovered/fun_00406510.cpp
    src/recovered/fun_00406540.cpp
    src/recovered/fun_00406570.cpp
    src/recovered/fun_004065a0.cpp
    src/recovered/fun_004065d0.cpp
    src/recovered/fun_00406600.cpp
    src/recovered/fun_00406620.cpp
    src/recovered/fun_00406680.cpp
    src/recovered/fun_004066c0.cpp
    src/recovered/fun_004066e0.cpp
    src/recovered/fun_00406700.cpp
    src/recovered/fun_00406720.cpp
    src/recovered/fun_00406740.cpp
    src/recovered/fun_00406760.cpp
    src/recovered/fun_00406780.cpp
    src/recovered/fun_004067a0.cpp
    src/recovered/fun_004067c0.cpp
    src/recovered/fun_00406800.cpp
    src/recovered/fun_004067e0.cpp
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
    tests/unit/test_fun_00401e30.cpp
    tests/unit/test_fun_00401e60.cpp
    tests/unit/test_fun_00401fe0.cpp
    tests/unit/test_fun_00402000.cpp
    tests/unit/test_fun_00402020.cpp
    tests/unit/test_fun_004020e0.cpp
    tests/unit/test_fun_00402110.cpp
    tests/unit/test_fun_00402120.cpp
    tests/unit/test_fun_00402250.cpp
    tests/unit/test_fun_004022f0.cpp
    tests/unit/test_fun_00402480.cpp
    tests/unit/test_fun_00402560.cpp
    tests/unit/test_fun_00402b10.cpp
    tests/unit/test_fun_00403150.cpp
    tests/unit/test_fun_00403640.cpp
    tests/unit/test_fun_00403af0.cpp
    tests/unit/test_fun_00403f50.cpp
    tests/unit/test_fun_00404e60.cpp
    tests/unit/test_fun_00404f90.cpp
    tests/unit/test_fun_00405710.cpp
    tests/unit/test_fun_00405930.cpp
    tests/unit/test_fun_00406010.cpp
    tests/unit/test_fun_00405a60.cpp
    tests/unit/test_fun_00405bd0.cpp
    tests/unit/test_fun_00405df0.cpp
    tests/unit/test_fun_004061f0.cpp
    tests/unit/test_fun_004064e0.cpp
    tests/unit/test_fun_00406510.cpp
    tests/unit/test_fun_00406540.cpp
    tests/unit/test_fun_00406570.cpp
    tests/unit/test_fun_004065a0.cpp
    tests/unit/test_fun_004065d0.cpp
    tests/unit/test_fun_00406600.cpp
    tests/unit/test_fun_00406620.cpp
    tests/unit/test_fun_00406680.cpp
    tests/unit/test_fun_004066c0.cpp
    tests/unit/test_fun_004066e0.cpp
    tests/unit/test_fun_00406700.cpp
    tests/unit/test_fun_00406720.cpp
    tests/unit/test_fun_00406740.cpp
    tests/unit/test_fun_00406760.cpp
    tests/unit/test_fun_00406780.cpp
    tests/unit/test_fun_004067a0.cpp
    tests/unit/test_fun_004067c0.cpp
    tests/unit/test_fun_00406800.cpp
    tests/unit/test_fun_004067e0.cpp
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
