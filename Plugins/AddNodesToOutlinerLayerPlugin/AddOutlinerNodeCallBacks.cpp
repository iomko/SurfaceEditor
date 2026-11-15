#include "AddChildOutlinerNodeCallBack.h"
#include "AddNewOutlinerNodeCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Callbacks/CallbackIDs.h"

static AutoRegisterCallback<AddNewOutlinerNodeCallBack<ModelObject>> regObject(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<ModelObject>(false));
static AutoRegisterCallback<AddChildOutlinerNodeCallBack<Mesh>> regMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<Mesh>(true));

static AutoRegisterCallback<AddChildOutlinerNodeCallBack<PrintableMesh>> regPrintableMesh(TemplateOutlinerNodeAdderCallbackIDManger::instance().RegisterIndex<PrintableMesh>(true));