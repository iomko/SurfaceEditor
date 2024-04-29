#pragma once
#include <vector>
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

struct PlaneProperties
{
    float m_size;
    int m_subdivisionLevel;
};

class AddPlaneCommand : public Observable, public Command {
public:
    AddPlaneCommand(ViewPortHolder* viewPortHolder, PlaneProperties& planeProperties) : m_planeProperties(planeProperties)
    {
        addObserver(viewPortHolder);
    }

    virtual void execute() override {
        notifyObservers();
    }

    virtual void undo() override {

    }

    PlaneProperties& getPlaneProperties()
    {
        return m_planeProperties;
    }

    static constexpr std::string_view getCommandName() noexcept { return "AddPlaneCommand"; }

private:
    PlaneProperties& m_planeProperties;
};