#pragma once

#include <filesystem>
#include <dlfcn.h>
#include <iostream>
#include "PluginAPI.h"

class PluginLoader
{
public:
    struct LoadedPlugin
    {
        void *handle;
        IPlugin *plugin;
        std::string path;
    };

    static LoadedPlugin LoadPluginInternal(const std::string& path)
    {
        void* handle = dlopen(path.c_str(), RTLD_NOW);

        if (!handle)
        {
            std::cout << dlerror() << "\n";
            return {nullptr, nullptr, path};
        }

        auto create = (CreatePluginFn)dlsym(handle, "CreatePlugin");

        if (!create)
        {
            std::cout << "CreatePlugin symbol not found\n";
            return {nullptr, nullptr, path};
        }

        IPlugin* plugin = create();

        return {handle, plugin, path};
    }

    static void LoadPlugin(const std::string &path)
    {
        LoadPluginInternal(path).plugin->OnLoad();
    }

    static void LoadPlugins(const std::string &directory)
    {
        printf("Directory: %s \n", directory.c_str());
        std::vector<LoadedPlugin> plugins;

        for (auto &file : std::filesystem::recursive_directory_iterator(directory))
        {
            if (file.path().extension() == ".so")
            {
                auto loaded = LoadPluginInternal(file.path().string());
                if (loaded.plugin)
                    plugins.push_back(loaded);
            }
        }

        std::sort(plugins.begin(), plugins.end(),
                  [](const LoadedPlugin &a, const LoadedPlugin &b)
                  {
                      return a.plugin->GetPriority() > b.plugin->GetPriority();
                  });

        for (auto &p : plugins)
        {
            std::cout << "Loading plugin: " << p.plugin->GetName()
                      << " priority=" << p.plugin->GetPriority() << "\n";

            p.plugin->OnLoad();
        }
    }
};