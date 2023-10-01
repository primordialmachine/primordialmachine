#include "dx/assets/material.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.material",
                      dx_asset_material,
                      dx_object);

static void dx_asset_material_destruct(dx_asset_material* SELF) {
  if (SELF->ambient_color) {
    DX_UNREFERENCE(SELF->ambient_color);
    SELF->ambient_color = NULL;
  }
  if (SELF->controller) {
    DX_UNREFERENCE(SELF->controller);
    SELF->controller = NULL;
  }
  if (SELF->ambient_texture_reference) {
    DX_UNREFERENCE(SELF->ambient_texture_reference);
    SELF->ambient_texture_reference = NULL;
  }
  if (SELF->name) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
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

  SELF->ambient_color = NULL;
  
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

dx_result dx_asset_material_set_ambient_color(dx_asset_material* SELF, dx_assets_color_rgb_n8* ambient_color) {
  if (!SELF || !ambient_color) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!SELF->ambient_color) {
    dx_string* name = NULL;
    if (dx_string_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return DX_FAILURE;
    }
    if (dx_asset_reference_create(&SELF->ambient_color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(ambient_color);
  if (SELF->ambient_color->object) {
    DX_UNREFERENCE(SELF->ambient_color->object);
  }
  SELF->ambient_color->object = DX_OBJECT(ambient_color);
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
