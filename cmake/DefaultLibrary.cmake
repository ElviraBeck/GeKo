cmake_minimum_required(VERSION 2.8)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

#Set include folders
include_directories(
	${OpenGL3_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
	${ASSIMP_INCLUDE_PATH}
	${STB_INCLUDE_PATH}
	${OPENAL_INCLUDE_PATH}
	${IMGUI_INCLUDE_PATH}
	#${TBB_INCLUDE_PATH}
	${TinyXML_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${CMAKE_SOURCE_DIR}/src/libraries/
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)
file(GLOB_RECURSE HPP *.hpp)

add_definitions(-DSHADERS_PATH="${SHADERS_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLFW_INCLUDE_GLCOREARB)
add_definitions(-DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)

add_library(${ProjectId} ${SOURCES} ${HEADER} ${HPP})

#Link to library files
target_link_libraries(
    ${ProjectId}
    ${ALL_LIBRARIES}
    ${GLFW3_LIB}
    ${GLEW_LIB}
    ${OpenGL3_LIB}
	${ASSIMP_LIB}
	${OPENAL_LIB}
	#${TBB_LIB}
	${TinyXML_LIB}
)

#Used to delay in build order
add_dependencies(
	${ProjectId}
	glew
	glfw
	glm
	OpenAL
	imgui
	TinyXML
	#TBB
)