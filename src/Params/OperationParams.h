#pragma once
#include "../Scene/Scene.h"
#include "Structures/PrintableMesh.h"

struct OpParams {
	virtual ~OpParams() = default;
};

struct SelectionLayerParams : public OpParams
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

struct CubeParams : public OpParams
{
	float m_size;
	float m_subdivisionLevel;

	glm::vec3 m_position;
};

struct SelectFaceParamsOut : public OpParams
{
	Mesh* mesh = nullptr;
	ExtendedFace* face = nullptr;
	glm::vec3 hitPoint;
};

struct SelectFaceParamsIn : public OpParams
{
	Ray ray;
};

struct OctreeNodeDataParams : public OpParams
{
	SceneResources::MeshFacePair meshFacePair;
	glm::vec3 hitPoint;
};

struct MeshParams : public OpParams
{
	Mesh* m_mesh = nullptr;
};

struct FaceParams : public OpParams
{   
    Mesh* mesh;
    std::vector<ExtendedFace*>* faces;
};

struct NewFaceParams : public OpParams {
    Mesh* mesh;
    ExtendedFace* face;
};

struct EdgeParams : public OpParams
{
    Mesh* mesh;
    std::vector<ExtendedEdge*>* edges;
};

struct VertexParams : public OpParams
{
    Mesh* mesh = nullptr;
    ExtendedVertex* vertex = nullptr;
    glm::vec3 moveByVector; //newPosition;
};

struct BrushToolParams : public OpParams
{
	float radius;
	float brushStrength;
};

struct EdgeConnectionsParams : public OpParams 
{
    Mesh* mesh;
    ExtendedEdge* firstEdge;
    ExtendedEdge* secondEdge;
};

struct BrushInteractionParams : public OpParams
{
	BrushToolParams brushToolParams;
	Camera* camera = nullptr;
	Window* window = nullptr;
};

struct PrintableMeshParams : public OpParams {
    PrintableMesh printableMesh; 
};

struct PrintMeshSettingsParams : public OpParams {
    Mesh* mesh = nullptr;
    float height = 1.0f;
};


