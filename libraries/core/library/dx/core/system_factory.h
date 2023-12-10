#if !defined(DX_SYSTEM_FACTORY_H_INCLUDED)
#define DX_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"
typedef struct dx_system dx_system;

DX_DECLARE_OBJECT_TYPE("dx.system_factory",
                       dx_system_factory,
                       Core_Object);

static inline dx_system_factory* DX_SYSTEM_FACTORY(void* p) {
  return (dx_system_factory*)p;
}

struct dx_system_factory {
  Core_Object _parent;
};

static inline dx_system_factory_dispatch* DX_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_system_factory_dispatch*)p;
}

struct dx_system_factory_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*create_system)(dx_system**,dx_system_factory*, dx_msg_queue*);
};

/// @brief Construct this system factory.
/// @param SELF A pointer to this system factory.
/// @method-call
Core_Result dx_system_factory_construct(dx_system_factory* SELF);

/// @brief Create the system.
/// @param RETURN A pointer to a <code>dx_system*</code> variable.
/// @param SELF A pointer to this system factory.
/// @param msg_queue A pointer to the message queue.
/// @method-call.
static inline Core_Result dx_system_factory_create_system(dx_system** RETURN, dx_system_factory* SELF, dx_msg_queue* msg_queue) {
  DX_OBJECT_VIRTUALCALL(dx_system_factory, create_system, RETURN, SELF, msg_queue);
}

#endif // DX_SYSTEM_FACTORY_H_INCLUDED
