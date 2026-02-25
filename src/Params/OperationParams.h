#pragma once

struct OpParams {
	virtual ~OpParams() = default;
};

struct SelectionLayerParams;
struct ImportExportMeshesParams;
struct BoxSelectionParams;
struct GizmoLayerParams;
struct GizmoParams;
struct OpenTopoParams;
struct PlaneParams;
struct CubeParams;
struct SelectFaceParamsOut;
struct SelectFaceParamsIn;
struct OctreeNodeDataParams;
struct MoveMeshParams;
struct MeshParams;
struct FaceParams;
struct SingleFaceParams;
struct MoveFaceParams;
struct MoveSelectedMeshesParams;
struct MoveSelectedFacesParams;
struct EdgeParams;
struct VertexParams;
struct BrushToolParams;
struct EdgeConnectionsParams;
struct BrushInteractionParams;
struct PrintableMeshParams;
struct PrintMeshSettingsParams;
struct AddOutlinerNodeCallBackParams;

template<typename T>
struct AddNewOutlinerNodeCallBackParams;

template<typename T>
struct AddChildOutlinerNodeCallBackParams;