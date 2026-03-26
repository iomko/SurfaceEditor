#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class SplitEdgeCallBack : public Callback<SplitEdgeParams>, public Observer
{
public:
    SplitEdgeCallBack();

    void execute(const SplitEdgeParams& params) override;
private:

	void updateEdgeVao(Mesh* mesh, ExtendedEdge* edge);

	void createEdgeVao(Mesh* mesh, ExtendedEdge* edge);

	void updateFaceVao(Mesh* mesh, ExtendedFace* face);

	void createFaceVao(Mesh* mesh, ExtendedFace* face);
};
