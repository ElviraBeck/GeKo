if(MSVC)
  option(USE_MINGW "USE MINGW COMPILER" OFF)

  if(USE_MINGW)
	SET(CMAKE_C_COMPILER C:/MinGW/bin/gcc.exe CACHE PATH "If different: Replace this path with your MINGW g++.exe Path")
	SET(CMAKE_CXX_COMPILER C:/MinGW/bin/g++.exe CACHE PATH "If different: Replace this path with your MINGW g++.exe Path")
  endif()

endif()

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ CACHE PATH "Project specific path. Set manually if it was not found.")
set(DEPENDENCIES_PATH ${CMAKE_BINARY_DIR}/dependencies/ CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources/ CACHE PATH "Project specific path. Set manually if it was not found.")
set(SHADERS_PATH ${CMAKE_SOURCE_DIR}/src/shaders/ CACHE PATH "Project specific path. Set manually if it was not found.")

include(${CMAKE_MODULE_PATH}/DefaultProject.cmake)
