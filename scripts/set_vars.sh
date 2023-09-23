#!/usr/bin/env bash

ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export APP_ROOT_PATH="${APP_ROOT_PATH:-${ROOT_PATH}}"
export SCP_MACHINE_PATH="${SCP_MACHINE_PATH:-${APP_ROOT_PATH}}"
export ROOT_CMAKE_PATH="${ROOT_CMAKE_PATH:-${SCP_MACHINE_PATH}}"
export BINARY_PATH="${BINARY_PATH:-${ROOT_CMAKE_PATH}/bin}"
export BUILD_PATH="${BUILD_PATH:-${ROOT_CMAKE_PATH}/build}"
