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

class KeyEvent : public Event
{
public:
	KeyEvent(int keyCode)
	{
		this->m_keyCode = keyCode;
	}
protected:
	int m_keyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
	KeyPressEvent(int keyCode) : KeyEvent(keyCode) {}
	virtual EventType getType() override
	{
		return EventType::KeyPress;
	}
	static EventType getStaticType()
	{
		return EventType::KeyPress;
	}
	virtual const char* getName() override {
		return "keyPressEvent";
	}
	virtual ~KeyPressEvent() override = default;
};

class KeyReleaseEvent : public KeyEvent
{
public:
	KeyReleaseEvent(int keyCode) : KeyEvent(keyCode) {}
	virtual EventType getType() override
	{
		return EventType::KeyRelease;
	}
	static EventType getStaticType()
	{
		return EventType::KeyRelease;
	}
	virtual const char* getName() override {
		return "keyReleaseEvent";
	}
	virtual ~KeyReleaseEvent() override = default;
};

class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent(double xOffset, double yOffset)
	{
		this->m_xOffset = xOffset;
		this->m_yOffset = yOffset;
	}
	virtual EventType getType() override
	{
		return EventType::MouseScroll;
	}
	static EventType getStaticType()
	{
		return EventType::MouseScroll;
	}
	virtual const char* getName() override {
		return "mouseScrollEvent";
	}
	virtual ~MouseScrollEvent() override = default;
private:
	double m_xOffset;
	double m_yOffset;
};


class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(double xPos, double yPos)
	{
		this->m_xPos = xPos;
		this->m_yPos = yPos;
	}
	virtual EventType getType() override
	{
		return EventType::MouseMove;
	}
	static EventType getStaticType()
	{
		return EventType::MouseMove;
	}
	virtual const char* getName() override {
		return "mouseMoveEvent";
	}
	virtual ~MouseMoveEvent() override = default;
private:
	double m_xPos;
	double m_yPos;
};

class MouseButtonPressEvent : public Event
{
public:
	MouseButtonPressEvent(int buttonCode)
	{
		this->m_buttonCode = buttonCode;
	}
	virtual EventType getType() override
	{
		return EventType::MouseButtonPress;
	}
	static EventType getStaticType()
	{
		return EventType::MouseButtonPress;
	}
	virtual const char* getName() override {
		return "mouseButtonPressEvent";
	}
	virtual ~MouseButtonPressEvent() override = default;
private:
	int m_buttonCode;
};

class MouseButtonReleaseEvent : public Event
{
public:
	MouseButtonReleaseEvent(int buttonCode)
	{
		this->m_buttonCode = buttonCode;
	}
	virtual EventType getType() override
	{
		return EventType::MouseButtonRelease;
	}

	static EventType getStaticType()
	{
		return EventType::MouseButtonRelease;
	}
	virtual const char* getName() override {
		return "mouseButtonReleaseEvent";
	}
	virtual ~MouseButtonReleaseEvent() override = default;
private:
	int m_buttonCode;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() = default;

	virtual ~WindowCloseEvent() override = default;

	static EventType getStaticType()
	{
		return EventType::WindowClose;
	}

	virtual EventType getType() override
	{
		return EventType::WindowClose;
	}
	virtual const char* getName() override
	{
		return "windowCloseEvent";
	}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
	{
		this->m_width = width;
		this->m_height = height;
	}

	virtual ~WindowResizeEvent() override = default;

	static EventType getStaticType()
	{
		return EventType::WindowResize;
	}

	virtual EventType getType() override
	{
		return EventType::WindowResize;
	}
	virtual const char* getName() override
	{
		return "windowResizeEvent";
	}

private:
	int m_width;
	int m_height;
};
