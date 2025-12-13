#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"

static AutoRegisterTemplateCallback<AddNewOutlinerNodeCallBack<Mesh>> regMesh(TemplateCallbackIDManger::instance().RegisterIndex(typeid(Mesh), false));

static AutoRegisterTemplateCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateCallbackIDManger::instance().RegisterIndex(typeid(PrintableMesh), true));