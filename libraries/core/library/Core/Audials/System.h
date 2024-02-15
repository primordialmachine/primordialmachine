#if !defined(CORE_AUDIALS_SYSTEM_H_INCLUDED)
#define CORE_AUDIALS_SYSTEM_H_INCLUDED

#include "Core/MessageQueue.h"
#include "Core/System.h"
#include "Core/Audials/Context.h"

Core_declareObjectType("Core.Audials.System",
                       Core_Audials_System,
                       Core_System);

static inline Core_Audials_System* CORE_AUDIALS_SYSTEM(void* p) {
  return (Core_Audials_System*)p;
}

struct Core_Audials_System {
  Core_System _parent;
  /// @brief A reference to the message queue.
  Core_MessageQueue* messageQueue;
};

static inline Core_Audials_System_Dispatch* CORE_AUDIALS_SYSTEM_DISPATCH(void* p) {
  return (Core_Audials_System_Dispatch*)p;
}

struct Core_Audials_System_Dispatch {
  Core_System_Dispatch _parent;
  Core_Result (*getContext)(Core_Audials_Context**, Core_Audials_System*);
};

/// @param SELF A pointer to this audials system object.
/// @param messageQueue A pointer to the message queue.
/// The system acquires a reference to the message queue.
/// @constructor{Core_Audials_System}
Core_Result Core_Audials_System_construct(Core_Audials_System* SELF, Core_MessageQueue* messageQueue);

/// @brief Get the audials context.
/// @param RETURN A pointer to a <code>Core_Audials_Context*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the Core_Audials_Context object.
/// The caller acquired a reference to that object.
/// @warning
/// This function fails if this audials system is not started.
/// @method{Core_Audials_System}
static inline Core_Result Core_Audials_System_getContext(Core_Audials_Context** RETURN, Core_Audials_System* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Audials_System, getContext, RETURN, SELF); 
}

#endif // CORE_AUDIALS_SYSTEM_H_INCLUDED
