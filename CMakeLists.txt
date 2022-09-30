cmake_minimum_required(VERSION 3.10)

project(distributed_tracing)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
include(googletest)

include_directories(${googletest_INCLUDE_DIRS} ${googletest_SOURCE_DIR}/googletest/include)

link_libraries(gtest_main)

add_executable(distributed_tracing distributed_tracing.cpp)
add_executable(distributed_tracing_test distributed_tracing_test.cpp)
gtest_discover_tests(distributed_tracing_test)
target_compile_features(distributed_tracing PUBLIC cxx_std_20)
target_compile_features(distributed_tracing_test PUBLIC cxx_std_20)
