#pragma once
#include <unordered_map>
#include "Tool.h"
#include <string>
#include <typeinfo>
#include <typeindex>

class ToolRegistry
{
public:
	template<typename ToolT, typename... Args>
	static void registerTool(Args&&... args) {
        m_tools[typeid(ToolT)] = new ToolT(std::forward<Args>(args)...);
	}

	template<typename ToolT>
	static ToolT* getTool() {
		auto it = m_tools.find(std::type_index(typeid(ToolT)));
		if (it != m_tools.end()) {
			return static_cast<ToolT*>(it->second);
		}
		return nullptr;
	}

	static void deleteRegistry()
	{
		for (auto& pair : m_tools) {
			delete pair.second;
		}
	}

private:
	static inline std::unordered_map<std::type_index, ITool*> m_tools;
};
