cmake_minimum_required(VERSION 3.20)
project(RE5_Attachment_323d1aab LANGUAGES CXX)

# Independent verification target. This does not link a game executable.
set(REPOSITORY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
add_library(attachment_reference STATIC src/recovered.cpp)
target_include_directories(attachment_reference PUBLIC include)
target_compile_features(attachment_reference PUBLIC cxx_std_17)
add_library(canonical_string_wrapper STATIC
  "${REPOSITORY_ROOT}/src/recovered/fun_004022f0.cpp")
target_include_directories(canonical_string_wrapper PUBLIC "${REPOSITORY_ROOT}/include")
target_compile_features(canonical_string_wrapper PUBLIC cxx_std_17)
add_executable(recovered_tests tests/recovered_tests.cpp)
target_link_libraries(recovered_tests PRIVATE attachment_reference canonical_string_wrapper)
add_executable(existing_string_tests tests/canonical_main.cpp
  "${REPOSITORY_ROOT}/tests/unit/test_fun_004022f0.cpp")
target_link_libraries(existing_string_tests PRIVATE canonical_string_wrapper)
foreach(target attachment_reference canonical_string_wrapper recovered_tests existing_string_tests)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX /UNDEBUG)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror -UNDEBUG)
  endif()
endforeach()

enable_testing()
add_test(NAME semantic_and_canonical_regression COMMAND recovered_tests)
if(UNIX)
  add_test(NAME early_exit_guard_pages COMMAND recovered_tests --guards)
endif()
add_test(NAME existing_string_regression COMMAND existing_string_tests)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME locked_target_manifest COMMAND "${Python3_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/verify_target.py" --self-test)
add_test(NAME source_package_hygiene COMMAND "${Python3_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/verify_target.py" --check-source)
