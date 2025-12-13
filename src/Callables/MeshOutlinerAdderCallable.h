#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"

#include "../Callbacks/CallbackIDs.h"
#include "../Callbacks/CallbackRegister.h"

class MeshOutlinerAdderCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;
		auto* addNodeCallback = CallbackRegistry::instance().getCallback(TemplateCallbackIDManger::instance().GetIndex(typeid(Mesh),false));
		Observable* observable = dynamic_cast<Observable*>(addNodeCallback);
		if(observable)
		{
			AddNewOutlinerNodeCallBackParams<Mesh> params;
			params.id = 0;
			params.name = "Mesh";
			params.data = mesh;

			observable->notifyObservers(params);
		}
	}
};
