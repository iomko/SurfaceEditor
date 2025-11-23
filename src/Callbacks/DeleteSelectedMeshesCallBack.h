#pragma once
#include "Callback.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"

class DeleteSelectedMeshesCallBack : public Callback<DELETE_SELECTED_MESHES_CALLBACK>, public Observer
{
public:
    DeleteSelectedMeshesCallBack();

	void execute() override;

private:
};
