#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Callbacks/CallbackIDs.h"

static AutoRegisterCallback<AddNewOutlinerNodeCallBack<Object>> regObject(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<Object>(false));
static AutoRegisterCallback<AddChildOutlinerNodeCallBack<Mesh>> regMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<Mesh>(true));

static AutoRegisterCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<PrintableMesh>(true));