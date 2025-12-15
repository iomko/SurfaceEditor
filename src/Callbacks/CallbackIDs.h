#pragma once
#include "CallbackRegister.h"
#include <typeindex>
#include <map>

#define BRUSH_TOOL_CALLBACK 0
#define CONNECT_EDGES_CALLBACK 1
#define CREATE_PRINT_STRUCTURE_CALLBACK 2
#define DELETE_FACE_CALLBACK 3
#define DELETE_MESH_CALLBACK 4
#define DELETE_SELECTED_FACES_CALLBACK 5
#define DELETE_SELECTED_MESHES_CALLBACK 6
#define DESELECT_FACE_CALLBACK 7
#define DESELECT_MESH_CALLBACK 8
#define EXPORT_MESHES_CALLBACK 9
#define IMPORT_MESHES_CALLBACK 10
#define MOVE_SELECTED_FACE_CALLBACK 11
#define MOVE_VERTEX_CALLBACK 12
#define SELECT_FACE_CALLBACK 13
#define SELECTION_LAYER_CALLBACK 14
#define SELECT_MESH_CALLBACK 15
#define SOLIDIFY_MESHES_CALLBACK 16

#define ADD_CUBE_CALLBACK 17
#define ADD_PLANE_CALLBACK 18
#define FETCH_SURFACE_CALLBACK 19

class TemplateOutlinerNodeAdderCallbackIDManger
{
public:
    static TemplateOutlinerNodeAdderCallbackIDManger& instance() {
        static TemplateOutlinerNodeAdderCallbackIDManger manager;
        return manager;
    }

    TemplateOutlinerNodeAdderCallbackIDManger()
    {
        freeIndex = 20;
        while (!CallbackRegistry::instance().availableID(freeIndex))
        {
            freeIndex++;
        }
                
    }
    int RegisterIndex(std::type_index type_id, bool child)
    {
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
    int GetIndex(std::type_index type_id, bool child)
    {
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