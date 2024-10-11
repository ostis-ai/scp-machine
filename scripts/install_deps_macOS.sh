#!/usr/bin/env bash
set -eo pipefail

if [[ -z "${SCP_MACHINE_PATH}" || -z "${BINARY_PATH}" || -z "${SCP_MACHINE_BUILD_PATH}" ]];
then
  source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"
fi

brew install googletest
