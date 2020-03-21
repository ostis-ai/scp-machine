project (scp)
cmake_minimum_required (VERSION 2.8)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

set(SC_BIN_PATH "${PARENT_DIR}/sc-machine/bin/")
set(SC_MACHINE_ROOT "${PARENT_DIR}/sc-machine/")
set(SC_MEMORY_SRC "${SC_MACHINE_ROOT}/sc-memory/")

# codegen
if (${UNIX})
    set(SC_CODEGEN_TOOL "${SC_BIN_PATH}/sc-code-generator")
elseif (${WIN32})
    set(SC_CODEGEN_TOOL "${SC_BIN_PATH}/sc-code-generator.exe")
endif()
include("${SC_MACHINE_ROOT}/cmake/codegen.cmake")

# for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")  ## Strip binary
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")

include_directories(${SC_MEMORY_SRC} ${SC_MACHINE_ROOT})
link_directories(${SC_BIN_PATH})

set(SC_EXTENSION_DIRECTORY "${SC_BIN_PATH}/extensions")
file(MAKE_DIRECTORY ${SC_EXTENSION_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSION_DIRECTORY})  

add_subdirectory(scp)