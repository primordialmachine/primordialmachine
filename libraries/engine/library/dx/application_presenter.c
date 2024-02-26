#include "dx/application_presenter.h"

Core_defineObjectType("Core.ApplicationPresenter",
                      Core_ApplicationPresenter,
                      Core_Object);

static void Core_ApplicationPresenter_destruct(Core_ApplicationPresenter* SELF)
{/*Intentionally empty.*/}

static void Core_ApplicationPresenter_constructDispatch(Core_ApplicationPresenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_ApplicationPresenter_construct(Core_ApplicationPresenter* SELF) {
  DX_CONSTRUCT_PREFIX(Core_ApplicationPresenter);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
