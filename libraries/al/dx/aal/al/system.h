#if !defined(DX_AAL_AL_SYSTEM_H_INCLUDED)
#define DX_AAL_AL_SYSTEM_H_INCLUDED

#include "dx/aal/system.h"
#include "dx/aal/al/context.h"

DX_DECLARE_OBJECT_TYPE("dx.aal.al.system",
                       dx_aal_al_system,
                       dx_aal_system);

static inline dx_aal_al_system* DX_AAL_AL_SYSTEM(void* p) {
  return (dx_aal_al_system*)p;
}

struct dx_aal_al_system {
  dx_aal_system _parent;
};

static inline dx_aal_al_system_dispatch* DX_AAL_AL_SYSTEM_DISPATCH(void* p) {
  return (dx_aal_al_system_dispatch*)p;
}

struct dx_aal_al_system_dispatch {
  dx_aal_system_dispatch _parent;
};

/// @brief Construct this AAL AL system.
/// @param SELF A pointer to this AAL AL system.
/// @param msg_queue A pointer to the message queue.
/// @method-call
dx_result dx_aal_al_system_construct(dx_aal_al_system* SELF, dx_msg_queue* msg_queue);

dx_result dx_aal_al_system_create(dx_aal_al_system** RETURN, dx_msg_queue* msg_queue);

/// @brief Get the AAL AL context.
/// @param RETURN A pointer to a <code>dx_aal_al_context*</code> variable.
/// @param SELF A pointer to this AAL AL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the AAL AL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this AAL AL system is not started.
/// @method-call
dx_result dx_aal_al_system_get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF);

#endif // DX_AAL_AL_SYSTEM_H_INCLUDED
