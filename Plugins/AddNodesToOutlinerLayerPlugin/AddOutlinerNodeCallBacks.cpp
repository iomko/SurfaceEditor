#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Callbacks/CallbackIDs.h"

static AutoRegisterCallback<AddNewOutlinerNodeCallBack<Mesh>> regMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<Mesh>(false));

static AutoRegisterCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<PrintableMesh>(true));