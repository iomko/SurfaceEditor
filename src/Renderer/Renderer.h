#pragma once

#include <optional>

#include "../Primitives/AABB.h"
#include <vector>

#include "Buffers.h"
#include "../Scene/Mesh.h"
#include <string>
#include "DataStructures/PrintableMesh.h"


struct RendererConfig
{
	static constexpr unsigned int maxBoxCount = 2000000;
	static constexpr unsigned int maxVertexCount = 36 * maxBoxCount;
	static constexpr unsigned int maxIndexCount = 24 * maxBoxCount;
};

struct RendererBuffersData 
{
	struct AABBVertex {
		glm::vec3 position;
		glm::vec3 color;
	};

	struct LineVertex {
		glm::vec3 position;
		float isHighlighted;
	};

	struct PointVertex
	{
		glm::vec3 position;
		float isHighlighted = 0.0f;
	};

	struct MeshVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		float isHighlited = 0.0f;
	};

	friend class Renderer;
};

template<typename T, typename... Args>
concept HasGetBufferStorage = requires(T t, Args&&... args) {
    { t.getBufferStorage(std::forward<Args>(args)...) } -> std::same_as<bool>;
};

template<typename T, typename... Args>
concept HasGetRawDataStorage = requires(T t, Args&&... args) {
    { t.getRawDataStorage(std::forward<Args>(args)...) } -> std::same_as<bool>;
};

template<typename T, typename... Args>
concept HasUpdateBufferStorage = requires(T t, Args&&... args) {
    { t.updateBufferStorage(std::forward<Args>(args)...) } -> std::same_as<void>;
};

template<typename T, typename... Args>
concept HasRegisterBufferStorage = requires(T t, Args&&... args) {
    { t.registerBufferStorage(std::forward<Args>(args)...) } -> std::same_as<void>;
};


class BufferStorageConcept {
public:
    BufferStorageConcept() = default;
};

template<typename Derived>
class BufferStorage : public BufferStorageConcept {
public:
    BufferStorage() = default;

public:

    template<typename... Args>
    void getBufferStorage(Args&&... args){
        static_assert(HasGetRawDataStorage<Derived, Args...>, "Derived must implement getBufferDataStorage");
        static_cast<Derived*>(this)->getBufferDataStorage(std::forward<Args>(args)...);
    }

    template<typename... Args>
    void getRawDataStorage(Args&&... args){
        static_assert(HasGetRawDataStorage<Derived, Args...>, "Derived must implement getBufferDataStorage");
        static_cast<Derived*>(this)->getBufferDataStorage(std::forward<Args>(args)...);
    }

    template<typename... Args>
    void updateBufferStorage(Args&&... args) {
        static_assert(HasUpdateBufferStorage<Derived, Args...>, "Derived must implement updateBufferStorage");
        static_cast<Derived*>(this)->updateBufferStorage(std::forward<Args>(args)...);
    }

    template<typename... Args>
    void registerBufferStorage(Args&&... args) {
        static_assert(HasRegisterBufferStorage<Derived, Args...>, "Derived must implement registerBufferStorage");
        static_cast<Derived*>(this)->registerBufferStorage(std::forward<Args>(args)...);
    };

    static std::string getBufferStorageName() {
        return "BufferStorage";
    }
};

template<typename VertexType>
struct BufferData{
    VertexArrayObject vao;
    VertexBufferObject vbo;
    std::vector<VertexType> vertices;
};


class MeshBufferStorage : public BufferStorage<MeshBufferStorage> {
public:
    MeshBufferStorage() {}

    bool getBufferData(Mesh* mesh, Material* material, BufferData<RendererBuffersData::MeshVertex>*& retBufferData){
        auto meshMatsMapIt = meshMatsMap.find(mesh);

        if (meshMatsMapIt == meshMatsMap.end()) return false;

        auto& matBuffMap = meshMatsMapIt->second;
        
        auto matBuffMapIt = matBuffMap.find(material);

        if (matBuffMapIt == matBuffMap.end()) return false;

        auto& meshBufferData = matBuffMapIt->second;
        retBufferData = &meshBufferData;
        return true;
    }

    void updateBufferStorage(Mesh* mesh, Material* material){
        auto meshMatsMapIt = meshMatsMap.find(mesh);

        if (meshMatsMapIt == meshMatsMap.end()) return;

        auto& matBuffMap = meshMatsMapIt->second;
        auto matBuffMapIt = matBuffMap.find(material);

        if (matBuffMapIt == matBuffMap.end()) return;

        auto& meshBufferData = matBuffMapIt->second;
        
        meshBufferData.vbo.bind();
        meshBufferData.vbo.createData(meshBufferData.vertices.data(), meshBufferData.vertices.size() * sizeof(RendererBuffersData::MeshVertex), GL_DYNAMIC_DRAW);
        meshBufferData.vbo.unbind();
    }

    void registerBufferStorage(Mesh* mesh, Material* material){
        auto& matBuffMap = meshMatsMap[mesh];

        auto [it, inserted] = matBuffMap.try_emplace(material);

        if(inserted){
            auto& meshBufferData = it->second;
            
            meshBufferData.vao.create();
            meshBufferData.vbo.create();
            
            meshBufferData.vao.bind();
            meshBufferData.vbo.bind();
            meshBufferData.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::MeshVertex), (void*)offsetof(RendererBuffersData::MeshVertex, position));
            meshBufferData.vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::MeshVertex), (void*)offsetof(RendererBuffersData::MeshVertex, normal));
            meshBufferData.vao.addVertexBufferLayout(2, 1, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::MeshVertex), (void*)offsetof(RendererBuffersData::MeshVertex, isHighlited));
            meshBufferData.vao.unbind();
            meshBufferData.vbo.unbind();
        }
    }

    static std::string getBufferStorageName() {
        return "MeshBufferStorage";
    }

	using MatBuffMap = std::map<Material*, BufferData<RendererBuffersData::MeshVertex>>;
	using MeshMatsMap = std::map<Mesh*, MatBuffMap>;

	MeshMatsMap meshMatsMap;
};

class PrintableMeshBufferStorage : public BufferStorage<PrintableMeshBufferStorage> {
public:
    PrintableMeshBufferStorage() {}

    bool getBufferData(PrintableMesh* mesh, BufferData<RendererBuffersData::LineVertex>*& retBufferData) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);
        
        if (meshBuffMapIt == meshBuffMap.end()) return false;

        auto& lineBufferData = meshBuffMapIt->second;
        retBufferData = &lineBufferData;
        return true;
    }

    void updateBufferStorage(PrintableMesh* mesh) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);

        if (meshBuffMapIt == meshBuffMap.end()) return;

        auto& lineBufferData = meshBuffMapIt->second;

        lineBufferData.vbo.bind();
        lineBufferData.vbo.createData(lineBufferData.vertices.data(), lineBufferData.vertices.size() * sizeof(RendererBuffersData::LineVertex), GL_DYNAMIC_DRAW);
        lineBufferData.vbo.unbind();
    }

    void registerBufferStorage(PrintableMesh* mesh) {

        auto [it, inserted] = meshBuffMap.try_emplace(mesh);

        if(inserted) {
            auto& lineBufferData =it->second;
            
            lineBufferData.vao.create();
            lineBufferData.vbo.create();

            lineBufferData.vao.bind();
            lineBufferData.vbo.bind();
            lineBufferData.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::LineVertex), (void*)offsetof(RendererBuffersData::LineVertex, position));
            lineBufferData.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::LineVertex), (void*)offsetof(RendererBuffersData::LineVertex, isHighlighted));
            lineBufferData.vao.unbind();
            lineBufferData.vbo.unbind();
        }
    }

    static std::string getBufferStorageName() {
        return "LineBufferStorage";
    }

	using MeshBuffMap = std::map<PrintableMesh*, BufferData<RendererBuffersData::LineVertex>>;

	MeshBuffMap meshBuffMap;
};

class LineBufferStorage : public BufferStorage<LineBufferStorage> {
public:
    LineBufferStorage() {}

    bool getBufferData(Mesh* mesh, BufferData<RendererBuffersData::LineVertex>*& retBufferData) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);
        
        if (meshBuffMapIt == meshBuffMap.end()) return false;

        auto& lineBufferData = meshBuffMapIt->second;
        retBufferData = &lineBufferData;
        return true;
    }

    void updateBufferStorage(Mesh* mesh) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);

        if (meshBuffMapIt == meshBuffMap.end()) return;

        auto& lineBufferData = meshBuffMapIt->second;

        lineBufferData.vbo.bind();
        lineBufferData.vbo.createData(lineBufferData.vertices.data(), lineBufferData.vertices.size() * sizeof(RendererBuffersData::LineVertex), GL_DYNAMIC_DRAW);
        lineBufferData.vbo.unbind();
    }

    void registerBufferStorage(Mesh* mesh) {

        auto [it, inserted] = meshBuffMap.try_emplace(mesh);

        if(inserted) {
            auto& lineBufferData =it->second;
            
            lineBufferData.vao.create();
            lineBufferData.vbo.create();

            lineBufferData.vao.bind();
            lineBufferData.vbo.bind();
            lineBufferData.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::LineVertex), (void*)offsetof(RendererBuffersData::LineVertex, position));
            lineBufferData.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::LineVertex), (void*)offsetof(RendererBuffersData::LineVertex, isHighlighted));
            lineBufferData.vao.unbind();
            lineBufferData.vbo.unbind();
        }
    }

    static std::string getBufferStorageName() {
        return "LineBufferStorage";
    }

	using MeshBuffMap = std::map<Mesh*, BufferData<RendererBuffersData::LineVertex>>;

	MeshBuffMap meshBuffMap;
};

class PointBufferStorage : public BufferStorage<PointBufferStorage> {
public:
    PointBufferStorage() {}

    bool getBufferData(Mesh* mesh, BufferData<RendererBuffersData::PointVertex>*& retBufferData) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);

        if(meshBuffMapIt == meshBuffMap.end()) return false;

        auto& pointBufferData = meshBuffMapIt->second;
        retBufferData = &pointBufferData;
        return true;
    }

    void updateBufferStorage(Mesh* mesh) {
        auto meshBuffMapIt = meshBuffMap.find(mesh);

        if(meshBuffMapIt == meshBuffMap.end()) return;

        auto& pointBufferData = meshBuffMapIt->second;

        pointBufferData.vbo.bind();
        pointBufferData.vbo.createData(pointBufferData.vertices.data(), pointBufferData.vertices.size() * sizeof(RendererBuffersData::PointVertex), GL_DYNAMIC_DRAW);
        pointBufferData.vbo.unbind();
    }

    void registerBufferStorage(Mesh* mesh) {
        auto [it, inserted] = meshBuffMap.try_emplace(mesh);

        if(inserted) {
            auto& pointBufferData = it->second;

            pointBufferData.vao.create();
            pointBufferData.vbo.create();

            pointBufferData.vao.bind();
            pointBufferData.vbo.bind();
            pointBufferData.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::PointVertex), (void*)0);
            pointBufferData.vao.addVertexBufferLayout(1, 1, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::PointVertex), (void*)(offsetof(RendererBuffersData::PointVertex, isHighlighted)));
            pointBufferData.vao.unbind();
            pointBufferData.vbo.unbind();
        }

    }

    static std::string getBufferStorageName() {
        return "PointBufferStorage";
    }

	using MeshBuffMap = std::map<Mesh*, BufferData<RendererBuffersData::PointVertex>>;

	MeshBuffMap meshBuffMap;
};

class AABBBufferStorage : public BufferStorage<AABBBufferStorage> {
public:
    AABBBufferStorage() {}

    bool getBufferData(const AABBBoundingRegion& aabb, BufferData<RendererBuffersData::AABBVertex>*& retBufferData) {
        auto aabbBuffMapIt = aabbBuffMap.find(aabb);

        if(aabbBuffMapIt == aabbBuffMap.end()) return false;

        auto& aabbBufferData = aabbBuffMapIt->second;
        
        retBufferData = &aabbBufferData;
        return true;
    }

    void updateBufferStorage(const AABBBoundingRegion& aabb) {
        auto aabbBuffMapIt = aabbBuffMap.find(aabb);

        if(aabbBuffMapIt == aabbBuffMap.end()) return;

        auto& aabbBufferData = aabbBuffMapIt->second;

        aabbBufferData.vbo.bind();
        aabbBufferData.vbo.createData(aabbBufferData.vertices.data(), aabbBufferData.vertices.size() * sizeof(RendererBuffersData::AABBVertex), GL_DYNAMIC_DRAW);
        aabbBufferData.vbo.unbind();
    }

    void registerBufferStorage(const AABBBoundingRegion& aabb) {
        auto [it, inserted] = aabbBuffMap.try_emplace(aabb); 
        
        if(inserted) {
            auto& aabbBufferData = it->second;

            aabbBufferData.vao.create();
            aabbBufferData.vbo.create();

            aabbBufferData.vao.bind();
            aabbBufferData.vbo.bind();
            aabbBufferData.vao.addVertexBufferLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::AABBVertex), (void*)0);
            aabbBufferData.vao.addVertexBufferLayout(1, 3, GL_FLOAT, GL_FALSE, sizeof(RendererBuffersData::AABBVertex), (void*)offsetof(RendererBuffersData::AABBVertex, color));
            aabbBufferData.vao.unbind();
            aabbBufferData.vbo.unbind();
        }
    }

    static std::string getBufferStorageName() {
        return "AABBBufferStorage";
    }

	using AABBBuffMap = std::map<AABBBoundingRegion, BufferData<RendererBuffersData::AABBVertex>>;

	AABBBuffMap aabbBuffMap;
};


class Renderer {
public:
	static void init()
	{
		using AABBVertex = RendererBuffersData::AABBVertex;
		using LineVertex = RendererBuffersData::LineVertex;
		using PointVertex = RendererBuffersData::PointVertex;
		using MeshVertex = RendererBuffersData::MeshVertex;

        s_bufferRegistry.registerBuffer<MeshBufferStorage>();
        s_bufferRegistry.registerBuffer<LineBufferStorage>();
        s_bufferRegistry.registerBuffer<AABBBufferStorage>();
        s_bufferRegistry.registerBuffer<PointBufferStorage>();
        s_bufferRegistry.registerBuffer<PrintableMeshBufferStorage>();
	}

    static void drawMeshPoints(Mesh* mesh, Shader* shader){
        PointBufferStorage* pointBufferStorage = s_bufferRegistry.queryBuffer<PointBufferStorage>();
        
        BufferData<RendererBuffersData::PointVertex>* pointBufferData;
        if(pointBufferStorage->getBufferData(mesh, pointBufferData)) {
           
            shader->bind();
            glDisable(GL_PROGRAM_POINT_SIZE);
            pointBufferData->vao.bind();

            glPointSize(10.0f);

            glDrawArrays(GL_POINTS, 0, pointBufferData->vertices.size());

            pointBufferData->vao.unbind();
            shader->unbind();
        }
    }

    static void drawMeshLines(Mesh* mesh, Shader* shader){
		LineBufferStorage* lineBufferStorage = s_bufferRegistry.queryBuffer<LineBufferStorage>();

        BufferData<RendererBuffersData::LineVertex>* lineBufferData;

        if(lineBufferStorage->getBufferData(mesh, lineBufferData)) {
            shader->bind();
            lineBufferData->vao.bind();
            glLineWidth(1.0f);
            glDrawArrays(GL_LINES, 0, lineBufferData->vertices.size());
            glLineWidth(0.5f);
            lineBufferData->vao.unbind();
            shader->unbind();
        }
    }

    static void drawPrintableMesh(PrintableMesh* mesh, Shader* shader) {
		PrintableMeshBufferStorage* printableMeshBufferStorage = s_bufferRegistry.queryBuffer<PrintableMeshBufferStorage>();

        BufferData<RendererBuffersData::LineVertex>* lineBufferData;

        if(printableMeshBufferStorage->getBufferData(mesh, lineBufferData)) {
            shader->bind();
            lineBufferData->vao.bind();
            glLineWidth(5.0f);
            glDrawArrays(GL_LINES, 0, lineBufferData->vertices.size());
            glLineWidth(0.5f);
            lineBufferData->vao.unbind();
            shader->unbind();
        }
    }

    static void drawMesh(Mesh* mesh){
        MeshBufferStorage* meshBufferStorage = s_bufferRegistry.queryBuffer<MeshBufferStorage>();

        BufferData<RendererBuffersData::MeshVertex>* meshBufferData;

        if(meshBufferStorage->getBufferData(mesh, mesh->m_defaultMaterial, meshBufferData)) {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f);
            
            mesh->m_defaultMaterial->getShader()->bind();
            meshBufferData->vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, meshBufferData->vertices.size()); 
            meshBufferData->vao.unbind();
            mesh->m_defaultMaterial->getShader()->bind();

            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }

    static void drawBox(const AABBBoundingRegion& aabb){
        AABBBufferStorage* aabbBufferStorage = s_bufferRegistry.queryBuffer<AABBBufferStorage>();

        BufferData<RendererBuffersData::AABBVertex>* aabbBufferData;
        
        if(aabbBufferStorage->getBufferData(aabb, aabbBufferData)) {
            aabbBufferData->vao.bind();

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, aabbBufferData->vertices.size());
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            aabbBufferData->vao.unbind();
        }
    }

private:
	class BufferRegistry
	{
	public:
		template<typename BufferType>
		void registerBuffer() {
            m_buffers.try_emplace(BufferType::getBufferStorageName(), new BufferType());
		}

        template<typename BufferType>
		BufferType* queryBuffer()
		{
            auto it = m_buffers.find(BufferType::getBufferStorageName());
            if (it != m_buffers.end()) {
                return static_cast<BufferType*>(it->second);
            }
            return nullptr;
		}

	private:
		std::map<std::string, BufferStorageConcept*> m_buffers;
	};
public:
	inline static BufferRegistry s_bufferRegistry;
};
