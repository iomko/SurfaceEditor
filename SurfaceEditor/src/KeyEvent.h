#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "Event.h"

class KeyEvent : public Event
{
public:
	KeyEvent(int keyCode)
	{
		this->keyCode = keyCode;
	}
protected:
	int keyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
	KeyPressEvent(int keyCode) : KeyEvent(keyCode){}
	virtual EventType getType() override
	{
		return EventType::KeyPress;
	}
	static EventType getStaticType()
	{
		return EventType::KeyPress;
	}
	virtual const char* getName() override{
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
#endif
