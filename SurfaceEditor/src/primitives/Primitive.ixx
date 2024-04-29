module;
#include <any>
#include "../TriangleData.h"
export module Geometry.Primitive;

class Primitive
{
protected:
	std::vector<float> m_vertexData;
	virtual void calculateVertexData() = 0;
public:
	virtual ~Primitive() = default;
	virtual std::vector<std::any>& getVertexData() = 0;
};