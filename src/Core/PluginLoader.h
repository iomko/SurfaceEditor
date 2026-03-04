#pragma once

#include <filesystem>
#include <dlfcn.h>
#include <iostream>
#include "PluginAPI.h"

class PluginLoader
{
public:
    static void LoadPlugin(const std::string &path)
    {
        printf("Path: %s \n", path.c_str());
        void *handle = dlopen(path.c_str(), RTLD_NOW);

        if (!handle)
        {
            std::cout << dlerror() << "\n";
            return;
        }

        auto create = (CreatePluginFn)dlsym(handle, "CreatePlugin");

        if (!create)
            return;

        IPlugin *plugin = create();
        plugin->OnLoad();
    }

    static void LoadPlugins(const std::string &directory, std::unordered_set<std::string> *loadedPluginPaths = nullptr)
    {
        printf("Directory: %s \n", directory.c_str());
        for (auto &file : std::filesystem::recursive_directory_iterator(directory))
        {
            if (file.path().extension() == ".so")
            {
                if (loadedPluginPaths && loadedPluginPaths->find(file.path().string()) != loadedPluginPaths->end())
                {
                    continue; // Plugin already loaded, skip it
                }
                LoadPlugin(file.path().string());
                if (loadedPluginPaths)
                {
                    loadedPluginPaths->insert(file.path().string());
                }
            }
        }
    }
};