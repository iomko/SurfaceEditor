#pragma once

#include "AnalyserExecutionLayer.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class AnalyserExecutionLayerRegistry {
public:
    using Creator = std::function<std::unique_ptr<AnalyserExecutionLayer>()>;

    struct Entry {
        Creator creator;
    };

    static AnalyserExecutionLayerRegistry& instance() {
        static AnalyserExecutionLayerRegistry registry;
        return registry;
    }

    template<typename ExecutionLayerT>
    void registerExecutionLayer(const std::string& analyserTypeId) {
        if (m_entries.contains(analyserTypeId)) {
            return;
        }

        m_entries[analyserTypeId] = {
            []() {
                return std::make_unique<ExecutionLayerT>();
            }
        };
    }

    AnalyserExecutionLayer* getExecutionLayer(const std::string& analyserTypeId) {
        auto itInstance = m_instances.find(analyserTypeId);
        if (itInstance != m_instances.end()) {
            return itInstance->second.get();
        }

        auto itEntry = m_entries.find(analyserTypeId);
        if (itEntry == m_entries.end()) {
            return nullptr;
        }

        auto instance = itEntry->second.creator();
        auto* rawPtr = instance.get();

        m_instances[analyserTypeId] = std::move(instance);

        return rawPtr;
    }

private:
    std::unordered_map<std::string, Entry> m_entries;
    std::unordered_map<std::string, std::unique_ptr<AnalyserExecutionLayer>> m_instances;
};

template<typename ExecutionLayerT>
struct AutoRegisterAnalyserExecutionLayer {
    AutoRegisterAnalyserExecutionLayer(const std::string& analyserTypeId) {
        AnalyserExecutionLayerRegistry::instance()
            .registerExecutionLayer<ExecutionLayerT>(analyserTypeId);
    }
};