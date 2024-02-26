#include "Core/Audials/System.h"

Core_defineObjectType("Core.Audials.System",
                      Core_Audials_System,
                      Core_System);

static void Core_Audials_System_destruct(Core_Audials_System* SELF) {
  CORE_UNREFERENCE(SELF->messageQueue);
  SELF->messageQueue = NULL;
}

static void Core_Audials_System_constructDispatch(Core_Audials_System_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Audials_System_construct(Core_Audials_System* SELF, Core_MessageQueue* messageQueue) {
  Core_BeginConstructor(Core_Audials_System);
  if (Core_System_construct(CORE_SYSTEM(SELF))) {
    return Core_Failure;
  }
  SELF->messageQueue = messageQueue;
  CORE_REFERENCE(SELF->messageQueue);
  Core_EndConstructor(Core_Audials_System);
}
