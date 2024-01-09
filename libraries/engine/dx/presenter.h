/// @file dx/engine/presenter.h
/// @brief An object used to present audial-visual aspects of an entity.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(DX_PRESENTER_H_INCLUDED)
#define DX_PRESENTER_INCLUDED

#include "dx/core.h"
typedef struct dx_aal_context dx_aal_context;
typedef struct dx_val_context dx_val_context;

Core_declareObjectType("dx.presenter",
                       dx_presenter,
                       Core_Object);

static inline dx_presenter* DX_PRESENTER(void* p) {
  return (dx_presenter*)p;
}

struct dx_presenter {
  Core_Object _parent;
  /// @brief A pointer to the VAL context.
  dx_val_context* val_context;
  /// @brief A pointer to the AAL context.
  dx_aal_context* aal_context;
};

static inline dx_presenter_Dispatch* DX_PRESENTER_DISPATCH(void* p) {
  return (dx_presenter_Dispatch*)p;
}

struct dx_presenter_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this presenter.
/// @param SELF A pointer to this presenter.
/// @param val_context A pointer to the VAL context.
/// The presenter acquires a reference to the context object.
/// @param aal_context A pointer to the AAL context.
/// The presenter acquires a reference to the context object.
/// @error #Core_Error_ArgumentInvalid @a val_context is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a aal_context is a null pointer.
/// @method-call
Core_Result dx_presenter_construct(dx_presenter* SELF, dx_val_context* val_context, dx_aal_context* aal_context);

/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @param SELF A pointer to this presenter.
/// @success <code>*RETURN</code> was assigned a pointer to the VAL context.
/// The caller acquired a reference to that context.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @method-call
Core_Result dx_presenter_get_val_context(dx_val_context** RETURN, dx_presenter* SELF);

/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @param SELF A pointer to this presenter.
/// @success <code>*RETURN</code> was assigned a pointer to the AAL context.
/// The caller acquired a reference to that context.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @method-call
Core_Result dx_presenter_get_aal_context(dx_aal_context** RETURN, dx_presenter* SELF);

#endif // DX_PRESENTER_H_INCLUDED
