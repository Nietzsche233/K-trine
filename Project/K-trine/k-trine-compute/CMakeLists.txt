cmake_minimum_required(VERSION 2.8)

project(k-trine-compute)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -O3")

include_directories(./src)
include_directories(./run)
include_directories(./test)

aux_source_directory(./src INCLUDES)
#set(RUNS ./run/exp.cpp)


add_library(INCLUDES_LIB ${INCLUDES})
#add_library(RUNS_LIB ${RUNS})

add_executable(main ./run/main.cpp)
target_link_libraries(main INCLUDES_LIB)

include_directories(test/include)
include_directories(test/lib)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/test/lib/libgtest.a)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/test/lib/libgtest_main.a)

add_executable(
  gtest
  ./test/gtest.cpp
)
target_link_libraries(gtest INCLUDES_LIB)



