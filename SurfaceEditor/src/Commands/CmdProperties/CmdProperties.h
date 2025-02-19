#pragma once

struct ToolBarParams : public Params
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

struct TransformMeshParams : public Params
{
	glm::mat4 m_modelMatrix{};
	Mesh* m_transformedMesh = nullptr;
};

struct ImportExportMeshesParams : public Params
{
	std::string m_filePathMeshes = "";
};

struct AddPlaneParams : public Params
{
	float m_size;
	int m_subdivisionLevel;
};