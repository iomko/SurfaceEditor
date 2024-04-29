#pragma once
class Callback {
public:
    virtual ~Callback() = default;
    virtual void execute() = 0;
};