#set(assimp_PREFIX "${CMAKE_SOURCE_DIR}/assimp")
#set(assimp_INSTALL_DIR "${CMAKE_SOURCE_DIR}/assimp")
#set(assimp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${assimp_INSTALL_DIR}
					  #-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
					  #-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
					  #-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                                          #-DCMAKE_DEBUG_POSTFIX=d
                                          #-DASSIMP_ENABLE_BOOST_WORKAROUND=ON
                                          #-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
                                          #-DASSIMP_BUILD_TESTS=OFF
					  #-DASSIMP_BUILD_SAMPLES=OFF
					  #-DASSIMP_BUILD_NO_IFC_IMPORTER=ON
   # )
	
	
#ExternalProject_Add (
	#assimp
	#PREFIX ${assimp_PREFIX}
        #GIT_REPOSITORY "https://github.com/assimp/assimp"
        #GIT_TAG "master"
        #GIT_TAG "v3.1.1"
	#INSTALL_DIR ${assimp_INSTALL_DIR}
        #UPDATE_COMMAND ""
        #INSTALL_COMMAND "" 
	#CMAKE_ARGS ${assimp_CMAKE_ARGS}
#)

IF (MINGW)

	#set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/assimp/src/assimp/include")
	set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp/include/")
	set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW/libassimp.a")

ELSEIF (MSVC)

    set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp/include/")
	
	if(CMAKE_BUILD_TYPE MATCHES Debug)
	
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/debug/32bit/assimp.lib")
		else()
			set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/debug/64bit/assimp.lib")
		endif()
	
	else()
	
		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/release/32bit/assimp.lib")
		else()
			set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/lib/release/64bit/assimp.lib")
		endif()
	
	endif()
	
	
	
ELSEIF(APPLE)

    #set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/glfw/src/glfw/include")

ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")
	
  #set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp_linux/include")
  #set(ASSIMP_LIB "${CMAKE_SOURCE_DIR}/dependencies/assimp/libMinGW/libassimp.a")

  set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/dependencies/assimp/include/")
  FIND_LIBRARY(ASSIMP_LIBRARY
       NAMES assimp
	)

#ELSE()

	#set(ASSIMP_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/assimp/src/assimp/include")
	
ENDIF ()


