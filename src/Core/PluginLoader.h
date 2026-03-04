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

    static void LoadPlugins(const std::string &directory)
    {
        printf("Directory: %s \n", directory.c_str());
        for (auto &file : std::filesystem::recursive_directory_iterator(directory))
        {
            if (file.path().extension() == ".so")
            {
                LoadPlugin(file.path().string());
            }
        }
    }
};