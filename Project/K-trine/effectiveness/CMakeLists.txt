cmake_minimum_required(VERSION 2.8)

project(effectiveness)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++14 -O3")

include_directories(./src)
include_directories(./run)
include_directories(./test)

aux_source_directory(./src INCLUDES)
#set(RUNS ./run/exp.cpp)


add_library(INCLUDES_LIB ${INCLUDES})
#add_library(RUNS_LIB ${RUNS})

add_executable(Chakins ./run/Chakins.cpp)
target_link_libraries(Chakins INCLUDES_LIB)
add_executable(ClusteringCoefficient ./run/ClusteringCoefficient.cpp)
target_link_libraries(ClusteringCoefficient INCLUDES_LIB)
add_executable(Density ./run/Density.cpp)
target_link_libraries(Density INCLUDES_LIB)
add_executable(Casestudy ./run/Casestudy.cpp)
target_link_libraries(Casestudy INCLUDES_LIB)
add_executable(Casestudy_v2 ./run/Casestudy_v2.cpp)
target_link_libraries(Casestudy_v2 INCLUDES_LIB)
add_executable(max ./run/max.cpp)
target_link_libraries(max INCLUDES_LIB)

include_directories(test/include)
include_directories(test/lib)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/test/lib/libgtest.a)
link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/test/lib/libgtest_main.a)

add_executable(
  gtest
  ./test/gtest.cpp
)
target_link_libraries(gtest INCLUDES_LIB)



