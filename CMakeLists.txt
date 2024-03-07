cmake_minimum_required(VERSION 3.18...3.22)

project(pyodb )

#find_package(PythonExtensions REQUIRED)
find_package(PythonInterp  REQUIRED)
execute_process( COMMAND "${Python_EXECUTABLE}" -c "from sysconfig import get_paths as gp; print(gp()['include'])" OUTPUT_VARIABLE Python_INCLUDE_DIR  )
execute_process( COMMAND "${Python_EXECUTABLE}" -c "import importlib.machinery as imp ; print( imp.all_suffixes()[2])" OUTPUT_VARIABLE Module_SUFFIX   )


set ( Python_INCLUDE_DIR   "/usr/local/apps/python3/3.10.10-01/include/python3.10" )

message ( "${Python_INCLUDE_DIR}"   )
include_directories("${Python_INCLUDE_DIR}" ./include )

#set ( pyodb_init     ${CMAKE_CURRENT_SOURCE_DIR}/build/pyodb_init${Module_SUFFIX})
#set ( libdir        /home/cvah/pkg/src/odb_ok/spam/lib  )  

#execute_process(COMMAND /usr/local/apps/python3/3.10.10-01/bin/python3 ${CMAKE_CURRENT_SOURCE_DIR}/setup.py build OUTPUT_FILE build RESULT_VARIABLE 1 )
#add_library ( MODULE      ${CMAKE_CURRENT_SOURCE_DIR}/build/pyodb_init${Module_SUFFIX} DEPENDS 1 )



#python_extension_module(_hello)
#install(TARGETS _hello LIBRARY DESTINATION hello)







#cmake_minimum_required(VERSION 3.18) # Needed to avoid requiring embedded Python libs too
#project(readgsa
#  VERSION 1.0
#  DESCRIPTION "read GSA FILE module"
#  LANGUAGES C Fortran
#)
# Safety net
#if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
#  message(
#    FATAL_ERROR
#      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n"
#  )
#endif()
# Grab Python, 3.8 or newer
#find_package(Python 3.6 REQUIRED  COMPONENTS Interpreter Development.Module NumPy)
# GET THE MODULE SUFFIX 
#execute_process( COMMAND "${Python_EXECUTABLE}" -c  "import importlib.machinery as imp ; print( imp.all_suffixes()[2])" OUTPUT_VARIABLE Module_SUFFIX)
# Print out the discovered paths
#include(CMakePrintHelpers)
#cmake_print_variables(Python_INCLUDE_DIRS)
# GENERATE THE C FILE WRAPPER 
#add_custom_command(
#  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${f2py_module_c}"
#  COMMAND ${Python_EXECUTABLE}  -c " setup.py  build"   
#)
#add_compile_options("-Wno-cpp")
# Set up target
#Python_add_library(${CMAKE_PROJECT_NAME} MODULE WITH_SOABI
#  "${CMAKE_CURRENT_BINARY_DIR}/${f2py_module_c}" # Generated
#  "${F2PY_INCLUDE_DIR}/fortranobject.c"          # From NumPy
#  "${fortran_src_file}"                          # Fortran source(s)
#)
#set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/module)
#install(MODULE  ${CMAKE_BINARY_DIR}/src/${f2py_module_name}${Module_SUFFIX} RUNTIME  DESTINATION ${CMAKE_Fortran_MODULE_DIRECTORY})
#install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/use_odb.sh DESTINATION bin)
# Depend on sources
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Python::NumPy)
#add_dependencies(${CMAKE_PROJECT_NAME} genpyf)
#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${F2PY_INCLUDE_DIR}")
#add_library ( SHARED ${f2py_module_name}${Module_SUFFIX}  )

