cmake_minimum_required(VERSION 2.8)
include(${CMAKE_MODULE_PATH}/DefaultInclude.cmake)

file(GLOB_RECURSE HEADER *.h)
file(GLOB_RECURSE CPP *.cpp)
file(GLOB_RECURSE HPP *.hpp)
SOURCE_GROUP(" " FILES 	${HEADER} ${CPP} ${HPP})