# Registry-only milestone check for 1,000 existing leaf implementations entering canonical progress.
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(NAME canonical_2000_registry COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/verification/verify_canonical_2000.py --registry)
