#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/../set_vars.sh"

cmake -B "${SCP_MACHINE_BUILD_PATH}" -DSCP_CLANG_FORMAT_CODE=ON "${ROOT_PATH}"
cmake --build "${SCP_MACHINE_BUILD_PATH}" --target clangformat "${ROOT_PATH}"
