#
# FieldML-API
#

SET(proj FieldML-API)

  
execute_process(COMMAND ${GIT_EXECUTABLE} remote show "${git_protocol}://github.com/b3c/FieldML-API.git"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  RESULT_VARIABLE RESULT)
       

  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  SET(ep_build_shared_libs ON)
  SET(ep_build_testing OFF)
                               
IF(RESULT)
  IF(NOT DEFINED FieldML-API_DIR)
  
  SET(FieldML-API_DEPENDS)
  SET(proj_DEPENDENCIES)
  
  SET(FieldML-API_enabling_variable FieldML-API_LIBRARY)
  MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      DOWNLOAD_DIR "${ep_build_dir}/${proj}"
      CMAKE_GENERATOR ${gen}
      INSTALL_COMMAND ""
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(FieldML-API_DIR ${ep_build_dir}/${proj})
    
    # Since FieldML-API is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

ELSE(RESULT)
  IF(NOT DEFINED FieldML-API_DIR)
    #MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "${git_protocol}://github.com/b3c/FieldML-API.git"
      GIT_TAG "origin/master"
      CMAKE_GENERATOR ${gen}
      INSTALL_COMMAND ""
      CMAKE_ARGS
        ${ep_common_args}
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DBUILD_DOCUMENTATION:BOOL=FALSE
        -DQT_QMAKE_EXECUTABLE:FILEPATH=${QT_QMAKE_EXECUTABLE}
      DEPENDS
        ${proj_DEPENDENCIES}
      )
    SET(FieldML-API_DIR ${ep_build_dir}/${proj})
    
    # Since FieldML-API is statically build, there is not need to add its corresponding 
    # library output directory to CTK_EXTERNAL_LIBRARY_DIRS
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

ENDIF(RESULT)

  SET(${FieldML-API_enabling_variable}_INCLUDE_DIRS FieldML-API_INCLUDE_DIRS)
  SET(${FieldML-API_enabling_variable}_FIND_PACKAGE_CMD FieldML-API)
    


