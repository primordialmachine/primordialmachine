#if !defined(DX_AAL_SYSTEM_H_INCLUDED)
#define DX_AAL_SYSTEM_H_INCLUDED

#include "dx/core.h"
#include "dx/aal/context.h"

Core_declareObjectType("dx.val.system",
                       dx_aal_system,
                       dx_system);

static inline dx_aal_system* DX_AAL_SYSTEM(void* p) {
  return (dx_aal_system*)p;
}

struct dx_aal_system {
  Core_System _parent;
  /// @brief A weak reference to the message queue.
  Core_MessageQueue* msg_queue;
};

static inline dx_aal_system_Dispatch* DX_AAL_SYSTEM_DISPATCH(void* p) {
  return (dx_aal_system_Dispatch*)p;
}

struct dx_aal_system_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result (*get_context)(dx_aal_context**, dx_aal_system*);
};

/// @brief Construct this AL system.
/// @param SELF A pointer to this AAL system object.
/// @param msg_queue A pointer to the message queue.
/// @method-call
Core_Result dx_aal_system_construct(dx_aal_system* SELF, Core_MessageQueue* msg_queue);

/// @brief Get the AAL context.
/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @param SELF A pointer to this AAL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the AAL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this AAL system is not started.
/// @method-call
static inline Core_Result dx_aal_system_get_context(dx_aal_context** RETURN, dx_aal_system* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_aal_system, get_context, RETURN, SELF); 
}

#endif // DX_VAL_SYSTEM_H_INCLUDED
