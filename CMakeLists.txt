##################################################################################
#
# Main CMakeList.txt 

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR}/cmake/platform ${CMAKE_MODULE_PATH})

#DigitalSignage project
PROJECT(DS CXX C ASM-ATT) 
SET(DS_VERSION 1.0.0)

SET(EXECUTABLE_OUTPUT_PATH ${DS_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${DS_BINARY_DIR}/lib)

INCLUDE(CMakeDependentOption)
INCLUDE(Option)
INCLUDE(Common)
INCLUDE(Toolchain)
INCLUDE(GlobalCompilerFlags)
INCLUDE(BuildGNUPackage)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/Projects/${DS_PROJECT})

