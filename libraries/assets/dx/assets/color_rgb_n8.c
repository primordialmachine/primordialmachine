#include "dx/assets/color_rgb_n8.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.color_rgb_n8",
                      dx_asset_color_rgb_n8,
                      dx_object);

static void dx_asset_color_rgb_n8_destruct(dx_asset_color_rgb_n8* SELF)
{/*Intentionally empty.*/}

static void dx_asset_color_rgb_n8_dispatch_construct(dx_asset_color_rgb_n8_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_color_rgb_n8_construct(dx_asset_color_rgb_n8* SELF, DX_RGB_N8 const* value) {
  dx_rti_type* TYPE = dx_asset_color_rgb_n8_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->value = *value;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_color_rgb_n8_create(dx_asset_color_rgb_n8** RESULT, DX_RGB_N8 const* value) {
  dx_asset_color_rgb_n8* SELF = DX_ASSET_COLOR_RGB_N8(dx_object_alloc(sizeof(dx_asset_color_rgb_n8)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_asset_color_rgb_n8_construct(SELF, value)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RESULT = SELF;
  return DX_SUCCESS;
}

DX_RGB_N8 const* dx_asset_color_rgb_n8_get_value(dx_asset_color_rgb_n8* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  return &SELF->value;
}
