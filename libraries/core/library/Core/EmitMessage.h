#if !defined(CORE_EMITMESSAGE_H_INCLUDED)
#define CORE_EMITMESSAGE_H_INCLUDED

#include "Core/Message.h"
#include "dx/core/string.h"

// The opaque type of an "emit" message.
// An "emit" message makes the engine print out the specifid string on the output device for emit messages.
Core_declareObjectType("Core.EmitMessage",
                       Core_EmitMessage,
                       Core_Message);

static inline Core_EmitMessage* CORE_EMITMESSAGE(void* p) {
  return (Core_EmitMessage*)p;
}

struct Core_EmitMessage {
  Core_Message _parent;
  Core_String* message;
};

static inline Core_EmitMessage_Dispatch* CORE_EMITMESSAGE_DISPATCH(void* p) {
  return (Core_EmitMessage_Dispatch*)p;
}

struct Core_EmitMessage_Dispatch {
  Core_Message_Dispatch _parent;
};

/// @constructor{Core_EmitMessage}
Core_Result Core_EmitMessage_construct(Core_EmitMessage* SELF, Core_String* message);

/// @brief Create an "emit" message.
/// @param RETURN A pointer to a <code>Core_EmitMessage*</code> variable.
/// @param message The message.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the Core_EmitMessage object.
/// The caller acquired a reference to the returned object.
/// @create-operator{Core_EmitMessage}
Core_Result Core_EmitMessage_create(Core_EmitMessage** RETURN, Core_String* message);

Core_Result Core_EmitMessage_get(Core_String** RETURN, Core_EmitMessage* SELF);

#endif // CORE_EMITMESSAGE_H_INCLUDED
