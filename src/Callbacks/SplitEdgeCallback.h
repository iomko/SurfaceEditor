#pragma once
#include "../Params/OperationParams.h"
#include "Callback.h"
#include "../Patterns/Observer.h"

#include "CallbackIDs.h"


class SplitEdgeCallback : public Callback<SPLIT_EDGE_CALLBACK, SplitEdgeParams>, public Observer
{
public:
    SplitEdgeCallback();

    void execute(const SplitEdgeParams& params) override;
private:

	void updateEdgeVao(Mesh* mesh, ExtendedEdge* edge);

	void createEdgeVao(Mesh* mesh, ExtendedEdge* edge);

	void updateFaceVao(Mesh* mesh, ExtendedFace* face);

	void createFaceVao(Mesh* mesh, ExtendedFace* face);
};
