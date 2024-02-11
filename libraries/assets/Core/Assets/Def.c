#include "Core/Assets/Def.h"

Core_defineObjectType("Core.Assets.Def",
                      Core_Assets_Def,
                      Core_Object);

static void Core_Assets_Def_destruct(Core_Assets_Def* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_Def_constructDispatch(Core_Assets_Def_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Def_construct(Core_Assets_Def* SELF) {
  Core_BeginConstructor(Core_Assets_Def);
  Core_EndConstructor(Core_Assets_Def);
}
