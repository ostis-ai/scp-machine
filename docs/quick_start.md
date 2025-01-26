# Quick Start

## Use scp-machine as an extension to sc-machine

### GitHub Releases

Note: currently, using scp-machine natively on Windows isn't supported.

You can download pre-built artifacts from [GitHub Releases](https://github.com/ostis-ai/scp-machine/releases). Extract it to any location, then specify the folder path to `lib/extensions` when running the sc-machine binary:

```sh
# create empty knowledge base sources folder
mkdir kb
# note: at this stage you can move your KB sources to the ./kb folder

cd sc-machine-<version>-<platform>
# build knowledge base
./bin/sc-builder -i ../kb -o ../kb.bin --clear
# run sc-machine with scp-machine and sc-machine extensions
./bin/sc-machine -s ../kb.bin \
    -e "path/to/extracted/scp-machine/lib/extensions;lib/extensions"
# if it needs you can provide several paths to extensions separated 
# with semicolon
```

To learn more about how to install and use sc-machine artifacts, see [sc-machine quick start](https://ostis-ai.github.io/sc-machine/quick_start/).

In case you want to make changes to the project sources, please refer to the [build system docs](build/build_system.md).
