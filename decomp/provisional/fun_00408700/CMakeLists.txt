cmake_minimum_required(VERSION 3.20)
project(re5_00408700_provisional LANGUAGES CXX)

# Standalone and deliberately NOT included from the canonical root build.
# These tests do not qualify this candidate for FAST_PASS_VALIDATED.
add_library(fun_00408700_candidate STATIC candidate.cpp)
target_compile_features(fun_00408700_candidate PUBLIC cxx_std_20)
target_include_directories(fun_00408700_candidate PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(fun_00408700_candidate_test candidate_test.cpp)
target_link_libraries(fun_00408700_candidate_test PRIVATE fun_00408700_candidate)
foreach(target IN ITEMS fun_00408700_candidate fun_00408700_candidate_test)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endforeach()

enable_testing()
foreach(test IN ITEMS null_guard known_writes preserved_word guard_regions
                      repeated_construction independent_objects byte_patterns)
    add_test(NAME candidate_${test} COMMAND fun_00408700_candidate_test ${test})
endforeach()
