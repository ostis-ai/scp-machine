## Clone:

```sh
git clone https://github.com/ostis-ai/scp-machine.git
```

## Prepare knowledge base

In order for scp-machine to interpret scp-programs properly it is required to add some directories from [OSTIS Metasystem knowledge base](https://github.com/ostis-ai/ims.ostis.kb.git) to knowledge base sources, in particular:

```path
ims.ostis.kb/ims/ostis_tech/semantic_network_represent
ims.ostis.kb/ims/ostis_tech/semantic_networks_processing
ims.ostis.kb/ims/ostis_tech/unificated_models
```

## Build scp-machine
Before build scp-machine make sure that sc-machine is assembled. Sc-machine and scp-machine must be allocated in the same folder.
Sc-machine version to use: 0.9.0-Unlock

```sh
./scripts/build_scp_machine.sh #You can also pass all CMake generation arguments there
```
or, alternatively (requires CMake 3.13+)
```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release #or Debug type for development
cmake --build build -j$(nproc) #-j flag for paralleled build process
```
