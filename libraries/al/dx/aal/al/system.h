#if !defined(DX_AAL_AL_SYSTEM_H_INCLUDED)
#define DX_AAL_AL_SYSTEM_H_INCLUDED

#include "Core/Audials/System.h"
typedef struct dx_aal_al_context dx_aal_al_context;

#define AL_LIBTYPE_STATIC
#include <AL/alc.h>

Core_declareObjectType("dx.aal.al.system",
                       dx_aal_al_system,
                       Core_Audials_System);

static inline dx_aal_al_system* DX_AAL_AL_SYSTEM(void* p) {
  return (dx_aal_al_system*)p;
}

struct dx_aal_al_system {
  Core_Audials_System _parent;
  ALCdevice* device;
};

static inline dx_aal_al_system_Dispatch* DX_AAL_AL_SYSTEM_DISPATCH(void* p) {
  return (dx_aal_al_system_Dispatch*)p;
}

struct dx_aal_al_system_Dispatch {
  Core_Audials_System_Dispatch _parent;
};

/// @brief Construct this AAL AL system.
/// @param SELF A pointer to this AAL AL system.
/// @param msg_queue A pointer to the message queue.
/// @method-call
Core_Result dx_aal_al_system_construct(dx_aal_al_system* SELF, Core_MessageQueue* msg_queue);

Core_Result dx_aal_al_system_create(dx_aal_al_system** RETURN, Core_MessageQueue* msg_queue);

/// @brief Get the AAL AL context.
/// @param RETURN A pointer to a <code>dx_aal_al_context*</code> variable.
/// @param SELF A pointer to this AAL AL system.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the AAL AL context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this AAL AL system is not started.
/// @method-call
Core_Result dx_aal_al_system_get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF);

#endif // DX_AAL_AL_SYSTEM_H_INCLUDED
