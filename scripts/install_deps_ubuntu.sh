#!/usr/bin/env bash
set -eo pipefail
if [[ -z "${SCP_MACHINE_PATH}" || -z "${BINARY_PATH}" || -z "${SCP_MACHINE_BUILD_PATH}" ]];
then
  source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"
fi
function usage() {
  cat <<USAGE
Usage: $0 [--dev]
Options:
  --dev:          Installs dependencies required to compile scp-machine
USAGE
  exit 1
}
packagelist_runtime=(
libgtest-dev
)

packages=()
packages+=("${packagelist_runtime[@]}")
while [ "$1" != "" ]; do
  case $1 in
  --dev)
    packages+=("${packagelist_dev[@]}")
    ;;
  -h | --help)
    usage # show help
    ;;
  *)
    usage
    exit 1
    ;;
  esac
  shift 1 # remove the current value for `$1` and use the next
done
if ! command -v apt> /dev/null 2>&1;
then
  RED='\033[22;31m'
  NC='\033[0m' # No Color
  echo -e "${RED}[ERROR] Apt command not found. Debian-based distros are the only officially supported.
Please install the following packages by yourself:
  ${packages[*]}
  ${NC}"
  exit 1
fi
sudo apt-get update && sudo apt-get install -y --no-install-recommends software-properties-common
sudo add-apt-repository -y universe
sudo apt-get update
sudo apt-get install -y --no-install-recommends "${packages[@]}"
sudo apt autoremove
