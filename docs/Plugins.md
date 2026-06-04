# Plugins

SurfaceEditor supports modular functionality through plugins. Plugins are built as shared libraries and loaded at runtime from the `plugins` directory.

## How plugins are discovered

The application loads plugins on startup from the `plugins` folder in the current working directory:

```cpp
PluginLoader::LoadPlugins("plugins");
```

On Linux, only files with the `.so` extension are scanned.

## Build behavior

The root `CMakeLists.txt` scans the `Plugins/*` folder and creates a CMake option for each plugin:

- `BUILD_PLUGIN_<PluginName>`

When `CI_BUILD` is `OFF` (default), each plugin option defaults to `ON`.
When `CI_BUILD` is `ON`, plugin options default to `OFF`.

### Build all plugins

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

This builds the main app and all plugin libraries. Built plugin libraries are output to:

```bash
build/plugins
```

### Build specific plugins only

Enable or disable plugin targets with CMake options:

```bash
cmake -S . -B build \
  -DBUILD_PLUGIN_ImportMeshesPlugin=ON \
  -DBUILD_PLUGIN_AddPlanePlugin=OFF
cmake --build build
```

## Runtime use

Run the executable from the build directory so the relative `plugins` folder can be found:

```bash
cd build
./SurfaceEditor
```

The app prints the current working directory and then loads any plugin `.so` files from `build/plugins`.

## Plugin implementation

Each plugin must:

- implement `IPlugin`
- export a `CreatePlugin` factory function
- register itself during `OnLoad()`

Example skeleton:

```cpp
#include "../../src/Core/PluginAPI.h"

class MyPlugin : public IPlugin
{
public:
    void OnLoad() override
    {
        setupPlugin("MY_COMMAND", "MY_CALLBACK", "MY_TOOL");
    }

    void OnUnload() override
    {
        // unregister logic if needed
    }

    std::string GetName() const override
    {
        return "MyPlugin";
    }
};

extern "C" IPlugin* CreatePlugin()
{
    return new MyPlugin();
}
```

## Plugin types

The repository currently uses two registration helpers:

- `setupPlugin(command_id, callback_id, tool_id = "")`
- `setupLayerPlugin(layer_id, name, useWindowLateyBus)`

### `setupPlugin`

Use this for command/callback/tool-based plugins. For example:

```cpp
setupPlugin("IMPORT_MESHES_COMMAND", "IMPORT_MESHES_CALLBACK");
```

### `setupLayerPlugin`

Use this for layer plugins that register UI or renderer layers. For example:

```cpp
setupLayerPlugin("OUTLINER_LAYER", "OutlinerLayer", true);
```

## Plugin sorting

Loaded plugins are sorted by `GetBuildOrderPriority()` before `OnLoad()` is called. Higher priority plugins are loaded first.

## Notes

- `IPlugin::OnUnload()` is defined by the interface, but plugin unloading is not currently performed by the loader.
- The current system is Linux-focused because `PluginLoader` uses `dlopen` and only scans `.so` files.
- Plugin source directories typically include a CMakeLists file that links against `SurfaceEditorCore`.
