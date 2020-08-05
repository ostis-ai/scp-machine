project (scp)
cmake_minimum_required (VERSION 2.8)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

set(CMAKE_BUILD_TYPE Debug)

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

# find dependencies
if (${UNIX})
    include(FindPkgConfig)
    pkg_check_modules (GLIB2 REQUIRED glib-2.0)
    pkg_check_modules (GLIB2_MODULE REQUIRED gmodule-2.0)

    set (GLIB2_INCLUDE_DIRS "${GLIB2_INCLUDE_DIRS}" "/usr/lib/x86_64-linux-gnu/glib-2.0/include/" "${GLIB2_MODULE}")
    set (GLIB2_LIBRARIES "${GLIB2_LIBRARIES}" "${GLIB2_MODULE_LIBRARIES}")

    add_definitions(-DSC_PLATFORM_UNIX)

    # for std::thread support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

elseif (${WIN32})
    set(PACKAGES_PATH "${CMAKE_BINARY_DIR}/packages")

    if (${MSVC_VERSION} EQUAL "1800")
        set (MSVC_SUFFIX "vc120")
    elseif (${MSVC_VERSION} EQUAL "1900")
        set (MSVC_SUFFIX "vc140")
    elseif (${MSVC_VERSION} EQUAL "1911")
        set (MSVC_SUFFIX "vc141")
    endif()

    set (GLIB_PATH "${PACKAGES_PATH}/glib.2.36.2.11/build/native")
    set (GLIB2_INCLUDE_DIRS "${GLIB_PATH}/include"
                            "${GLIB_PATH}/include/glib")
    link_directories("${GLIB_PATH}/lib/v110/x64/Release/dynamic/")

    set (GLIB2_LIBRARIES "gio-2.0.lib" "glib-2.0.lib" "gmodule-2.0.lib" "gobject-2.0.lib" "gthread-2.0.lib")

    add_definitions(-DSC_PLATFORM_WIN)

    # copy runtime libraries
    set (GLIB_RUNTIME_PATH "${PACKAGES_PATH}/glib.redist.2.36.2.11/build/native/bin/v110/x64/Release/dynamic")
    set (WIN_RUNTIME_LIBRARIES
            "${GLIB_RUNTIME_PATH}/glib-2-vs10.dll"
            "${GLIB_RUNTIME_PATH}/gmodule-2-vs10.dll"
            "${GLIB_RUNTIME_PATH}/gio-2-vs10.dll"
            "${GLIB_RUNTIME_PATH}/gobject-2-vs10.dll"
            )
    file(COPY ${WIN_RUNTIME_LIBRARIES} DESTINATION "${SC_BIN_PATH}")

endif(${UNIX})

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DSC_DEBUG -DSC_PROFILE")

include_directories(${SC_MEMORY_SRC} ${SC_MACHINE_ROOT})
link_directories(${SC_BIN_PATH})

set(SC_EXTENSION_DIRECTORY "${SC_BIN_PATH}/extensions")
file(MAKE_DIRECTORY ${SC_EXTENSION_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSION_DIRECTORY})  

add_subdirectory(scp)
