cmake_minimum_required(VERSION 3.10)

project(distributed_tracing)

add_executable(distributed_tracing distributed_tracing.cpp)

target_compile_features(distributed_tracing PUBLIC cxx_std_20)
