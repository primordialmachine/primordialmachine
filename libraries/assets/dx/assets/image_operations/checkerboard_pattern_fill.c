#include "dx/assets/image_operations/checkerboard_pattern_fill.h"


DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.checkerboard_pattern_fill",
                      dx_asset_image_operations_checkerboard_pattern_fill,
                      dx_asset_image_operation);

static void dx_asset_image_operations_checkerboard_pattern_fill_destruct(dx_asset_image_operations_checkerboard_pattern_fill* SELF)
{/*Intentionally empty.*/}

static void dx_asset_image_operations_checkerboard_pattern_fill_dispatch_construct(dx_asset_image_operations_checkerboard_pattern_fill_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_image_operations_checkerboard_pattern_fill_construct(dx_asset_image_operations_checkerboard_pattern_fill* SELF) {
  dx_rti_type* TYPE = dx_asset_image_operations_checkerboard_pattern_fill_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(SELF))) {
    return DX_FAILURE;
  }
  SELF->first_checker_color = NULL;
  SELF->second_checker_color = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_asset_image_operations_checkerboard_pattern_fill* dx_asset_image_operations_checkerboard_pattern_fill_create() {
  dx_rti_type* _type = dx_asset_image_operations_checkerboard_pattern_fill_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* self = DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(dx_object_alloc(sizeof(dx_asset_image_operations_checkerboard_pattern_fill)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_image_operations_checkerboard_pattern_fill_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

#define PROPERTY(TYPE, PROPERTY_TYPE, PROPERTY_NAME) \
  dx_result TYPE##_set_##PROPERTY_NAME(TYPE* SELF, PROPERTY_TYPE PROPERTY_NAME) { \
    SELF->PROPERTY_NAME = PROPERTY_NAME; \
    return DX_SUCCESS; \
  } \
  dx_result TYPE##_get_##PROPERTY_NAME##(PROPERTY_TYPE* RETURN, TYPE* SELF) { \
    *RETURN = SELF->PROPERTY_NAME; \
    return DX_SUCCESS; \
  }

PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, number_of_checkers_horizontal)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, number_of_checkers_vertical)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, checker_size_horizontal)
PROPERTY(dx_asset_image_operations_checkerboard_pattern_fill, dx_size, checker_size_vertical)

dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_asset_color_rgb_n8* first_checker_color) {
  DX_REFERENCE(first_checker_color);
  if (SELF->first_checker_color) {
    DX_UNREFERENCE(SELF->first_checker_color);
  }
  SELF->first_checker_color = first_checker_color;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_first_checker_color(DX_RGB_N8* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF) {
  *RETURN = SELF->first_checker_color->value;
  return DX_SUCCESS;
}

#undef PROPERTY

dx_result dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(dx_asset_image_operations_checkerboard_pattern_fill* SELF, dx_asset_color_rgb_n8* second_checker_color) {
  DX_REFERENCE(second_checker_color);
  if (SELF->second_checker_color) {
    DX_UNREFERENCE(SELF->second_checker_color);
  }
  SELF->second_checker_color = second_checker_color;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_checkerboard_pattern_fill_get_second_checker_color(DX_RGB_N8* RETURN, dx_asset_image_operations_checkerboard_pattern_fill* SELF) {
  *RETURN = SELF->second_checker_color->value;
  return DX_SUCCESS;
}
