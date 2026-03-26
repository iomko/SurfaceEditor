#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "FeatureStrategy.h"

class FeatureStrategyRegistry {
public:
    using Creator = std::function<std::unique_ptr<FeatureStrategyConcept>()>;

    struct Entry {
        Creator creator;
        const char* name;
    };

    static FeatureStrategyRegistry& instance() {
        static FeatureStrategyRegistry registry;
        return registry;
    }

    template<typename FeatureT>
    void registerFeature() {
        m_entries[FeatureT::ID] = {
            []() { return std::make_unique<FeatureT>(); },
            FeatureT::NAME
        };
    }

    FeatureStrategyConcept* getFeature(int id) {
        // already created
        auto itInstance = m_instances.find(id);
        if (itInstance != m_instances.end()) {
            return itInstance->second.get();
        }

        // create from entry
        auto itEntry = m_entries.find(id);
        if (itEntry != m_entries.end()) {
            auto instance = itEntry->second.creator();
            auto* rawPtr = instance.get();
            m_instances[id] = std::move(instance);
            return rawPtr;
        }

        return nullptr;
    }

    std::vector<FeatureStrategyConcept*> getFeaturesByType(const FeatureObjectType& type) {
        std::vector<FeatureStrategyConcept*> features;

        for (const auto& [id, entry] : m_entries) {
            FeatureStrategyConcept* feature = getFeature(id);
            if (feature && feature->getObjectType() == type) {
                features.push_back(feature);
            }
        }

        return features;
    }

    std::vector<std::pair<std::string, int>> getNamesWithIdByType(const FeatureObjectType& type) {
        std::vector<std::pair<std::string, int>> result;

        for (const auto& [id, entry] : m_entries) {
            FeatureStrategyConcept* feature = getFeature(id);
            if (feature && feature->getObjectType() == type) {
                result.emplace_back(entry.name, id);
            }
        }

        return result;
    }

    std::vector<int> getRegisteredIds() const {
        std::vector<int> ids;
        ids.reserve(m_entries.size());

        for (const auto& [id, _] : m_entries) {
            ids.push_back(id);
        }

        return ids;
    }

private:
    std::unordered_map<int, Entry> m_entries;
    std::unordered_map<int, std::unique_ptr<FeatureStrategyConcept>> m_instances;
};

template<typename FeatureT>
struct AutoRegisterFeatureStrategy {
    AutoRegisterFeatureStrategy() {
        FeatureStrategyRegistry::instance().registerFeature<FeatureT>();
        std::cout << "REGISTERED FEATURE STRATEGY: " << FeatureT::NAME << std::endl;
    }
};