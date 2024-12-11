#pragma once
#include <vector>
#include "Patterns/Observer.h"
#include "Patterns/Command.h"
#include <glm/mat4x4.hpp>
#include "../Renderer/VertexDataStructs.h"
import LayerSystem.Layer.ViewPortLayer;
#include "Core/Layer.h"


class ViewPortHolder : public Layer, public Observer
{
public:

	//renderFlags budu nahradene nejakym RenderSettings pre kazdy ViewPortLayer zvlast
	struct MeshRenderingFlags
	{
		bool RENDER_NORMALS = true;
		bool RENDER_POINTS = true;
		bool RENDER_EDGES = true;
		bool RENDER_FACES = true;
	};

	
	struct MeshRenderingShaderData
	{
		MeshRenderingShaderData(Shader point_shader, Shader edge_shader, Shader normal_shader,
			const Shader face_shader)
			: PointShader(point_shader),
			EdgeShader(edge_shader),
			NormalShader(normal_shader),
			FaceShader(face_shader)
		{
		}

		Shader PointShader;
		Shader EdgeShader;
		Shader NormalShader;
		Shader FaceShader;
	};

	struct MeshRenderingVAOData
	{
	public:
		std::vector<LineVertex> m_edges;
		std::vector<MeshPoint> m_points;
	};

	//toto pojde prec
	std::map<Mesh*, std::tuple<MeshRenderingFlags, MeshRenderingShaderData, MeshRenderingVAOData>> m_meshesShaderData;
	
public:

	ViewPortHolder(ViewPortLayer* viewPortLayer): Layer("ViewPortHolder")
	{
		m_viewPortLayer = viewPortLayer;
	}

	virtual void onEvent(Event& event) override
	{
		//tuto budu prichadzat eventy a ja ich potom nasledne budem moct posielat do viewPortlayeru podla toho co som stlacil,
		//cize budem 
	}

	void addCommandToQueue(Command* command)
	{
		m_commandsQueue.push_back(command);
	}

public:
	//selectedMeshes
	std::set<Mesh*> m_selectedMeshes;
	//selectedFaces
	//nepouzivane pri renderovani
	std::map<Mesh*, std::set<HalfEdgeDS::Face*>> m_selectedFaces;
	std::map<Mesh*, std::set<HalfEdgeDS::Vertex*>> m_selectedVertices;

	std::vector<Command*> m_commandsQueue;
	ViewPortLayer* m_viewPortLayer;

	//temporary
	size_t m_currentMeshId = 0;
};