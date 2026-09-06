cmake_minimum_required(VERSION 3.20)
project(BioHazardVResurrection LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(re5_recovered STATIC
    src/runtime/bootstrap.cpp
    src/recovered/pilot_bb9070.cpp
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
