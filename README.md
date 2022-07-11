## Clone:

```sh
git clone https://github.com/ostis-ai/scp-machine.git
```

## Build scp-machine
Before build scp-machine make sure that sc-machine is assembled. Sc-machine and scp-machine must be allocated in the same folder.

```sh
cd scp-machine/scripts
./make_all.sh #You can also pass all CMake generation arguments there
```
or, alternatively (requires CMake 3.13+)
```sh
cd scp-machine
cmake -B build -DCMAKE_BUILD_TYPE=Release #or Debug type for development
cmake --build build -j$(nproc) #-j flag for paralleled build process
```
