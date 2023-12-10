#include "dx/assets/image_operations/color_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.image_operations.color_fill",
                      dx_assets_image_operations_color_fill,
                      dx_assets_image_operation);

static void dx_assets_image_operations_color_fill_destruct(dx_assets_image_operations_color_fill* SELF) {
  if (SELF->color) {
    DX_UNREFERENCE(SELF->color);
    SELF->color = NULL;
  }
}

static void dx_assets_image_operations_color_fill_constructDispatch(dx_assets_image_operations_color_fill_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_image_operations_color_fill_construct(dx_assets_image_operations_color_fill* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_image_operations_color_fill);
  if (dx_assets_image_operation_construct(DX_ASSETS_IMAGE_OPERATION(SELF))) {
    return Core_Failure;
  }
  SELF->color = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_image_operations_color_fill_create(dx_assets_image_operations_color_fill** RETURN) {
  DX_CREATE_PREFIX(dx_assets_image_operations_color_fill);
  if (dx_assets_image_operations_color_fill_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_image_operations_color_fill_set_color(dx_assets_image_operations_color_fill* SELF, dx_assets_color_rgb_n8* color) {
  if (!SELF->color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&SELF->color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(color);
  if (SELF->color->object) {
    DX_UNREFERENCE(SELF->color->object);
  }
  SELF->color->object = CORE_OBJECT(color);
  return Core_Success;
}

Core_Result dx_assets_image_operations_color_fill_get_color(DX_RGB_N8* RETURN, dx_assets_image_operations_color_fill* SELF) {
  *RETURN = DX_ASSETS_COLOR_RGB_N8(SELF->color->object)->value;
  return Core_Success;
}
