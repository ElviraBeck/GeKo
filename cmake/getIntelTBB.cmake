set(TBB_VERSION "tbb43_20140724oss")
set(TBB_PREFIX "${DEPENDENCIES_PATH}/TBB")
set(TBB_BINARY_DIR "${TBB_PREFIX}/TBB-Build")
set(TBB_SOURCE_DIR "${TBB_PREFIX}/TBB")
set(TBB_STAMP_DIR "${TBB_PREFIX}/Stamp")
set(TBB_TEMP_DIR "${TBB_PREFIX}/Tmp")
set(TBB_INSTALL_DIR "${TBB_PREFIX}")
set(TBB_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TBB_INSTALL_DIR})

#if(MSVC)
	#set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/windows/${TBB_VERSION}_win.zip")
#elseif(MINGW)
	#set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/source/${TBB_VERSION}_src.tgz")
#elseif("${CMAKE_SYSTEM}" MATCHES "Linux")
#	set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/linux/${TBB_VERSION}_lin.tgz")
#endif()

if(MSVC)
	set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/windows/${TBB_VERSION}_win.zip")
	
	if (MSVC90)
		set(_TBB_COMPILER "vc9")
	endif(MSVC90)
	if(MSVC10)
		set(_TBB_COMPILER "vc10")
	endif(MSVC10)
	if (MSVC11)
		set(_TBB_COMPILER "vc11")
	endif(MSVC11)
	if(MSVC12)
		set(_TBB_COMPILER "vc12")
	endif(MSVC12)
	
	ExternalProject_Add( TBB
		#--Download step--------------
		URL ${TBB_URL}
		URL_MD5
		#--Update/Patch step----------
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
		#--Configure step-------------
		SOURCE_DIR "${TBB_SOURCE_DIR}"
		CONFIGURE_COMMAND ""
		#--Build step-----------------
		BINARY_DIR "${TBB_BINARY_DIR}"
		BUILD_COMMAND ""
		#--Install step-----------------
		INSTALL_DIR "${TBB_INSTALL_DIR}"
		INSTALL_COMMAND ""
		CMAKE_ARGS ${glew_CMAKE_ARGS}
		DEPENDS ${TBB_DEPENDENCIES}
	)
	
	set(TBB_INCLUDE_PATH "${TBB_SOURCE_DIR}/include")
	set(TBB_LIB "${TBB_SOURCE_DIR}/lib/ia32/${_TBB_COMPILER}/tbb.lib")
	
elseif(MINGW)
	set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/source/${TBB_VERSION}_src.tgz")
	#set(TBB_BUILD_COMMAND mingw32-make.exe -compiler=gcc Makefile)
	set(TBB_BUILD_COMMAND mingw32-make.exe compiler=gcc)
	#set(TBB_BUILD_COMMAND mingw32-make.exe tbb_root=${TBB_PREFIX} tbb_build_dir=${TBB_BINARY_DIR}/lib tbb_build_prefix=gcc Makefile)
	
	ExternalProject_Add( TBB
		#--Download step--------------
		URL ${TBB_URL}
		URL_MD5
		#--Update/Patch step----------
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
		#--Configure step-------------
		SOURCE_DIR "${TBB_SOURCE_DIR}"
		CONFIGURE_COMMAND ""
		#--Build step-----------------
		BINARY_DIR "${TBB_BINARY_DIR}"
		BUILD_COMMAND ${TBB_BUILD_COMMAND}
		#BUILD_COMMAND ""
		#--Install step-----------------
		INSTALL_DIR "${TBB_INSTALL_DIR}"
		INSTALL_COMMAND ""
		CMAKE_ARGS ${glew_CMAKE_ARGS}
		DEPENDS ${TBB_DEPENDENCIES}
	)
	set(TBB_INCLUDE_PATH "${TBB_SOURCE_DIR}/include")
	#set(TBB_LIB "${TBB_BINARY_DIR}/windows_intel64_gcc_mingw4.9.0_debug/tbb.lib")
	
	set_property(DIRECTORY PROPERTY EP_BASE ${TBB_PREFIX})

elseif("${CMAKE_SYSTEM}" MATCHES "Linux")
	set(TBB_URL "https://www.threadingbuildingblocks.org/sites/default/files/software_releases/linux/${TBB_VERSION}_lin.tgz")

	ExternalProject_Add( TBB
		#--Download step--------------
		# GIT_REPOSITORY ""
		# GIT_TAG ""
		URL ${TBB_URL}
		URL_MD5
		#--Update/Patch step----------
		UPDATE_COMMAND ""
		PATCH_COMMAND ""
		#--Configure step-------------
		SOURCE_DIR "${TBB_SOURCE_DIR}"
		CONFIGURE_COMMAND ""
		#--Build step-----------------
		BINARY_DIR "${TBB_BINARY_DIR}"
		BUILD_COMMAND ""
		#--Install step-----------------
		INSTALL_DIR "${TBB_INSTALL_DIR}"
		INSTALL_COMMAND ""
		CMAKE_ARGS ${glew_CMAKE_ARGS}
		DEPENDS ${TBB_DEPENDENCIES}
	)
	set(TBB_INCLUDE_PATH "${TBB_SOURCE_DIR}/include")
	set(TBB_LIB "${TBB_SOURCE_DIR}/lib/ia32/gcc4.4/libtbb.so")
	
endif()



