/// @file dx/engine/presenter.c
/// @brief An object used to present audial-visual aspects of an entity.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#include "dx/presenter.h"

DX_DEFINE_OBJECT_TYPE("dx.presenter",
                      dx_presenter,
                      dx_object);

static void dx_presenter_destruct(dx_presenter* SELF) {
  DX_UNREFERENCE(SELF->aal_context);
  SELF->aal_context = NULL;

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_presenter_dispatch_construct(dx_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_presenter_construct(dx_presenter* SELF, dx_val_context* val_context, dx_aal_context* aal_context) {
  dx_rti_type* TYPE = dx_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!val_context) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);
  if (!aal_context) {
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->aal_context = aal_context;
  DX_REFERENCE(SELF->aal_context);
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_presenter_get_val_context(dx_val_context** RETURN, dx_presenter* SELF) {
  DX_REFERENCE(SELF->val_context);
  *RETURN = SELF->val_context;
  return DX_SUCCESS;
}

dx_result dx_presenter_get_aal_context(dx_aal_context** RETURN, dx_presenter* SELF) {
  DX_REFERENCE(SELF->aal_context);
  *RETURN = SELF->aal_context;
  return DX_SUCCESS;
}
