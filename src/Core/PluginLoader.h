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
        for (auto &file : std::filesystem::directory_iterator(directory))
        {
            if (file.path().extension() == ".so")
            {
                LoadPlugin(file.path().string());
            }
        }
    }
};