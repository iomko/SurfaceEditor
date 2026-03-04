#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

#include "../../src/Callbacks/CallbackIDs.h"

class DeleteFaceCallBack : public Callback<DELETE_FACE_CALLBACK ,SingleFaceParams>, public Observer
{
public:
	void execute(const SingleFaceParams& iParams) override;

private:

	void deleteHalfEdgeStructureData(Mesh* mesh, ExtendedFace* face);


    void deleteLineBufferData(Mesh* mesh, ExtendedEdge* edge);

	//tymto vymazeme vao data z meshu
	void deleteTriangleBufferData(Mesh* mesh, ExtendedFace* face);
};
