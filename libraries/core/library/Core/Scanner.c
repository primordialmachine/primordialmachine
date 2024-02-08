#include "Core/Scanner.h"

Core_defineObjectType("Core.Scanner",
                      Core_Scanner,
                      Core_Object);

static void Core_Scanner_destruct(Core_Scanner* SELF)
{/*Intentionally empty.*/}

static void Core_Scanner_constructDispatch(Core_Scanner_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Scanner_construct(Core_Scanner* SELF) {
  Core_BeginConstructor(Core_Scanner);
  Core_EndConstructor(Core_Scanner);
}
