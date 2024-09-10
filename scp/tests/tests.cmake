make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME scp-tests
        DEPENDS sc-agents-common sc-builder-lib gtest scp
        INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${SC_MACHINE_PATH}/sc-tools/sc-builder/src)
add_definitions(-DSCP_MACHINE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
