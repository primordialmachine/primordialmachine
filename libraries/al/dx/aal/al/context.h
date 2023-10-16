#if !defined(DX_AAL_AL_CONTEXT_H_INCLUDED)
#define DX_AAL_AL_CONTEXT_H_INCLUDED

#include "dx/aal/context.h"
typedef struct dx_aal_al_system dx_aal_al_system;

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>

DX_DECLARE_OBJECT_TYPE("dx.aal.al.context",
                       dx_aal_al_context,
                       dx_aal_context);

static inline dx_aal_al_context* DX_AAL_AL_CONTEXT(void* p) {
  return (dx_aal_al_context*)p;
}

struct dx_aal_al_context {
  dx_aal_context _parent;
  ALCcontext* context;
  ALuint buffer;
};

static inline dx_aal_al_context_dispatch* DX_AAL_AL_CONTEXT_DISPATCH(void* p) {
  return (dx_aal_al_context_dispatch*)p;
}

struct dx_aal_al_context_dispatch {
  dx_aal_context_dispatch _parent;
};

/// @brief Construct this AAL AL context.
/// @param SELF A pointer to this AAL AL context.
/// @param system A pointer to the AAL AL system that creates this context.
/// @method-call
dx_result dx_aal_al_context_construct(dx_aal_al_context* SELF, dx_aal_al_system* system);

dx_result dx_aal_al_context_create(dx_aal_al_context** RETURN, dx_aal_al_system* system);

#endif // DX_AAL_AL_CONTEXT_H_INCLUDED