#include "dx/assets/image_operations/color_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.color_fill",
                      dx_asset_image_operations_color_fill,
                      dx_asset_image_operation);

static void dx_asset_image_operations_color_fill_destruct(dx_asset_image_operations_color_fill* SELF)
{/*Intentionally empty.*/}

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

dx_result dx_asset_image_operations_color_fill_set_color(dx_asset_image_operations_color_fill* SELF, dx_asset_color_rgb_n8* color) {
  SELF->color = color->value;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_color_fill_get_color(DX_RGB_N8* RETURN, dx_asset_image_operations_color_fill* SELF) {
  *RETURN = SELF->color;
  return DX_SUCCESS;
}
