#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Analyser.h"

class AnalyserRegistry {
public:
    using Creator = std::function<std::unique_ptr<Analyser>()>;

    struct Entry {
        Creator creator;
        const char* name;
    };

    static AnalyserRegistry& instance() {
        static AnalyserRegistry registry;
        return registry;
    }

    template<typename AnalyserT>
    void registerAnalyser(const std::string& id) {
        m_entries[id] = {
            []() { return std::make_unique<AnalyserT>(); },
            AnalyserT::NAME
        };
    }

    std::vector<Predictor*> getPredictors() {
        std::vector<Predictor*> predictors;

        for (const auto& [id, instance] : m_instances) {
            auto* predictor = dynamic_cast<Predictor*>(instance.get());
            if (predictor) {
                predictors.push_back(predictor);
            }
        }

        return predictors;
    }

    Analyser* getAnalyser(const std::string& id) {
        auto itInstance = m_instances.find(id);
        if (itInstance != m_instances.end()) {
            return itInstance->second.get();
        }

        auto itEntry = m_entries.find(id);
        if (itEntry != m_entries.end()) {
            auto instance = itEntry->second.creator();
            auto* rawPtr = instance.get();
            m_instances[id] = std::move(instance);
            return rawPtr;
        }

        return nullptr;
    }

    std::vector<std::pair<std::string, std::string>> getNamesWithId() const {
        std::vector<std::pair<std::string, std::string>> names;

        for (const auto& [id, entry] : m_entries) {
            names.emplace_back(entry.name, id);
        }

        return names;
    }

private:
    std::unordered_map<std::string, Entry> m_entries;
    std::unordered_map<std::string, std::unique_ptr<Analyser>> m_instances;
};

template<typename AnalyserT>
struct AutoRegisterAnalyser {
    AutoRegisterAnalyser(const std::string& id) {
        AnalyserRegistry::instance().registerAnalyser<AnalyserT>(id);
    }
};