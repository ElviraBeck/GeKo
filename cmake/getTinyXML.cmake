set(TinyXML_PREFIX "${DEPENDENCIES_PATH}/TinyXML")
set(TinyXML_INSTALL_DIR "${DEPENDENCIES_PATH}/TinyXML")
set(TinyXML_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TinyXML_INSTALL_DIR})
	   
ExternalProject_Add (
    TinyXML
	PREFIX ${TinyXML_PREFIX}
    GIT_REPOSITORY "https://github.com/leethomason/tinyxml2"
    GIT_TAG "master"
	INSTALL_DIR ${TinyXML_INSTALL_DIR}
    UPDATE_COMMAND ""
    INSTALL_COMMAND "" 
	CMAKE_ARGS ${TinyXML_CMAKE_ARGS}
)

IF (MINGW)
	set(TinyXML_INCLUDE_PATH "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML")
	set(TinyXML_LIB "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML-build/libtinyxml2.dll.a")
	
ELSEIF (MSVC)
    set(TinyXML_INCLUDE_PATH "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML")
	set(TinyXML_LIB "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML-build/$<CONFIGURATION>/tinyxml2.lib")

ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")
	set(TinyXML_INCLUDE_PATH "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML")
	set(TinyXML_LIB "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML-build/libtinyxml2.so")

ELSE()
	set(TinyXML_INCLUDE_PATH "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML")
	set(TinyXML_LIB "${DEPENDENCIES_PATH}/TinyXML/src/TinyXML-build/libtinyxml2.dll.a")

ENDIF ()
