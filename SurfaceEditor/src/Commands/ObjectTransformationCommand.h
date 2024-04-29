#pragma once
#include <glm/mat4x4.hpp>

#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortHolder.h"

struct ObjectTransformationConfiguration
{
    glm::mat4 m_translation{};
    glm::mat4 m_rotation{};
    glm::mat4 m_scale{};
};

class ObjectTransformationCommand : public Observable, public Command
{
public:
    ObjectTransformationCommand(ViewPortHolder* viewPortHolder)
    {
        addObserver(viewPortHolder);
    }

    void execute() override
    {
        notifyObservers();
    }
    void undo() override
    {

    }

    static constexpr std::string_view getCommandName() noexcept { return "ObjectTransformationCommand"; }

public:
    ObjectTransformationConfiguration m_ObjectTransformationConfiguration;
    Mesh* m_transformedMesh;
};