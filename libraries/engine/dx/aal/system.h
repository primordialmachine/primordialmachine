#if !defined(DX_AAL_SYSTEM_H_INCLUDED)
#define DX_AAL_SYSTEM_H_INCLUDED

#include "dx/system.h"
#include "dx/aal/context.h"

DX_DECLARE_OBJECT_TYPE("dx.val.system",
                       dx_aal_system,
                       dx_system);

static inline dx_aal_system* DX_AAL_SYSTEM(void* p) {
  return (dx_aal_system*)p;
}

struct dx_aal_system {
  dx_system _parent;
  /// @brief A weak reference to the message queue.
  dx_msg_queue* msg_queue;
};

static inline dx_aal_system_dispatch* DX_AAL_SYSTEM_DISPATCH(void* p) {
  return (dx_aal_system_dispatch*)p;
}

struct dx_aal_system_dispatch {
  dx_system_dispatch _parent;
  dx_result (*get_context)(dx_aal_context**, dx_aal_system*);
};

/// @brief Construct this AL system.
/// @param SELF A pointer to this AAL system object.
/// @param msg_queue A pointer to the message queue.
/// @method-call
dx_result dx_aal_system_construct(dx_aal_system* SELF, dx_msg_queue* msg_queue);

/// @brief Get the AAL context.
/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @param SELF A pointer to this AAL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the AAL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this AAL system is not started.
/// @method-call
static inline dx_result dx_aal_system_get_context(dx_aal_context** RETURN, dx_aal_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_aal_system, get_context, RETURN, SELF); 
}

#endif // DX_VAL_SYSTEM_H_INCLUDED
