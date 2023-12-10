#include "dx/assets/image_operations/checkerboard_pattern_fill.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.image_operations.checkerboard_pattern_fill",
                      dx_assets_image_operations_checkerboard_pattern_fill,
                      dx_assets_image_operation);

static void dx_assets_image_operations_checkerboard_pattern_fill_destruct(dx_assets_image_operations_checkerboard_pattern_fill* SELF) {
  if (SELF->second_checker_color) {
    DX_UNREFERENCE(SELF->second_checker_color);
    SELF->second_checker_color = NULL;
  }
  if (SELF->first_checker_color) {
    DX_UNREFERENCE(SELF->first_checker_color);
    SELF->first_checker_color = NULL;
  }
}

static void dx_assets_image_operations_checkerboard_pattern_fill_constructDispatch(dx_assets_image_operations_checkerboard_pattern_fill_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_construct(dx_assets_image_operations_checkerboard_pattern_fill* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_image_operations_checkerboard_pattern_fill);
  if (dx_assets_image_operation_construct(DX_ASSETS_IMAGE_OPERATION(SELF))) {
    return Core_Failure;
  }
  SELF->first_checker_color = NULL;
  SELF->second_checker_color = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_create(dx_assets_image_operations_checkerboard_pattern_fill** RETURN) {
  DX_CREATE_PREFIX(dx_assets_image_operations_checkerboard_pattern_fill);
  if (dx_assets_image_operations_checkerboard_pattern_fill_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

#define PROPERTY(TYPE, PROPERTY_TYPE, PROPERTY_NAME) \
  Core_Result TYPE##_set_##PROPERTY_NAME(TYPE* SELF, PROPERTY_TYPE PROPERTY_NAME) { \
    SELF->PROPERTY_NAME = PROPERTY_NAME; \
    return Core_Success; \
  } \
  Core_Result TYPE##_get_##PROPERTY_NAME##(PROPERTY_TYPE* RETURN, TYPE* SELF) { \
    *RETURN = SELF->PROPERTY_NAME; \
    return Core_Success; \
  }

PROPERTY(dx_assets_image_operations_checkerboard_pattern_fill, Core_Size, number_of_checkers_horizontal)
PROPERTY(dx_assets_image_operations_checkerboard_pattern_fill, Core_Size, number_of_checkers_vertical)
PROPERTY(dx_assets_image_operations_checkerboard_pattern_fill, Core_Size, checker_size_horizontal)
PROPERTY(dx_assets_image_operations_checkerboard_pattern_fill, Core_Size, checker_size_vertical)

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_assets_image_operations_checkerboard_pattern_fill* SELF, dx_assets_color_rgb_n8* first_checker_color) {
  if (!SELF->first_checker_color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&SELF->first_checker_color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(first_checker_color);
  if (SELF->first_checker_color->object) {
    DX_UNREFERENCE(SELF->first_checker_color->object);
  }
  SELF->first_checker_color->object = CORE_OBJECT(first_checker_color);
  return Core_Success;
}

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_first_checker_color(DX_RGB_N8* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF) {
  *RETURN = DX_ASSETS_COLOR_RGB_N8(SELF->first_checker_color->object)->value;
  return Core_Success;
}

#undef PROPERTY

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_assets_image_operations_checkerboard_pattern_fill* SELF, dx_assets_color_rgb_n8* second_checker_color) {
  if (!SELF->second_checker_color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&SELF->second_checker_color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(second_checker_color);
  if (SELF->second_checker_color->object) {
    DX_UNREFERENCE(SELF->second_checker_color->object);
  }
  SELF->second_checker_color->object = CORE_OBJECT(second_checker_color);
  return Core_Success;
}

Core_Result dx_assets_image_operations_checkerboard_pattern_fill_get_second_checker_color(DX_RGB_N8* RETURN, dx_assets_image_operations_checkerboard_pattern_fill* SELF) {
  *RETURN = DX_ASSETS_COLOR_RGB_N8(SELF->second_checker_color)->value;
  return Core_Success;
}
