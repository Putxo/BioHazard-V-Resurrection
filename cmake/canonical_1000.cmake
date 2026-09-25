# Registry-only milestone check for 800 existing leaf implementations entering canonical progress.
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(NAME canonical_1000_registry COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/verification/verify_canonical_1000.py --registry)
