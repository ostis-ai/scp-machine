#!/usr/bin/env bash
set -eo pipefail

CURRENT_DIR="$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)"
OSTIS_SCRIPTS_DIR="${CURRENT_DIR}/ostis-scripts"
source "${OSTIS_SCRIPTS_DIR}/message-scripts/messages.sh"

if [[ -z "${SCP_MACHINE_CMAKE_PATH}" || -z "${SCP_MACHINE_CXX_SOURCES_PATH}"
  || -z "${SCP_MACHINE_BUILD_PATH}" || -z "${BINARY_PATH}" ]];
then
  source "${CURRENT_DIR}/set_vars.sh"
fi

info "Build scp-machine"

BUILD_SCRIPTS="${OSTIS_SCRIPTS_DIR}/build-scripts"
"${BUILD_SCRIPTS}/build_cxx_project.sh" -p "${SCP_MACHINE_CMAKE_PATH}" -s "${SCP_MACHINE_CXX_SOURCES_PATH}" \
  -b "${SCP_MACHINE_BUILD_PATH}" --cmake-arg "-DSC_BIN_PATH=${BINARY_PATH}" "$@"

info "scp-machine is built successfully"
