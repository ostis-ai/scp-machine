make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME scp-tests
    DEPENDS scp sc-builder-lib
    INCLUDES ${SC_MEMORY_PATH} ${SC_BUILDER_PATH}
)
add_definitions(-DSCP_MACHINE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")


if (${SCP_CLANG_FORMAT_CODE})
    target_clangformat_setup(scp-tests)
endif()
