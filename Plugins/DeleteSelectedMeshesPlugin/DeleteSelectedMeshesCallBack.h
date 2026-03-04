#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Patterns/Observer.h"

class DeleteSelectedMeshesCallBack : public Callback<DELETE_SELECTED_MESHES_CALLBACK>, public Observer
{
public:
    DeleteSelectedMeshesCallBack();

	void execute() override;

private:
};
