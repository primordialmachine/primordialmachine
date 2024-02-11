#include "Core/Assets/ImageOperation.h"

Core_defineObjectType("Core.Assets.ImageOperation",
                      Core_Assets_ImageOperation,
                      Core_Object);

static void Core_Assets_ImageOperation_destruct(Core_Assets_ImageOperation* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_ImageOperation_constructDispatch(Core_Assets_ImageOperation_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperation_construct(Core_Assets_ImageOperation* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperation);
  Core_EndConstructor(Core_Assets_ImageOperation);
}
