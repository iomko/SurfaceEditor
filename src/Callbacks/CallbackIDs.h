#pragma once
#include "CallbackRegister.h"
#include <typeindex>
#include <map>

enum CallbackIDS {
    BRUSH_TOOL_CALLBACK = 0,
    CONNECT_EDGES_CALLBACK,
    CREATE_PRINT_STRUCTURE_CALLBACK,
    DELETE_FACE_CALLBACK,
    DELETE_MESH_CALLBACK,
    DELETE_SELECTED_FACES_CALLBACK,
    DELETE_SELECTED_MESHES_CALLBACK,
    DESELECT_FACE_CALLBACK,
    DESELECT_MESH_CALLBACK,
    EXPORT_MESHES_CALLBACK,
    IMPORT_MESHES_CALLBACK,
    MOVE_SELECTED_FACE_CALLBACK,
    MOVE_VERTEX_CALLBACK,
    SELECT_FACE_CALLBACK,
    SELECTION_LAYER_CALLBACK,
    SELECT_MESH_CALLBACK,
    SOLIDIFY_MESHES_CALLBACK,
    ADD_CUBE_CALLBACK,
    ADD_PLANE_CALLBACK,
    FETCH_SURFACE_CALLBACK,
    HANDLE_GIZMO_CALLBACK,

    CALLBACK_IDS_COUNT   // ← počet prvkov
};


class TemplateOutlinerNodeAdderCallbackIDManger
{
public:
    static TemplateOutlinerNodeAdderCallbackIDManger& instance() {
        static TemplateOutlinerNodeAdderCallbackIDManger manager;
        return manager;
    }

    TemplateOutlinerNodeAdderCallbackIDManger()
    {
        freeIndex = CALLBACK_IDS_COUNT;
        while (!CallbackRegistry::instance().availableID(freeIndex))
        {
            freeIndex++;
        }
                
    }
    template<typename CallbackT>
    int RegisterIndex(bool child)
    {
        std::type_index type_id = std::type_index(typeid(CallbackT));
        int index = freeIndex;
        bool registered = false;
        if(child)
        {
            if(addChildOutlinerNodeCallbacks.contains(type_id))
            {
                return addChildOutlinerNodeCallbacks[type_id]; 
            }
            addChildOutlinerNodeCallbacks[type_id] = index; 
            registered = true;
        }
        else
        {
            if(addNewOutlinerNodeCallbacks.contains(type_id))
            {
                return addNewOutlinerNodeCallbacks[type_id]; 
            }
            addNewOutlinerNodeCallbacks[type_id] = index;
            registered = true;
        }
        if(registered)
        {
            freeIndex++;
            while (!CallbackRegistry::instance().availableID(freeIndex))
            {
                freeIndex++;
            }
        }
        return index;
    }
    template<typename CallbackT>
    int GetIndex(bool child)
    {
        std::type_index type_id = std::type_index(typeid(CallbackT));
        if(child)
        {
            return addChildOutlinerNodeCallbacks[type_id];
        }
        else
        {
            return addNewOutlinerNodeCallbacks[type_id];
        }
    }
private:
    int freeIndex;
    std::map<std::type_index, int> addNewOutlinerNodeCallbacks;
    std::map<std::type_index, int> addChildOutlinerNodeCallbacks;
};