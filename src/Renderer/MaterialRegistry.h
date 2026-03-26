#pragma once
#include <string_view>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include "Material.h"

class MaterialRegistry {
public:
    static void registerMaterial(const std::string& name, Shader* shader) {
        m_materials[name] = new Material(name, shader);
    }

    static Material* getMaterial(const std::string& name) {
        auto it = m_materials.find(name); 

        if (it != m_materials.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    static void deleteRegistry() {
        for (auto& pair : m_materials) {
            delete pair.second;
        }
    }

private:
    inline static std::unordered_map<std::string, Material*> m_materials;
};
