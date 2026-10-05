#pragma once

class IWindow
{
public:
    IWindow() = default;
    
    virtual ~IWindow() = default;

    virtual void setWindowSizeAndPosition() = 0;
};