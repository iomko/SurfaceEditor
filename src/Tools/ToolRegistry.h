#pragma once
#include <unordered_map>
#include "Tool.h"
#include <memory>
#include <unordered_map>
#include <functional>

class ToolRegistry
{
public:
    using Creator = std::function<std::unique_ptr<ITool>(CommandConcept*)>;

	static ToolRegistry& instance() {
        static ToolRegistry registry;
        return registry;
    }

	template<typename ToolT>
    void registerToolType(std::string id) {
        m_creators[id] = [](CommandConcept* cmd) {
            return std::make_unique<ToolT>(cmd);
        };
    }

    void initializeTool(std::string id, CommandConcept* cmd) {
        auto it = m_creators.find(id);
        if (it != m_creators.end()) {
            m_instances[id] = it->second(cmd);
        }
    }

    ITool* getTool(std::string id) {
        auto it = m_instances.find(id);
        if (it != m_instances.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void clear() {
        m_instances.clear();
    }

private:
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<ITool>> m_instances;
};

template<typename ToolT>
struct AutoRegisterTool {
    AutoRegisterTool(std::string id) {
        ToolRegistry::instance().registerToolType<ToolT>(id);
    }
};
