#pragma once

enum class EventType
{
	MouseMove, MouseButtonPress, MouseButtonRelease, MouseScroll,
	KeyPress, KeyRelease,
	WindowResize, WindowClose
};

class Event
{
public:
	virtual ~Event() = default;
	virtual EventType getType() = 0;
	virtual const char* getName() = 0;
	bool isHandled = false;
};