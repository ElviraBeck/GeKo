project(GeKo)

include(ExternalProject)
include(${CMAKE_MODULE_PATH}/getDoxygen.cmake)
include(${CMAKE_MODULE_PATH}/macros.cmake)

set(CMAKE_CONFIGURATION_TYPES Debug;Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Git REQUIRED)

include(${CMAKE_MODULE_PATH}/getOpenGL3.cmake)
include(${CMAKE_MODULE_PATH}/getGLEW.cmake)
include(${CMAKE_MODULE_PATH}/getGLFW3.cmake)
include(${CMAKE_MODULE_PATH}/getGLM.cmake)
#include(${CMAKE_MODULE_PATH}/getASSIMP.cmake)
include(${CMAKE_MODULE_PATH}/FindASSIMP.cmake)
include(${CMAKE_MODULE_PATH}/getSTB.cmake)
include(${CMAKE_MODULE_PATH}/getOpenAL.cmake)
include(${CMAKE_MODULE_PATH}/getIMGUI.cmake)
include(${CMAKE_MODULE_PATH}/getTinyXML.cmake)

#include(${CMAKE_MODULE_PATH}/getIntelTBB.cmake)


if("${CMAKE_SYSTEM}" MATCHES "Linux")
	find_package(X11)
	set(ALL_LIBRARIES ${ALL_LIBRARIES} ${X11_LIBRARIES} Xrandr Xxf86vm Xi pthread Xcursor Xinerama assimp)
endif()


set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
GENERATE_SUBDIRS(ALL_LIBRARIES ${CMAKE_SOURCE_DIR}/src/libraries)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

GENERATE_SUBDIRS(ALL_EXECUTABLES ${CMAKE_SOURCE_DIR}/src/executables/)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/shaders)
