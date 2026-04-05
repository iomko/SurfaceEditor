#pragma once
#include "Params/OperationParams.h"
#include "Callbacks/Callback.h"
#include "Patterns/Observer.h"


class ConnectEdgesCallBack : public Callback<EdgeConnectionsParams>, public Observer
{
public:
	void execute(const EdgeConnectionsParams& iParams) override;

private:

	void deleteEdgeVaoData(Mesh* mesh, ExtendedEdge* edge);
};
