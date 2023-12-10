#include "dx/assets/material.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.material",
                      dx_assets_material,
                      Core_Object);

static void dx_assets_material_destruct(dx_assets_material* SELF) {
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

static void dx_assets_material_constructDispatch(dx_assets_material_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_assets_material_construct(dx_assets_material* SELF, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_assets_material);
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  DX_REFERENCE(SELF->name);

  SELF->ambient_color = NULL;
  
  SELF->ambient_texture_reference = NULL;
  
  SELF->controller = NULL;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_material_create(dx_assets_material** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(dx_assets_material);
  if (dx_assets_material_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_material_set_ambient_color(dx_assets_material* SELF, dx_assets_color_rgb_n8* ambient_color) {
  if (!SELF || !ambient_color) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!SELF->ambient_color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&SELF->ambient_color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  DX_REFERENCE(ambient_color);
  if (SELF->ambient_color->object) {
    DX_UNREFERENCE(SELF->ambient_color->object);
  }
  SELF->ambient_color->object = CORE_OBJECT(ambient_color);
  return Core_Success;
}

Core_Result dx_assets_material_set_ambient_texture(dx_assets_material* SELF, dx_asset_reference* ambient_texture_reference) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (ambient_texture_reference) {
    DX_REFERENCE(ambient_texture_reference);
  }
  if (SELF->ambient_texture_reference) {
    DX_UNREFERENCE(SELF->ambient_texture_reference);
  }
  SELF->ambient_texture_reference = ambient_texture_reference;
  return Core_Success;
}
