#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class DeleteSelectedMeshesCallBack : public Callback<>, public Observer
{
public:
    DeleteSelectedMeshesCallBack();

	void execute() override;

private:
};
