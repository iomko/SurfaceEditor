#pragma once
#include "../Patterns/Command.h"
#include "CommandIDs.h"

class BasicSculptToolCommand : public Command<BASIC_SCULPT_TOOL_COMMAND>
{
public:
    virtual void execute() override;

    void undo();

private:

    glm::vec3 computeAverageNormal(const std::vector<glm::vec3>& normals);

    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius);

    float logarithmicInterpolation(float edge0, float edge1, float x);

    float smoothstep(float edge0, float edge1, float x);

    float calculateBrushScalingFactor(float distance, float radius, float brushStrength);
    
    std::pair<std::vector<ExtendedVertex*>, std::vector<ExtendedFace*>> accessNeighbouringVertsAndFacesOfVertex(ExtendedVertex* vertex);
};
