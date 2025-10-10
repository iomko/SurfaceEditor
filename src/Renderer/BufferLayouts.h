#pragma once
#include <map>
#include <optional>
#include <functional>
#include "BufferStorages.h"
#include "../Renderer/Material.h"

class MeshBufferLayout {
public:
    using TriangleMap = std::map<Material*, TriangleBufferStorage>;
    using TriangleIterator = TriangleMap::iterator;

    using LineMap = std::map<Material*, LineBufferStorage>;
    using LineIterator = LineMap::iterator;
    
    LineBufferStorage& registerLineBufferStorage(Material* material) {
        return m_matLineBuff[material];
    }

    std::optional<std::reference_wrapper<LineBufferStorage>> getLineBufferStorage(Material* material) {
        auto it = m_matLineBuff.find(material);

        if(it == m_matLineBuff.end()) {
            return std::nullopt;
        }
        return std::ref(it->second);
    }

    TriangleBufferStorage& registerTriangleBufferStorage(Material* material) {
        return m_matTriangleBuff[material];
    }

    std::optional<std::reference_wrapper<TriangleBufferStorage>> getTriangleBufferStorage(Material* material) {
        auto it = m_matTriangleBuff.find(material);

        if(it == m_matTriangleBuff.end()) {
            return std::nullopt;
        }
        return std::ref(it->second);
    };

    TriangleIterator triangleBuffersBegin() { return m_matTriangleBuff.begin(); }
    TriangleIterator triangleBuffersEnd()   { return m_matTriangleBuff.end(); }

    LineIterator lineBuffersBegin() { return m_matLineBuff.begin(); }
    LineIterator lineBuffersEnd() { return m_matLineBuff.end(); }

private:
    std::map<Material*, TriangleBufferStorage> m_matTriangleBuff;
    std::map<Material*, LineBufferStorage> m_matLineBuff;
};


class PrintableMeshBufferLayout {
public:
    using TriangleMap = std::map<Material*, TriangleBufferStorage>;
    using TriangleIterator = TriangleMap::iterator;

    using LineMap = std::map<Material*, LineBufferStorage>;
    using LineIterator = LineMap::iterator;
    
    LineBufferStorage& registerLineBufferStorage(Material* material) {
        return m_matLineBuff[material];
    }

    std::optional<std::reference_wrapper<LineBufferStorage>> getLineBufferStorage(Material* material) {
        auto it = m_matLineBuff.find(material);

        if(it == m_matLineBuff.end()) {
            return std::nullopt;
        }
        return std::ref(it->second);
    }

    LineIterator lineBuffersBegin() { return m_matLineBuff.begin(); }
    LineIterator lineBuffersEnd() { return m_matLineBuff.end(); }

private:
    std::map<Material*, LineBufferStorage> m_matLineBuff;
};

