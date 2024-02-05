#include "dx/assets/color_rgb_n8.h"

Core_defineObjectType("dx.assets.color_rgb_n8",
                      dx_assets_color_rgb_n8,
                      Core_Object);

static void dx_assets_color_rgb_n8_destruct(dx_assets_color_rgb_n8* SELF)
{/*Intentionally empty.*/}

static void dx_assets_color_rgb_n8_constructDispatch(dx_assets_color_rgb_n8_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_color_rgb_n8_construct(dx_assets_color_rgb_n8* SELF, Core_InlineRgbN8 const* value) {
  DX_CONSTRUCT_PREFIX(dx_assets_color_rgb_n8);
  if (!value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->value = *value;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_color_rgb_n8_create(dx_assets_color_rgb_n8** RETURN, Core_InlineRgbN8 const* value) {
  DX_CREATE_PREFIX(dx_assets_color_rgb_n8);
  if (dx_assets_color_rgb_n8_construct(SELF, value)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_color_rgb_n8_get_value(Core_InlineRgbN8* RETURN, dx_assets_color_rgb_n8* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->value;
  return Core_Success;
}
