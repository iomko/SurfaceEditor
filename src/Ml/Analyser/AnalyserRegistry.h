#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <filesystem>

#include "Analyser.h"
#include "../Editor/PredictorConfigDeserializer.h"

class AnalyserRegistry {
public:
    using Creator = std::function<std::unique_ptr<Analyser>(const std::string& instanceName)>;

    struct Entry {
        Creator creator;
        const char* name;
    };

    struct InstanceInfo {
        std::string instanceId;
        std::string typeId;
        std::string displayName;
    };

    static AnalyserRegistry& instance() {
        static AnalyserRegistry registry;
        return registry;
    }

    template<typename AnalyserT>
    void registerAnalyser(const std::string& typeId) {
        m_entries[typeId] = {
            [](const std::string& instanceName) -> std::unique_ptr<Analyser>
            {
                if constexpr (std::is_constructible_v<AnalyserT, const std::string&>) {
                    return std::make_unique<AnalyserT>(instanceName);
                } else {
                    return std::make_unique<AnalyserT>();
                }
            },
            AnalyserT::NAME
        };
    }

    Analyser* getAnalyser(const std::string& id) {
        ensureInitialized();

        if (auto* existing = getAnalyserInstance(id)) {
            return existing;
        }

        auto itEntry = m_entries.find(id);
        if (itEntry != m_entries.end()) {
            return createAnalyserInstance(id, id);
        }

        return nullptr;
    }

    Analyser* getAnalyserInstance(const std::string& instanceId) {
        ensureInitialized();

        auto it = m_instances.find(instanceId);
        if (it != m_instances.end()) {
            return it->second.instance.get();
        }

        return nullptr;
    }

    Analyser* createAnalyserInstance(const std::string& typeId, const std::string& instanceId)
    {
        ensureInitialized();

        if (instanceId.empty()) {
            return nullptr;
        }

        auto itExisting = m_instances.find(instanceId);
        if (itExisting != m_instances.end()) {
            return itExisting->second.instance.get();
        }

        auto itEntry = m_entries.find(typeId);
        if (itEntry == m_entries.end()) {
            return nullptr;
        }

        auto instance = itEntry->second.creator(instanceId);
        if (!instance) {
            return nullptr;
        }

        auto* rawPtr = instance.get();
        m_instances[instanceId] = InstanceRecord{
            typeId,
            std::move(instance)
        };

        return rawPtr;
    }

    std::vector<Predictor*> getPredictors() {
        ensureInitialized();

        std::vector<Predictor*> predictors;

        for (const auto& [id, instanceRecord] : m_instances) {
            auto* predictor = dynamic_cast<Predictor*>(instanceRecord.instance.get());
            if (predictor) {
                predictors.push_back(predictor);
            }
        }

        return predictors;
    }

    std::vector<InstanceInfo> getPredictorInstances() {
        ensureInitialized();

        std::vector<InstanceInfo> predictors;

        for (const auto& [instanceId, record] : m_instances)
        {
            auto* predictor = dynamic_cast<Predictor*>(record.instance.get());
            if (!predictor) {
                continue;
            }

            predictors.push_back({
                instanceId,
                record.typeId,
                predictor->getName()
            });
        }

        return predictors;
    }

    std::vector<InstanceInfo> getAnalyserInstances() {
        ensureInitialized();

        std::vector<InstanceInfo> analysers;

        for (const auto& [instanceId, record] : m_instances)
        {
            if (!record.instance) {
                continue;
            }

            analysers.push_back({
                instanceId,
                record.typeId,
                record.instance->getName()
            });
        }

        return analysers;
    }

    std::vector<std::pair<std::string, std::string>> getNamesWithId() {
        ensureInitialized();

        std::vector<std::pair<std::string, std::string>> names;

        for (const auto& [instanceId, record] : m_instances)
        {
            if (!record.instance) {
                continue;
            }

            names.emplace_back(record.instance->getName(), instanceId);
        }

        return names;
    }

    std::string getInstanceTypeId(const std::string& instanceId) const {
        auto it = m_instances.find(instanceId);
        if (it == m_instances.end()) {
            return {};
        }

        return it->second.typeId;
    }

private:
    struct InstanceRecord {
        std::string typeId;
        std::unique_ptr<Analyser> instance;
    };

    void ensureInitialized()
    {
        if (m_initialized) {
            return;
        }

        createDefaultInstances();
        loadInstancesFromConfigFiles();
        m_initialized = true;
    }

    void createDefaultInstances()
    {
        for (const auto& [typeId, entry] : m_entries)
        {
            if (m_instances.contains(typeId)) {
                continue;
            }

            auto instance = entry.creator(typeId);
            if (!instance) {
                continue;
            }

            m_instances[typeId] = InstanceRecord{
                typeId,
                std::move(instance)
            };
        }
    }

    void loadInstancesFromConfigFiles()
    {
        namespace fs = std::filesystem;

        const fs::path root = ML_DATA_DIR;

        if (!fs::exists(root) || !fs::is_directory(root)) {
            return;
        }

        for (const auto& entry : fs::directory_iterator(root))
        {
            if (!entry.is_regular_file()) {
                continue;
            }

            const fs::path path = entry.path();

            if (path.extension() != ".json") {
                continue;
            }

            const std::string filename = path.filename().string();
            const std::string suffix = ".config.json";

            if (filename.size() < suffix.size()) {
                continue;
            }

            if (filename.substr(filename.size() - suffix.size()) != suffix) {
                continue;
            }

            PredictorConfig config = PredictorConfigDeserializer::loadFromFile(path.string());

            if (config.instanceName.empty()) {
                config.instanceName = path.stem().stem().string();
            }

            if (config.typeId.empty()) {
                if (m_entries.contains(config.instanceName)) {
                    config.typeId = config.instanceName;
                } else {
                    continue;
                }
            }

            if (m_instances.contains(config.instanceName)) {
                continue;
            }

            auto itEntry = m_entries.find(config.typeId);
            if (itEntry == m_entries.end()) {
                continue;
            }

            auto instance = itEntry->second.creator(config.instanceName);
            if (!instance) {
                continue;
            }

            m_instances[config.instanceName] = InstanceRecord{
                config.typeId,
                std::move(instance)
            };
        }
    }

private:
    std::unordered_map<std::string, Entry> m_entries;
    std::unordered_map<std::string, InstanceRecord> m_instances;
    bool m_initialized = false;
};

template<typename AnalyserT>
struct AutoRegisterAnalyser {
    AutoRegisterAnalyser(const std::string& id) {
        AnalyserRegistry::instance().registerAnalyser<AnalyserT>(id);
    }
};