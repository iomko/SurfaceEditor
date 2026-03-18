#pragma once
#include <string>

void setupLayerPlugin(const int layer_id, const std::string name, bool useWindowLateyBus);
void setupPlugin(const int command_id, const int callback_id, const int tool_id = -1);

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