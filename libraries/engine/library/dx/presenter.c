/// @file dx/engine/presenter.c
/// @brief An object used to present audial-visual aspects of an entity.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "dx/presenter.h"

Core_defineObjectType("dx.presenter",
                      dx_presenter,
                      Core_Object);

static void dx_presenter_destruct(dx_presenter* SELF) {
  CORE_UNREFERENCE(SELF->aal_context);
  SELF->aal_context = NULL;

  CORE_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_presenter_constructDispatch(dx_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_presenter_construct(dx_presenter* SELF, Core_Visuals_Context* val_context, Core_Audials_Context* aal_context) {
  DX_CONSTRUCT_PREFIX(dx_presenter);
  if (!val_context) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->val_context = val_context;
  CORE_REFERENCE(SELF->val_context);
  if (!aal_context) {
    CORE_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->aal_context = aal_context;
  CORE_REFERENCE(SELF->aal_context);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_presenter_get_val_context(Core_Visuals_Context** RETURN, dx_presenter* SELF) {
  CORE_REFERENCE(SELF->val_context);
  *RETURN = SELF->val_context;
  return Core_Success;
}

Core_Result dx_presenter_get_aal_context(Core_Audials_Context** RETURN, dx_presenter* SELF) {
  CORE_REFERENCE(SELF->aal_context);
  *RETURN = SELF->aal_context;
  return Core_Success;
}
