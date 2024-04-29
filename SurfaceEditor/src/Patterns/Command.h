#pragma once

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() {}
    static constexpr std::string_view getCommandName() noexcept { return "Command"; }
};