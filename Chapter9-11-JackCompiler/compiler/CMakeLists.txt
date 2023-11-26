cmake_minimum_required(VERSION 3.5)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories($PROJECT_SOURCE_DIR)

add_executable(jack_tokenizer_test jack_tokenizer_test.cpp)