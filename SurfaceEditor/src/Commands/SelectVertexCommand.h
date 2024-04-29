#pragma once

#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortHolder.h"

class SelectVertexCommand : public Observable, public Command
{
public:
    SelectVertexCommand(ViewPortHolder* viewPortHolder)
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

    static constexpr std::string_view getCommandName() noexcept { return "SelectVertexCommand"; }

};