# One coordinator's contiguous batch; no duplicate standalone implementation.
target_sources(re5_recovered PRIVATE src/recovered/batch_00408b50.cpp)
add_executable(re5_test_batch_00408b50 tests/unit/test_batch_00408b50.cpp)
target_link_libraries(re5_test_batch_00408b50 PRIVATE re5_recovered)
add_test(NAME batch_00408b50 COMMAND re5_test_batch_00408b50)
