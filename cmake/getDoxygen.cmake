option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" ON)
option(USE_GRAPHVIZ "Use Doxygen and Dot to create the HTML based API documentation and a UML Diagram" OFF)

if(BUILD_DOCUMENTATION)

  set(DOXYGEN_VERSION "1.8.8")
  set(DOXYGEN_PREFIX "${CMAKE_BINARY_DIR}/Doxygen")
  set(DOXYGEN_BINARY_DIR "${DOXYGEN_PREFIX}/Doxygen-${DOXYGEN_VERSION}-Build")
  set(DOXYGEN_SOURCE_DIR "${DOXYGEN_PREFIX}/Doxygen-${DOXYGEN_VERSION}")
  set(DOXYGEN_STAMP_DIR "${DOXYGEN_PREFIX}/Stamp")
  set(DOXYGEN_TEMP_DIR "${DOXYGEN_PREFIX}/Tmp")
  set(DOXYGEN_INSTALL_DIR "${DOXYGEN_PREFIX}/Doxygen-${DOXYGEN_VERSION}-Install")

  if(WIN32)
    set(DOXYGEN_URL "http://ftp.stack.nl/pub/users/dimitri/doxygen-${DOXYGEN_VERSION}.windows.x64.bin.zip")
  #elseif(APPLE)
  #  set(DOXYGEN_URL "http://ftp.stack.nl/pub/users/dimitri/Doxygen-${DOXYGEN_VERSION}.dmg")
  else()
    set(DOXYGEN_URL "http://ftp.stack.nl/pub/users/dimitri/doxygen-${DOXYGEN_VERSION}.linux.bin.tar.gz")
  endif()
  
  #set_property(DIRECTORY PROPERTY EP_BASE ${DOXYGEN_PREFIX})
  
  #if(APPLE)
  #  set(DOXYGEN_DMG "${DOXYGEN_PREFIX}/Download/Doxygen-${DOXYGEN_VERSION}.dmg")
  #  if(NOT EXISTS "${DOXYGEN_PREFIX}/Download/Doxygen-${DOXYGEN_VERSION}.dmg")
  #    file(DOWNLOAD ${DOXYGEN_URL} "${DOXYGEN_PREFIX}/Download/Doxygen-${DOXYGEN_VERSION}.dmg" SHOW_PROGRESS)
  #  endif()
  #  configure_file(${Dream3DSdkBuild_SOURCE_DIR}/Doxygen_osx_install.sh.in
  #	               ${DOXYGEN_PREFIX}/Download/Install.sh @ONLY)
  #  execute_process(COMMAND "${DOXYGEN_PREFIX}/Download/Install.sh")
  #else()

  ExternalProject_Add( 
    Doxygen
    URL ${DOXYGEN_URL}
    URL_MD5
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    SOURCE_DIR "${DOXYGEN_SOURCE_DIR}"
    CONFIGURE_COMMAND ""
    BINARY_DIR "${DOXYGEN_BINARY_DIR}"
    BUILD_COMMAND ""
    INSTALL_DIR "${DOXYGEN_INSTALL_DIR}"
    INSTALL_COMMAND ""
    DEPENDS ${DOXYGEN_DEPENDENCIES}
  )	
  

  #endif()
  
  IF(WIN32)
	SET(DOXYGEN_EXECUTABLE "${DOXYGEN_SOURCE_DIR}/doxygen.exe")
  ELSEIF("${CMAKE_SYSTEM}" MATCHES "Linux")
        SET(DOXYGEN_EXECUTABLE "${DOXYGEN_SOURCE_DIR}/bin/doxygen")
  ENDIF()
	
  if(USE_GRAPHVIZ)
	message([WARNING] "You choose the option to use Graphviz for diagrams. Be sure to have Graphviz installed properly (@windows users: add it to the path variable)")
    configure_file(${CMAKE_MODULE_PATH}/Doxyfile_graphviz.in 
      ${DOXYGEN_PREFIX}/Doxyfile  @ONLY IMMEDIATE)
  else()
   configure_file(${CMAKE_MODULE_PATH}/Doxyfile.in 
      ${DOXYGEN_PREFIX}/Doxyfile  @ONLY IMMEDIATE)
  endif()
  
  add_custom_target (Docs ALL 
	  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_PREFIX}/Doxyfile
	  DEPENDS Doxygen
	  SOURCES ${DOXYGEN_PREFIX}/Doxyfile
	)
	
endif()