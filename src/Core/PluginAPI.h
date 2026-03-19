#pragma once
#include <string>

void setupLayerPlugin(std::string layer_id, const std::string name, bool useWindowLateyBus);
void setupPlugin(std::string command_id, std::string callback_id, std::string tool_id = "");

class IPlugin
{
public:
    virtual ~IPlugin() = default;
    virtual void OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual std::string GetName() const = 0;
    virtual int GetBuildOrderPriority() const { return 0; }
};

using CreatePluginFn = IPlugin* (*)();