#include "dx/assets/image_operations/color_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.color_fill",
                      dx_asset_image_operations_color_fill,
                      dx_asset_image_operation);

static void dx_asset_image_operations_color_fill_destruct(dx_asset_image_operations_color_fill* SELF) {
  if (SELF->color) {
    DX_UNREFERENCE(SELF->color);
    SELF->color = NULL;
  }
}

static void dx_asset_image_operations_color_fill_dispatch_construct(dx_asset_image_operations_color_fill_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_image_operations_color_fill_construct(dx_asset_image_operations_color_fill* SELF) {
  dx_rti_type* TYPE = dx_asset_image_operations_color_fill_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(SELF))) {
    return DX_FAILURE;
  }
  SELF->color = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_color_fill_create(dx_asset_image_operations_color_fill** RETURN) {
  DX_CREATE_PREFIX(dx_asset_image_operations_color_fill)
  if (dx_asset_image_operations_color_fill_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_color_fill_set_color(dx_asset_image_operations_color_fill* SELF, dx_assets_color_rgb_n8* color) {
  if (!SELF->color) {
    dx_string* name = NULL;
    if (dx_string_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return DX_FAILURE;
    }
    if (dx_asset_reference_create(&SELF->color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(color);
  if (SELF->color->object) {
    DX_UNREFERENCE(SELF->color->object);
  }
  SELF->color->object = DX_OBJECT(color);
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_color_fill_get_color(DX_RGB_N8* RETURN, dx_asset_image_operations_color_fill* SELF) {
  *RETURN = DX_ASSETS_COLOR_RGB_N8(SELF->color->object)->value;
  return DX_SUCCESS;
}
