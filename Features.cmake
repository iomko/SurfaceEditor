message(STATUS ">>> Features CMake included")

# Feature -> source files
set(FEATURE_MAP
    "MeshAdderCallables,src/Callables/MeshVaoInitCallable.cpp,src/Callables/SceneMeshAdderCallable.cpp"
    "AddCube,src/Commands/AddCubeCommand.cpp,src/Callbacks/AddCubeCallback.cpp,src/Callables/CubeVertexGenCallable.cpp"
    "AddPlane,src/Commands/AddPlaneCommand.cpp,src/Callbacks/AddPlaneCallback.cpp,src/Callables/PlaneVertexGenCallable.cpp"
    "FetchSurface,src/Commands/FetchSurfaceCommand.cpp,src/Callbacks/FetchSurfaceCallBack.cpp,src/Callables/FetchedSurfaceVertexGenCallable.cpp"
    "BrushTool,src/Commands/BrushToolCommand.cpp,src/Callbacks/BrushToolCallBack.cpp,src/Tools/BrushTool.cpp"
    "SolidifyMeshes,src/Callbacks/SolidifyMeshesCallBack.cpp,src/Callbacks/ConnectEdgesCallBack.cpp,src/Commands/SolidifyMeshesCommand.cpp,src/Callables/EdgesVaoInitCallable.cpp,src/Callables/FaceVaoInitCallable.cpp,src/Callables/SceneFacesAdderCallable.cpp,src/Callables/FacesVaoInitCallable.cpp"
    "CreatePrint,src/Callbacks/CreatePrintStructureCallBack.cpp,src/Commands/CreatePrintCommand.cpp"    
    "DeleteFace,src/Callbacks/DeleteFaceCallBack.cpp,src/Commands/DeleteFaceCommand.cpp"
    "DeleteMesh,src/Callbacks/DeleteMeshCallBack.cpp,src/Commands/DeleteMeshCommand.cpp"
    "DeleteSelectedFaces,src/Callbacks/DeleteSelectedFacesCallBack.cpp,src/Commands/DeleteSelectedFacesCommand.cpp"
    "DeleteSelectedMeshes,src/Callbacks/DeleteSelectedMeshesCallBack.cpp,src/Commands/DeleteSelectedMeshesCommand.cpp"
    "DeselectFace,src/Callbacks/DeselectFaceCallBack.cpp,src/Commands/DeselectFaceCommand.cpp,src/Tools/FaceDeselectionTool.cpp"
    "SelectFace,src/Callbacks/SelectFaceCallBack.cpp,src/Commands/SelectFaceCommand.cpp,src/Tools/FaceSelectionTool.cpp"
    "SelectMesh,src/Callbacks/SelectMeshCallBack.cpp,src/Commands/SelectMeshCommand.cpp,src/Tools/MeshSelectionTool.cpp"
    "ExportMeshes,src/Callbacks/ExportMeshesCallBack.cpp,src/Commands/ExportMeshesCommand.cpp"
    "ImportMeshes,src/Callbacks/ImportMeshesCallBack.cpp,src/Commands/ImportMeshesCommand.cpp"
    "MoveSelectedFaces,src/Callbacks/MoveSelectedFacesCallBack.cpp,src/Commands/MoveSelectedFacesCommand.cpp"
    "MoveSelectedMeshes,src/Callbacks/MoveSelectedMeshesCallBack.cpp,src/Commands/MoveSelectedMeshesCommand.cpp"
    "MoveVertex,src/Callbacks/MoveVertexCallBack.cpp,src/Commands/MoveVertexCommand.cpp"
    "MoveMesh,src/Callbacks/MoveMeshCallBack.cpp,src/Commands/MoveMeshCommand.cpp"
    "HandleGizmo,src/Callbacks/HandleGizmoCallBack.cpp,src/Commands/HandleGizmoCommand.cpp"
    "AddNodesToOutlinerLayer,src/Callbacks/AddOutlinerNodeCallBacks.cpp,src/Callables/MeshOutlinerAdderCallable.cpp"
    "DebugLayer,src/UI/DebugLayer.cpp"
    "AdditionLayer,src/UI/AdditionLayer.cpp"
    "GizmoLayer,src/UI/GizmoLayer.cpp"
    "ImGuiLayer,src/UI/ImGuiLayer.cpp"
    "ImportExportLayer,src/UI/ImportExportLayer.cpp"
    "ModifiersLayer,src/UI/ModifiersLayer.cpp"
    "OutlinerLayer,src/UI/OutlinerLayer.cpp"
    "ObjectManipulationLayer,src/UI/ObjectManipulationLayer.cpp"
    "ObjectsLayer,src/UI/ObjectsLayer.cpp"
    "PrintableMeshSettingsPopUpLayer,src/UI/PrintableMeshSettingsPopUpLayer.cpp"
    "RemovalLayer,src/UI/RemovalLayer.cpp"
    "SculptToolLayer,src/UI/SculptToolsLayer.cpp"
)


set(FEATURE_DEPENDENCIES
    "DeleteSelectedFaces,DeleteFace"
    "DeleteSelectedMeshes,DeleteMesh"
    "MoveSelectedFace,MoveVertex"
    "MoveSelectedMeshes,MoveMesh"
    "HandleGizmo,MoveSelectedMeshes"
    "HandleGizmo,MoveSelectedFaces"
    "BrushTool,MoveVertex"
    "MeshAdderCallables,AddNodesToOutlinerLayer"
    "AddCube,MeshAdderCallables"
    "AddPlane,MeshAdderCallables"
    "FetchSurface,MeshAdderCallables"
)

foreach(entry ${FEATURE_MAP})
    string(REPLACE "," ";" entry_list "${entry}")
    list(GET entry_list 0 featureName)
    string(TOUPPER "${featureName}" FEATURE_UPPER)
    option(FEATURE_${FEATURE_UPPER} "Enable feature ${featureName}" ON)
endforeach()

# Apply feature dependencies
set(changed TRUE)
while(changed)
    set(changed FALSE)
    foreach(dep IN LISTS FEATURE_DEPENDENCIES)
        # split string into list
        string(REPLACE "," ";" dep_list "${dep}")  # split into a proper list
        list(GET dep_list 0 feature)
        list(GET dep_list 1 required)

        string(TOUPPER "${feature}" FEATURE_UPPER)
        string(TOUPPER "${required}" REQUIRED_UPPER)

        if(FEATURE_${FEATURE_UPPER} AND NOT FEATURE_${REQUIRED_UPPER})
            set(FEATURE_${REQUIRED_UPPER} ON CACHE BOOL "" FORCE)
            message(STATUS "Auto enabling dependency: ${required} (needed by ${feature})")
            set(changed TRUE)
        endif()
    endforeach()
endwhile()


foreach(entry IN LISTS FEATURE_MAP)
    # split the string into a proper list
    string(REPLACE "," ";" entry_list "${entry}")  # now entry_list is a list
    list(GET entry_list 0 featureName)
    string(TOUPPER "${featureName}" FEATURE_UPPER)

    if(FEATURE_${FEATURE_UPPER})
        message(STATUS "Including feature: ${featureName}")

        list(LENGTH entry_list len)
        math(EXPR count "${len} - 1")

        foreach(i RANGE 1 ${count})
            list(GET entry_list ${i} file)
            list(APPEND SRC_FILES "${PROJECT_SOURCE_DIR}/${file}")
        endforeach()
    endif()
endforeach()


