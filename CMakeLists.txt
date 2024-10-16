cmake_minimum_required(VERSION 3.11)
project(scp-machine)
set(CMAKE_CXX_STANDARD 17)
get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

set(SCP_MACHINE_CMAKE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(NOT SC_BIN_PATH)
    set(SC_BIN_PATH "${CMAKE_BINARY_DIR}/../bin")
endif()
set(SC_EXTENSIONS_DIRECTORY "${SC_BIN_PATH}/extensions")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
link_directories(${SC_BIN_PATH} ${SC_EXTENSIONS_DIRECTORY})

set(SC_MACHINE_PATH "${PARENT_DIR}/sc-machine")
set(SC_MEMORY_PATH "${SC_MACHINE_PATH}/sc-memory/sc-memory/include")
set(SC_CORE_PATH "${SC_MACHINE_PATH}/sc-memory/sc-core/include")
set(SC_BUILDER_PATH "${SC_MACHINE_PATH}/sc-tools/sc-builder/include")

option(SCP_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)
option(SC_BUILD_TESTS "Flag to build unit tests" OFF)

if(${SCP_CLANG_FORMAT_CODE})
    include(${SCP_MACHINE_CMAKE_PATH}/ClangFormat.cmake)
endif()

if(${SC_BUILD_TESTS})
    include(CTest)
    enable_testing()
    include(GoogleTest)
    find_package(GTest REQUIRED)
    include(${SCP_MACHINE_CMAKE_PATH}/tests.cmake)
endif()

if(${WIN32})
    message(SEND_ERROR "scp-machine isn't supported on Windows OS.")
endif(${WIN32})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")  ## Strip binary
endif()

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
endforeach(OUTPUT_CONFIG)

add_subdirectory(scp)
