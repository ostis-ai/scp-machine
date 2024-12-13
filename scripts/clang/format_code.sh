#!/usr/bin/env bash
set -eo pipefail

cmake --preset release-with-tests-conan -DSCP_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat
