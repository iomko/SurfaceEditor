#pragma once
#include "Callback.h"
#include "../Commands/ObjectTransformationCommand.h"

#include "../ViewPortHolder.h"

#include <random>

class ObjectTransformCallBack : public Callback
{
public:
	ObjectTransformCallBack(ViewPortHolder* viewPortHolder, ObjectTransformationCommand* command)
	{
		m_viewPortHolder = viewPortHolder;
		m_command = command;
	}

	virtual void execute() override
	{
		//to znamena ze potrebujem aplikovat shader, ktory mi zobere moje vertices a na vsetky aplikuje este k tomu transformaciu.


	}

private:
	ViewPortHolder* m_viewPortHolder;
	ObjectTransformationCommand* m_command;

};
