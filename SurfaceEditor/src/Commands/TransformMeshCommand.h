#pragma once
#include <string_view>

#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortHolder.h"

class TransformMeshCommand : public Observable, public Command
{
public:
    TransformMeshCommand(ViewPortHolder* viewPortHolder)
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

    static constexpr std::string_view getCommandName() noexcept { return "TransformMeshCommand"; }

public:
    glm::mat4 m_modelMatrix;
    Mesh* m_transformedMesh;
};