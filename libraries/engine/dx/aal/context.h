#if !defined(DX_AAL_CONTEXT_H_INCLUDED)
#define DX_AAL_CONTEXT_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.aal.context",
                       dx_aal_context,
                       dx_object)

static inline dx_aal_context* DX_AAL_CONTEXT(void* p) {
  return (dx_aal_context*)p;
}

struct dx_aal_context {
  dx_object _parent;
};

static inline dx_aal_context_dispatch* DX_AAL_CONTEXT_DISPATCH(void* p) {
  return (dx_aal_context_dispatch*)p;
}

struct dx_aal_context_dispatch {
  dx_object_dispatch _parent;
  dx_result(*start)(dx_aal_context* SELF);
  dx_result(*stop)(dx_aal_context* SELF);
};

dx_result dx_aal_context_construct(dx_aal_context* SELF);

/// @brief Start playing the sound.
/// @param SELF A pointer to this context.
/// @param duration The duration in milliseconds. 
/// @method-call
/// @remarks This function does nothing if the sine wave is already started.
static inline dx_result dx_aal_al_context_start(dx_aal_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_aal_context, start, SELF);
}

/// @brief Stop playing the sound.
/// @param SELF A pointer to this context.
/// @method-call
/// @remarks This function does nothing if the sound is alreay stopped.
static inline dx_result dx_aal_al_context_stop(dx_aal_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_aal_context, stop, SELF);
}

#endif // DX_AAL_CONTEXT_H_INCLUDED
