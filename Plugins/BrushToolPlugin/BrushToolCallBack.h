#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"


class BrushToolCallBack : public Callback<BrushToolParams, OctreeNodeDataParams>, public Observer
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
