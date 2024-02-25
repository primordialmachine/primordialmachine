#include "Core/Font.h"

Core_defineObjectType("Core.Font",
                      Core_Font,
                      Core_Object);

static void Core_Font_destruct(Core_Font* SELF)
{/*Intentionally empty.*/}

static void Core_Font_constructDispatch(Core_Font_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Font_construct(Core_Font* SELF) {
  Core_BeginConstructor(Core_Font);
  Core_EndConstructor(Core_Font);
}
