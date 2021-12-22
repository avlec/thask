cmake_minimum_required(VERSION 3.22.1)

# Thask Library

project(libthask LANGUAGES CXX VERSION 0.0.1)

find_package(Threads REQUIRED)

add_library(libthask STATIC src/thask.cpp)
target_include_directories(libthask
PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(libthask
PRIVATE
  Threads::Threads
)
target_compile_features(libthask PUBLIC cxx_std_20)

# Library Test Executable

find_package(Catch2 3 REQUIRED)

add_executable(thask_tester test/thask_tester.cpp)

target_link_libraries(thask_tester
PRIVATE
  Catch2::Catch2WithMain
  libthask
)
target_compile_features(thask_tester
PUBLIC
  cxx_std_20
)

include(CTest)
include(Catch)
catch_discover_tests(thask_tester)
