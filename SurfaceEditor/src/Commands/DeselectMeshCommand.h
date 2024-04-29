#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortHolder.h"

class DeselectMeshCommand : public Observable, public Command
{
public:
    DeselectMeshCommand(ViewPortHolder* viewPortHolder)
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

    static constexpr std::string_view getCommandName() noexcept { return "DeselectMeshCommand"; }
};