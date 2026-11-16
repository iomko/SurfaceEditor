#include "ModelObjectRepository.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

ModelObject *ModelObjectRepository::addObject(std::unique_ptr<ModelObject> object)
{
    if (!object)
    {
        throw std::invalid_argument("Cannot add a null object.");
    }

    ModelObject *rawObject = object.get();
    m_objects.push_back(std::move(object));
    return rawObject;
}

ModelObject *ModelObjectRepository::createObject(const std::string &name)
{
    return addObject(std::make_unique<ModelObject>(name));
}

bool ModelObjectRepository::removeObject(ModelObject *object)
{
    const auto it = std::find_if(
        m_objects.begin(),
        m_objects.end(),
        [object](const std::unique_ptr<ModelObject> &candidate)
        {
            return candidate.get() == object;
        });
    if (it == m_objects.end() || !(*it)->getMeshes().empty())
    {
        return false;
    }

    m_objects.erase(it);
    return true;
}

void ModelObjectRepository::clear()
{
    m_objects.clear();
}
