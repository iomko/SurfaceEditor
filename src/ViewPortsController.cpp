#include "ViewPortsController.h"

// Define statics once - shared across all TUs
Window* ViewPortsHolderContext::s_window = nullptr;
Camera* ViewPortsHolderContext::s_camera = nullptr;
ViewPortsController* ViewPortsHolderContext::s_viewPortsController = nullptr;
SelectionController* ViewPortsHolderContext::s_selectionController = nullptr;