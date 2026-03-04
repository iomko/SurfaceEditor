#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"

static AutoRegisterTemplateCallback<AddNewOutlinerNodeCallBack<Mesh>> regMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<Mesh>(false));

static AutoRegisterTemplateCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<PrintableMesh>(true));