# Quick Start for Developers

This guide provides short information for developers to start to work with scp-machine quickly. You can always learn more about the scp-machine's [build system](build_system.md).

## Check CMake

Ensure you are using **CMake version 3.24** or newer. Verify your version with:

```sh
cmake --version
```

To upgrade CMake, follow the installation guide appropriate for your OS or use:
  
```sh
# Use pipx to install cmake if not already installed
# Install pipx first using guide: https://pipx.pypa.io/stable/installation/
pipx install cmake
pipx ensurepath
# relaunch your shell after installation
```

## Start develop scp-machine with Conan

### Install Conan

Install Conan, to build scp-machine dependencies with Conan-provided dependencies:

```sh
# Use pipx to install conan if not already installed
pipx install conan
pipx ensurepath
# relaunch your shell after installation
```

### Use scp-machine in Debug

#### Install dependencies with Conan

scp-machine is an extension to sc-machine, so sc-machine is main dependency for scp-machine. To install it, run in the root of the project:

```sh
conan install . -s build_type=Debug
```

#### Build scp-machine in Debug

To build scp-machine in debug mode using Conan-provided dependencies, run:

```sh
# debug build type
cmake --preset debug-conan
cmake --build --preset debug
```

Note: By default, configure preset `debug` enables building scp-machine tests.

#### Run scp-machine tests in Debug

After that, you can go to `build/Debug` and run tests via `ctest`:

```sh
cd build/Debug
ctest -V
```

You can also check code formatting. To learn more, go to the [CMake flags](cmake_flags.md) page.

### Use scp-machine in Release

#### Install dependencies with Conan

To install it, run in the root of the project:

```sh
conan install . -s build_type=Release
```

#### Build scp-machine in Release

To build scp-machine in release mode using Conan-provided dependencies, run:

```sh
# release build type without tests
cmake --preset release-conan
cmake --build --preset release
```

To build scp-machine with tests in release mode using Conan-provided dependencies, run:

```sh
# release build type with tests
cmake --preset release-with-tests-conan
cmake --build --preset release
```

#### Run scp-machine tests in Release

After that, you can run tests:

```sh
cd build/Release
ctest -V
```

You can also check code formatting. To learn more, go to the [CMake flags](cmake_flags.md) page.

### Run scp-machine

Before launching scp-machine, [extract sc-machine from GitHub Releases](https://ostis-ai.github.io/sc-machine/quick_start/) or [build it](https://ostis-ai.github.io/sc-machine/build/quick_start/).

To launch scp-machine, run:

```sh
# create empty knowledge base sources folder
mkdir kb
# note: at this stage you can move your KB sources to the ./kb folder

cd sc-machine-<version>-<platform>
# build knowledge base
./bin/sc-builder -i ../kb -o ../kb.bin --clear
# run sc-machine with scp-machine
./bin/sc-machine -s ../kb.bin \
    -e "path/to/scp-machine/build/<Release|Debug>/lib/extensions"
```
