#pragma once
#include "../Scene/Scene.h"
#include "../UI/SelectionRectangle.h"
#include "Structures/PrintableMesh.h"
#include "ImGuizmo.h"

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

	SelectionMode m_selectionMode;
	RectanglePos  m_rectanglePos;
};

struct ImportExportMeshesParams : public OpParams
{
	std::string m_filePathMeshes = "";
};
struct BoxSelectionParams : public OpParams
{
	glm::vec2 start_mouse_pos;
};
struct GizmoLayerParams : public OpParams
{
    enum Type
    {
        Move,
        Rotate,
		Scale,
        Disable
    };

    Type m_type;
};

struct GizmoParams : public OpParams
{
	enum SelectionMode
	{
		Mesh,
		Face
	};

	SelectionMode m_selectionMode;
	ImGuizmo::OPERATION m_type;
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
	glm::vec3 m_position;
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

struct MoveMeshParams : public OpParams
{
    Mesh* mesh = nullptr;
	glm::mat4 transformMatrix;
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

struct SingleFaceParams : public OpParams {
    Mesh* mesh;
    ExtendedFace* face;
};

struct MoveFaceParams : public OpParams {
    Mesh* mesh;
    ExtendedFace* face;
    glm::vec3 moveByVector;
};

struct MoveSelectedMeshesParams : public OpParams {
	glm::mat4 transformMatrix;
};

struct MoveSelectedFacesParams : public OpParams {
    glm::vec3 moveByVector;
};

struct EdgeParams : public OpParams
{
    Mesh* mesh;
    std::vector<ExtendedEdge*>* edges;
};

struct VertexParams : public OpParams
{
	enum MoveBy
	{
		VECTOR,
		TRANSFORM_MATRIX
	};

    Mesh* mesh = nullptr;
    ExtendedVertex* vertex = nullptr;
	glm::vec3 newPosition;
	glm::mat4 transformMatrix;
	MoveBy moveBy;
};

struct BrushToolParams : public OpParams
{
	float radius;
	float brushStrength;
};

struct SelectionToolParams : public OpParams
{
	std::vector<Mesh*> m_selectedMeshes;
	std::vector<ExtendedFace*> m_selectedFaces;
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

class LayerState;
class OutlinerNodeConcept;

struct AddOutlinerNodeCallBackParams : public OpParams 
{
	LayerState* state = nullptr;
	int id = 0;
	std::string name = "";
};

template<typename T>
struct AddNewOutlinerNodeCallBackParams : public AddOutlinerNodeCallBackParams 
{	
	T* data = nullptr;
};

template<typename T>
struct AddChildOutlinerNodeCallBackParams : public AddNewOutlinerNodeCallBackParams<T>
{
	OutlinerNodeConcept* parent = nullptr;
};

