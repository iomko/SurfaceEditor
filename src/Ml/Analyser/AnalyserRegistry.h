#pragma once
#include <functional>
#include <memory>
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
    void registerAnalyser() {
        m_entries[AnalyserT::ID] = {
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

    Analyser* getAnalyser(int id) {
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

    std::vector<std::pair<std::string, int>> getNamesWithId() const {
        std::vector<std::pair<std::string, int>> names;
        for (const auto& [id, entry] : m_entries) {
            names.emplace_back(entry.name, id);
        }
        return names;
    }

private:
    std::unordered_map<int, Entry> m_entries;
    std::unordered_map<int, std::unique_ptr<Analyser>> m_instances;
};

template<typename AnalyserT>
struct AutoRegisterAnalyser {
    AutoRegisterAnalyser() {
        AnalyserRegistry::instance().registerAnalyser<AnalyserT>();
        std::cout << "REGISTERED ANALYSER" << AnalyserT::NAME << std::endl;
    }
};
