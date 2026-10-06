#pragma once
#include "CallbackRegister.h"
#include <typeindex>
#include <map>

class TemplateOutlinerNodeAdderCallbackIDManger
{
public:
    static TemplateOutlinerNodeAdderCallbackIDManger& instance() {
        static TemplateOutlinerNodeAdderCallbackIDManger manager;
        return manager;
    }

    TemplateOutlinerNodeAdderCallbackIDManger()
    {
        freeIndex = 0;
        while (!CallbackRegistry::instance().availableID(std::to_string(freeIndex)))
        {
            freeIndex++;
        }
                
    }
    template<typename CallbackT>
    std::string RegisterIndex(bool child)
    {
        std::type_index type_id = std::type_index(typeid(CallbackT));
        int index = freeIndex;
        bool registered = false;
        if(child)
        {
            if(addChildOutlinerNodeCallbacks.contains(type_id))
            {
                return std::to_string(addChildOutlinerNodeCallbacks[type_id]);
            }
            addChildOutlinerNodeCallbacks[type_id] = index; 
            registered = true;
        }
        else
        {
            if(addNewOutlinerNodeCallbacks.contains(type_id))
            {
                return std::to_string(addNewOutlinerNodeCallbacks[type_id]);
            }
            addNewOutlinerNodeCallbacks[type_id] = index;
            registered = true;
        }
        if(registered)
        {
            freeIndex++;
            while (!CallbackRegistry::instance().availableID(std::to_string(freeIndex)))
            {
                freeIndex++;
            }
        }
        return std::to_string(index);
    }
    template<typename CallbackT>
    std::string GetIndex(bool child)
    {
        std::type_index type_id = std::type_index(typeid(CallbackT));
        int id;
        if(child)
        {
            id = addChildOutlinerNodeCallbacks[type_id];
        }
        else
        {
            id = addNewOutlinerNodeCallbacks[type_id];
        }
        return std::to_string(id);
    }
private:
    int freeIndex;
    std::map<std::type_index, int> addNewOutlinerNodeCallbacks;
    std::map<std::type_index, int> addChildOutlinerNodeCallbacks;
};