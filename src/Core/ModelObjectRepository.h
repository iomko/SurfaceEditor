#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../Scene/ModelObject.h"

class ModelObjectRepository
{
public:
    ModelObjectRepository() = default;
    ~ModelObjectRepository() = default;

    ModelObject* addObject(std::unique_ptr<ModelObject> object);
    ModelObject* createObject(const std::string& name);
    bool removeObject(ModelObject* object);

    void clear();


private:
    std::vector<std::unique_ptr<ModelObject>> m_objects;
};