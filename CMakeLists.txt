cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(PhotonAnalysis)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
set(CMAKE_CXX_STANDARD 17)
find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
find_package(Boost 1.61 COMPONENTS program_options REQUIRED)
find_package(fmt)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})


set(SRCS
   src/Functions.cxx
   src/Drawer.cxx
   src/Utility.cxx
   src/DrawSlave.cxx
   )

set(HEADERS
   include/Functions.h
   include/Drawer.h
   include/Utility.h
   include/DrawSlave.h
   )

ROOT_GENERATE_DICTIONARY(G__PHA ${HEADERS} LINKDEF include/LinkDef.h)
add_library(PhotoAna SHARED ${SRCS} G__PHA)
target_link_libraries(PhotoAna ${ROOT_LIBRARIES} Boost::program_options fmt::fmt)

# Create main programs using the library

set(EXE_SRCS
   macros/plotPAFile.C
   macros/plotPAFile6chReadout.C
   macros/testReadFile.C
   macros/plotMultiPAFilesStandard.C
   macros/plotSpecific.C
   macros/plotFromCSV.C
   macros/plotFromJson.C
   )

set(EXE_NAMES
   plotPAFile
   plotPAFile6chReadout
   testReadFile
   plotMultiPAFilesStandard
   plotSpecific
   plotFromCSV
   plotFromJson
   )

list(LENGTH EXE_SRCS count)
math(EXPR count "${count}-1")
foreach(i RANGE ${count})
  list(GET EXE_SRCS ${i} src)
  list(GET EXE_NAMES ${i} name)
  add_executable(${name} ${src})
  set_target_properties(${name}
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
  target_link_libraries(${name} PhotoAna)
endforeach()
