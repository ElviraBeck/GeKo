set(OpenAL_PREFIX "${DEPENDENCIES_PATH}/OpenAL")
set(OpenAL_INSTALL_DIR "${DEPENDENCIES_PATH}/OpenAL")
set(OpenAL_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${OpenAL_INSTALL_DIR})
	   
ExternalProject_Add (
    OpenAL
	PREFIX ${OpenAL_PREFIX}
    GIT_REPOSITORY "https://github.com/JogAmp/openal-soft"
    GIT_TAG "master"
	INSTALL_DIR ${OpenAL_INSTALL_DIR}
    UPDATE_COMMAND ""
    INSTALL_COMMAND "" 
	CMAKE_ARGS ${OpenAL_CMAKE_ARGS}
)

IF (MINGW)
	set(OPENAL_INCLUDE_PATH "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL/include")
	set(OPENAL_LIB "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL-build/libOpenAL32.dll.a")
	
ELSEIF (MSVC)
    set(OPENAL_INCLUDE_PATH "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL/include")
	set(OPENAL_LIB "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL-build/$<CONFIGURATION>/OpenAL32.lib")
	
ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")
	set(OPENAL_INCLUDE_PATH "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL/include")
	set(OPENAL_LIB "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL-build/libopenal.so")

ELSE()
	set(OPENAL_INCLUDE_PATH "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL/include")
	set(OPENAL_LIB "${DEPENDENCIES_PATH}/OpenAL/src/OpenAL-build/libOpenAL32.dll.a")

ENDIF ()
