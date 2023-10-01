/// @file dx/engine/presenter.h
/// @brief An object used to present audial-visual aspects of an entity.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_PRESENTER_H_INCLUDED)
#define DX_PRESENTER_INCLUDED

#include "dx/core.h"
typedef struct dx_aal_context dx_aal_context;
typedef struct dx_val_context dx_val_context;

DX_DECLARE_OBJECT_TYPE("dx.presenter",
                       dx_presenter,
                       dx_object);

static inline dx_presenter* DX_PRESENTER(void* p) {
  return (dx_presenter*)p;
}

struct dx_presenter {
  dx_object _parent;
  /// @brief A pointer to the VAL context.
  dx_val_context* val_context;
  /// @brief A pointer to the AAL context.
  dx_aal_context* aal_context;
};

static inline dx_presenter_dispatch* DX_PRESENTER_DISPATCH(void* p) {
  return (dx_presenter_dispatch*)p;
}

struct dx_presenter_dispatch {
  dx_object_dispatch _dispatch;
};

/// @brief Construct this presenter.
/// @param SELF A pointer to this presenter.
/// @param val_context A pointer to the VAL context.
/// The presenter acquires a reference to the context object.
/// @param aal_context A pointer to the AAL context.
/// The presenter acquires a reference to the context object.
/// @error #DX_ERROR_INVALID_ARGUMENT @a val_context is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a aal_context is a null pointer.
/// @method-call
dx_result dx_presenter_construct(dx_presenter* SELF, dx_val_context* val_context, dx_aal_context* aal_context);

/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @param SELF A pointer to this presenter.
/// @success <code>*RETURN</code> was assigned a pointer to the VAL context.
/// The caller acquired a reference to that context.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @method-call
dx_result dx_presenter_get_val_context(dx_val_context** RETURN, dx_presenter* SELF);

/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @param SELF A pointer to this presenter.
/// @success <code>*RETURN</code> was assigned a pointer to the AAL context.
/// The caller acquired a reference to that context.
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @method-call
dx_result dx_presenter_get_aal_context(dx_aal_context** RETURN, dx_presenter* SELF);

#endif // DX_PRESENTER_H_INCLUDED
