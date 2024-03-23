set(SCP_SRC "${CMAKE_SOURCE_DIR}/scp")

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.hpp")

set(CMAKE_C_FLAGS_DEBUG "-DSCP_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-DSCP_DEBUG")
add_definitions(-DSCP_MATH)
add_definitions(-DSCP_STRING)

add_library(scp SHARED ${SOURCES} ${HEADERS})
target_link_libraries(scp
    PRIVATE sc-memory
    PRIVATE sc-kpm-common
)
target_include_directories(scp
    PRIVATE ${SCP_SRC}
    PRIVATE ${SC_MEMORY_SRC}
)

sc_codegen_ex(scp ${SCP_SRC} "${SCP_SRC}/generated")
