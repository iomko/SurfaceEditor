#pragma once
#include "Mesh.h"
#include <string>
#include <vector>

class BaseImporter
{
public:
	virtual ~BaseImporter() {}

	virtual bool read() = 0;
	virtual std::vector<Mesh*> getMeshes() = 0;
};

