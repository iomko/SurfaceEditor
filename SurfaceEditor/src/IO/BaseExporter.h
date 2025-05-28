#pragma once
#include "../Scene/Mesh.h"
#include <string>

class BaseExporter
{
public:
	virtual ~BaseExporter() {}

	virtual bool write() = 0;
	virtual void parseMesh(Mesh* mesh) = 0;
};