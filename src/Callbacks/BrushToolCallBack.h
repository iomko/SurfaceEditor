#pragma once
#include "../Params/OperationParams.h"
#include "Callback.h"
#include "../Patterns/Observer.h"

#include "CallbackIDs.h"

class BrushToolCallBack : public Callback<BRUSH_TOOL_CALLBACK, BrushToolParams, OctreeNodeDataParams>, public Observer
{
public:
    BrushToolCallBack();

	void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override;

private:

	float calculateNormalScaleFactor(float distance, float radius, float brushStrength);

	glm::vec3 computeAvgNormal(ExtendedVertex* vertex);

	std::pair<std::unordered_set<ExtendedVertex*>, std::unordered_set<ExtendedFace*>>
	collectIntersectingElements(const Sphere& sphere, Mesh* mesh);

	ExtendedVertex* findClosestVertexOnFace(ExtendedFace* face, glm::vec3 hitPoint);

};
