# Quick start

## Use scp-machine as an extension to sc-machine

### GitHub Releases

Note: currently, using scp-machine natively on Windows isn't supported.

You can download pre-built artifacts from [GitHub Releases](https://github.com/ostis-ai/scp-machine/releases). Extract it to any location, then specify the folder path to `lib/extensions` when running the sc-machine binary:

```sh
./bin/sc-machine -s ../kb.bin \
    -e "path/to/extracted/scp-machine/lib/extensions;path/to/other/extensions"
```

To learn more about how to install and use sc-machine artifacts, see [sc-machine quick start](https://ostis-ai.github.io/sc-machine/quick_start/).

In case you want to make changes to the project sources, please refer to the [build system docs](https://ostis-ai.github.io/scp-machine/build/build_system/).
