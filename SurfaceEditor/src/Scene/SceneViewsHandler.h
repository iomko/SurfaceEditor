#pragma once
#include <map>
#include <set>

import Scene;
import LayerSystem.Layer.ViewPortLayer;

class SceneViewsHandler
{
public:
	SceneViewsHandler(Scene& scene, ViewPortLayer& viewPortLayer) : m_scene(scene), m_viewPortLayer(viewPortLayer) {}
	Scene& getScene() { return m_scene; }
private:
	std::set<Mesh*> m_selectedMeshes;
	std::map<Mesh*, std::set<HalfEdgeDS::Face*>> m_selectedFaces;
	Scene& m_scene;
	ViewPortLayer& m_viewPortLayer;
};
