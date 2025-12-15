#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"

static AutoRegisterTemplateCallback<AddNewOutlinerNodeCallBack<Mesh>> regMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex(typeid(Mesh), false));

static AutoRegisterTemplateCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex(typeid(PrintableMesh), true));