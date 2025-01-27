make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME scp-tests
    DEPENDS sc-machine::sc-builder-lib scp
    INCLUDES ${SCP_SRC}
)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(scp-tests)
endif()
