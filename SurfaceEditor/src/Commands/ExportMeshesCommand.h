#pragma once
#include <string_view>
#include <vector>
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

#include "../ViewPortHolder.h"

class ExportMeshesCommand : public Observable, public Command {
public:
    ExportMeshesCommand(ViewPortHolder* viewPortHolder)
    {
        addObserver(viewPortHolder);
    }

    virtual void execute() override {
        notifyObservers();
    }

    virtual void undo() override {

    }

    static constexpr std::string_view getCommandName() noexcept { return "ExportMeshesCommand"; }

public:
    std::string m_filePathMeshes = "";
};