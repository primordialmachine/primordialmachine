#if !defined(CORE_AUDIALS_CONTEXT_H_INCLUDED)
#define CORE_AUDIALS_CONTEXT_H_INCLUDED

#include "Core/Context.h"

Core_declareObjectType("Core.Audials.Context",
                       Core_Audials_Context,
                       Core_Context);

static inline Core_Audials_Context* CORE_AUDIALS_CONTEXT(void* p) {
  return (Core_Audials_Context*)p;
}

struct Core_Audials_Context {
  Core_Context _parent;
};

static inline Core_Audials_Context_Dispatch* CORE_AUDIALS_CONTEXT_DISPATCH(void* p) {
  return (Core_Audials_Context_Dispatch*)p;
}

struct Core_Audials_Context_Dispatch {
  Core_Context_Dispatch _parent;
  Core_Result(*startSound)(Core_Audials_Context* SELF);
  Core_Result(*stopSound)(Core_Audials_Context* SELF);
};

Core_Result Core_Audials_Context_construct(Core_Audials_Context* SELF);

/// @brief Start playing the sound.
/// @param SELF A pointer to this context.
/// @param duration The duration in milliseconds. 
/// @remarks This function does nothing if the sine wave is already started.
/// @method{startSound}
static inline Core_Result Core_Audials_Context_startSound(Core_Audials_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Audials_Context, startSound, SELF);
}

/// @brief Stop playing the sound.
/// @param SELF A pointer to this context.
/// @remarks This function does nothing if the sound is alreay stopped.
/// @method{stopSound}
static inline Core_Result Core_Audials_Context_stopStop(Core_Audials_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Audials_Context, stopSound, SELF);
}

/// @brief Get the volume.
/// @param RETURN A pointer to a <code>Core_Real32</code> variable.
/// @param SELF A pointer to this context.
/// @post <code>*RETURN</code> was assigned the volume.
/// @method{getVolume}
static inline Core_Result Core_Audials_Context_getVolume(Core_Real32* RETURN, Core_Audials_Context* SELF);

/// @brief Set the volume.
/// @param volume The volumne.
/// @param SELF A pointer to this context.
/// @success The device's volumne was assigned the volumne specified by <code>volumne</code>.
/// @method{getVolume}
static inline Core_Result Core_Audials_Context_setVolume(Core_Audials_Context* SELF, Core_Real32 volume);

#endif // CORE_AUDIALS_CONTEXT_H_INCLUDED
