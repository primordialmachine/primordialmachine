#include "Core/FontSystem.h"

Core_defineObjectType("Core.FontSystem",
                      Core_FontSystem,
                      Core_System);

static void Core_FontSystem_destruct(Core_FontSystem* SELF)
{}

static void Core_FontSystem_constructDispatch(Core_FontSystem_Dispatch* SELF)
{}

Core_Result Core_FontSystem_construct(Core_FontSystem* SELF) {
  Core_BeginConstructor(Core_FontSystem);
  if (Core_System_construct(CORE_SYSTEM(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_FontSystem);
}
