#pragma once
#include "../Scene/Scene.h"

struct OpParams {
	virtual ~OpParams() = default;
};

struct ToolBarParams : public OpParams
{
public:
	enum SelectionMode {
		Face,
		Edge,
		Vertex,
		Object
	};

	enum Type {
		Selection,
		Deselection
	};

	SelectionMode m_selectionMode;
	Type m_type;
};

struct TransformMeshParams : public OpParams
{
	glm::mat4 m_modelMatrix{};
	Mesh* m_transformedMesh = nullptr;
};

struct ImportExportMeshesParams : public OpParams
{
	std::string m_filePathMeshes = "";
};

struct OpenTopoParams : public OpParams
{
	float m_lowerLeftLon = 0.0f;
	float m_lowerLeftLat = 0.0f;
	float m_upperRightLon = 0.0f;
	float m_upperRightLat = 0.0f;

	std::string m_apiKey = "";
};

struct PlaneParams : public OpParams
{
	float m_size;
	int m_subdivisionLevel;
};

struct SelectFaceParamsOut : public OpParams
{
	Mesh* mesh = nullptr;
	HalfEdgeDS::Face* face = nullptr;
	glm::vec3 hitPoint;
};

struct SelectFaceParamsIn : public OpParams
{
	Ray ray;
};

struct OctreeNodeDataParams : public OpParams
{
	SceneRes::MeshFacePair meshFacePair;
	glm::vec3 hitPoint;
};

struct MeshParams : public OpParams
{
	Mesh* m_mesh = nullptr;
};

struct BrushToolParams : public OpParams
{
	float radius;
	float brushStrength;
};