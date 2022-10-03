cmake_minimum_required(VERSION 3.10)

project(distributed_tracing)

find_library(GTEST_LIBRARY gtest)

add_compile_options(-fsanitize=address -fsanitize=undefined)
add_link_options(-fsanitize=address -fsanitize=undefined)

enable_testing()

add_executable(distributed_tracing_test distributed_tracing_test.cpp distributed_tracing.cpp)

target_link_libraries(distributed_tracing_test gtest pthread)

include(GoogleTest)
gtest_discover_tests(distributed_tracing_test)

target_compile_features(distributed_tracing_test PUBLIC cxx_std_20)
