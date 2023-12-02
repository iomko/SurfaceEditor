#pragma once
#include <iostream>
#include <ostream>

#include "Layer.h"

class TestLayer : public Layer
{
public:
	virtual ~TestLayer() override
	{
		
	}
	virtual void onAttach() override
	{
		
	}
	virtual void onDetach() override
	{
		
	}
	virtual void onUpdate() override
	{
		
	}
	virtual void onEvent(Event& event) override
	{
		//std::cout << event.getName() << std::endl;
	}
};