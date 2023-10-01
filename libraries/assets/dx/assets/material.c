#include "dx/assets/material.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.material",
                      dx_asset_material,
                      dx_object);

static void dx_asset_material_destruct(dx_asset_material* self) {
  if (self->controller) {
    DX_UNREFERENCE(self->controller);
    self->controller = NULL;
  }
  if (self->ambient_texture_reference) {
    DX_UNREFERENCE(self->ambient_texture_reference);
    self->ambient_texture_reference = NULL;
  }
  if (self->name) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
  }
}

static void dx_asset_material_dispatch_construct(dx_asset_material_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_asset_material_construct(dx_asset_material* SELF, dx_string* name) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_asset_material_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  
  if (!name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->name = name;
  DX_REFERENCE(SELF->name);

  DX_RGB_N8 WHITE = { .r = 255, .g = 255, .b = 255 };
  if (dx_asset_color_rgb_n8_create(&SELF->ambient_color, &WHITE)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    return DX_FAILURE;
  }
  
  SELF->ambient_texture_reference = NULL;
  
  SELF->controller = NULL;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_material_create(dx_asset_material** RETURN, dx_string* name) {
  DX_CREATE_PREFIX(dx_asset_material)
  if (dx_asset_material_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_asset_material_set_ambient_color(dx_asset_material* SELF, dx_asset_color_rgb_n8* ambient_color) {
  if (!SELF || !ambient_color) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(ambient_color);
  DX_UNREFERENCE(SELF->ambient_color);
  SELF->ambient_color = ambient_color;
  return DX_SUCCESS;
}

dx_result dx_asset_material_set_ambient_texture(dx_asset_material* SELF, dx_asset_reference* ambient_texture_reference) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (ambient_texture_reference) {
    DX_REFERENCE(ambient_texture_reference);
  }
  if (SELF->ambient_texture_reference) {
    DX_UNREFERENCE(SELF->ambient_texture_reference);
  }
  SELF->ambient_texture_reference = ambient_texture_reference;
  return DX_SUCCESS;
}
