export module EventSystem.Event.MouseEvent;
import EventSystem.Event;

export class MouseScrollEvent : public Event
{
public:
	MouseScrollEvent(double xOffset, double yOffset)
	{
		this->xOffset = xOffset;
		this->yOffset = yOffset;
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
	double xOffset;
	double yOffset;
};


export class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(double xPos, double yPos)
	{
		this->xPos = xPos;
		this->yPos = yPos;
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
	double xPos;
	double yPos;
};

export class MouseButtonPressEvent : public Event
{
public:
	MouseButtonPressEvent(int buttonCode)
	{
		this->buttonCode = buttonCode;
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
	int buttonCode;
};

export class MouseButtonReleaseEvent : public Event
{
public:
	MouseButtonReleaseEvent(int buttonCode)
	{
		this->buttonCode = buttonCode;
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
	int buttonCode;
};
