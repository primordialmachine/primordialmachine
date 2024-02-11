#include "Core/Assets/ColorRgbN8.h"

Core_defineObjectType("Core.Assets.ColorRgbN8",
                      Core_Assets_ColorRgbN8,
                      Core_Object);

static void Core_Assets_ColorRgbN8_destruct(Core_Assets_ColorRgbN8* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_ColorRgbN8_constructDispatch(Core_Assets_ColorRgbN8_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ColorRgbN8_construct(Core_Assets_ColorRgbN8* SELF, Core_InlineRgbN8 const* value) {
  Core_BeginConstructor(Core_Assets_ColorRgbN8);
  if (!value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->value = *value;
  Core_EndConstructor(Core_Assets_ColorRgbN8);
}

Core_Result Core_Assets_ColorRgbN8_create(Core_Assets_ColorRgbN8** RETURN, Core_InlineRgbN8 const* value) {
  DX_CREATE_PREFIX(Core_Assets_ColorRgbN8);
  if (Core_Assets_ColorRgbN8_construct(SELF, value)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Assets_ColorRgbN8_get_value(Core_InlineRgbN8* RETURN, Core_Assets_ColorRgbN8* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->value;
  return Core_Success;
}
