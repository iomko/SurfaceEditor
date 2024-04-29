#pragma once
#include "Mesh.h"
#include <string>

class BaseExporter
{
public:
	virtual ~BaseExporter() {}

	virtual bool open(const std::string& filePath) = 0;
	virtual bool write() = 0;

	virtual void setMesh(Mesh* mesh) = 0;
};



