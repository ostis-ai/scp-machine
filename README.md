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

```sh
conan install . -s build_type=<Debug|Release>
cmake --preset <configure-preset>
cmake --build --preset <build-preset>
```
