#
# Try to find Assimp library and include path.
# Once done this will define
#
# ASSIMP_FOUND
# ASSIMP_INCLUDE_PATH
# ASSIMP_LIBRARY
#

IF (MINGW)
    FIND_PATH( ASSIMP_INCLUDE_PATH assimp/defs.h
        ${CMAKE_SOURCE_DIR}/dependencies/assimp/include
    )

    FIND_LIBRARY( ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW/libassimp.dll
        ${PROJECT_BINARY_DIR}/bin/
    )

ELSEIF (MSVC)
    FIND_PATH( ASSIMP_INCLUDE_PATH assimp/defs.h
        ${CMAKE_SOURCE_DIR}/dependencies/assimp/include
    )

    FIND_LIBRARY( ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/assimp_release-dll_win32
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/assimp_release-dll_win32/Assimp32.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/
        )
    endforeach()

ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")

  #set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp_linux/include")
  #set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW/libassimp.a")

  set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp/include/")
  FIND_LIBRARY(ASSIMP_LIBRARY
       NAMES assimp
)

ELSE()

    FIND_PATH(ASSIMP_INCLUDE_PATH assimp/defs.h)
    FIND_LIBRARY(ASSIMP_LIBRARY
        NAMES assimp
    PATH_SUFFIXES dynamic)

ENDIF ()



SET(ASSIMP_FOUND "NO")
IF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
	SET(ASSIMP_LIB ${ASSIMP_LIBRARY})
	SET(ASSIMP_FOUND "YES")
    message("EXTERNAL LIBRARY 'ASSIMP' FOUND")
    message("ASSIMP_LIBRARY: " ${ASSIMP_LIBRARY})
    message("ASSIMP_INCLUDE_PATH: " ${ASSIMP_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'ASSIMP' NOT FOUND")
ENDIF (ASSIMP_INCLUDE_PATH AND ASSIMP_LIBRARY)
