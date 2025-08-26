#pragma once
#include <unordered_map>
#include "Tool.h"
#include <string>

class ToolRegistry
{
public:
	template<typename ToolT, typename... Args>
	static void registerTool(Args&&... args) {
		const std::string& toolName = ToolT::getToolName();
		m_tools[toolName] = new ToolT(std::forward<Args>(args)...);
	}

	template<typename ToolT>
	static ToolT* getTool() {
		auto it = m_tools.find(ToolT::getToolName());
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
	static inline std::unordered_map<std::string, ITool*> m_tools;
};