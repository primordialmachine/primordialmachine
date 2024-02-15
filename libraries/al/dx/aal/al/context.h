#if !defined(DX_AAL_AL_CONTEXT_H_INCLUDED)
#define DX_AAL_AL_CONTEXT_H_INCLUDED

#include "Core/Audials/Context.h"
typedef struct dx_aal_al_system dx_aal_al_system;

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

Core_declareObjectType("dx.aal.al.context",
                       dx_aal_al_context,
                       Core_Audials_Context);

static inline dx_aal_al_context* DX_AAL_AL_CONTEXT(void* p) {
  return (dx_aal_al_context*)p;
}

struct dx_aal_al_context {
  Core_Audials_Context _parent;
  ALCcontext* context;
  ALuint buffer;
  ALuint source;
};

static inline dx_aal_al_context_Dispatch* DX_AAL_AL_CONTEXT_DISPATCH(void* p) {
  return (dx_aal_al_context_Dispatch*)p;
}

struct dx_aal_al_context_Dispatch {
  Core_Audials_Context_Dispatch _parent;
};

/// @brief Construct this AAL AL context.
/// @param SELF A pointer to this AAL AL context.
/// @param system A pointer to the AAL AL system that creates this context.
/// @method-call
Core_Result dx_aal_al_context_construct(dx_aal_al_context* SELF, dx_aal_al_system* system);

Core_Result dx_aal_al_context_create(dx_aal_al_context** RETURN, dx_aal_al_system* system);

#endif // DX_AAL_AL_CONTEXT_H_INCLUDED
