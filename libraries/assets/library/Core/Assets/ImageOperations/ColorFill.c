#include "Core/Assets/ImageOperations/ColorFill.h"

Core_defineObjectType("Core.Assets.ImageOperations.ColorFill",
                      Core_Assets_ImageOperations_ColorFill,
                      Core_Assets_ImageOperation);

static void Core_Assets_ImageOperations_ColorFill_destruct(Core_Assets_ImageOperations_ColorFill* SELF) {
  if (SELF->color) {
    CORE_UNREFERENCE(SELF->color);
    SELF->color = NULL;
  }
}

static void Core_Assets_ImageOperations_ColorFill_constructDispatch(Core_Assets_ImageOperations_ColorFill_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperations_ColorFill_construct(Core_Assets_ImageOperations_ColorFill* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperations_ColorFill);
  if (Core_Assets_ImageOperation_construct(CORE_ASSETS_IMAGEOPERATION(SELF))) {
    return Core_Failure;
  }
  SELF->color = NULL;
  Core_EndConstructor(Core_Assets_ImageOperations_ColorFill);
}

Core_Result Core_Assets_ImageOperations_ColorFill_create(Core_Assets_ImageOperations_ColorFill** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_ImageOperations_ColorFill);
  if (Core_Assets_ImageOperations_ColorFill_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_ColorFill_set_color(Core_Assets_ImageOperations_ColorFill* SELF, Core_Assets_ColorRgbN8* color) {
  if (!SELF->color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (Core_Assets_Ref_create(&SELF->color, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  CORE_REFERENCE(color);
  if (SELF->color->object) {
    CORE_UNREFERENCE(SELF->color->object);
  }
  SELF->color->object = CORE_OBJECT(color);
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_ColorFill_get_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_ColorFill* SELF) {
  *RETURN = CORE_ASSETS_COLORRGBN8(SELF->color->object)->value;
  return Core_Success;
}
