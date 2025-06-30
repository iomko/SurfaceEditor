# BrushToolCallBack
The BrushToolCallBack class responds to brush tool input by modifying the geometry of the mesh in the scene. It detects the closest mesh face under the brush cursor, calculates the brush's area of influence, and displaces vertices within this area along their averaged normals. After updating vertex positions, it refreshes the rendering buffers and updates the scene's spatial structures to reflect the geometry changes.

###### Public Methods
`void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams)`
Handles brush interaction by updating mesh vertices and faces within the brush radius based on brush strength and position.