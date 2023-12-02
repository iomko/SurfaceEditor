#ifndef LAYER_H
#define LAYER_H
#include "Event.h"

class Layer
{
public:
	virtual ~Layer(){}
	virtual void onAttach(){}
	virtual void onDetach(){}
	virtual void onUpdate(){}
	virtual void onImGuiRender(){}
	virtual void onEvent(Event& event){}
};

#endif

