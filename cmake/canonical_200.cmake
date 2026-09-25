# Register existing leaf implementations with a focused regression target, not duplicates.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64|AMD64")
    add_executable(re5_canonical_200_tests tests/unit/test_canonical_200_leaves.cpp)
    target_link_libraries(re5_canonical_200_tests PRIVATE re5_leaf323d1aab)
    target_compile_options(re5_canonical_200_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    add_test(NAME canonical_200_properties COMMAND re5_canonical_200_tests properties)
    add_test(NAME canonical_200_guards COMMAND re5_canonical_200_tests guards)
endif()
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(NAME canonical_200_registry COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/verification/verify_canonical_200.py --registry)

# Legacy smoke fixtures contain setup inside assert(). Keep those checks and
# setup active in optimized configurations; never silently compile them out.
if(MSVC)
    target_compile_options(re5_tests PRIVATE /UNDEBUG)
else()
    target_compile_options(re5_tests PRIVATE -UNDEBUG)
endif()
