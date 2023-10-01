#include "dx/val/material.h"

#include "dx/val/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.val.material",
                      dx_val_material,
                      dx_object);

static dx_result add_to_backend(dx_val_material* SELF) {
  if (SELF->material_asset->ambient_texture_reference && SELF->material_asset->ambient_texture_reference->object) {
    dx_assets_texture* asset_ambient_texture = DX_ASSETS_TEXTURE(SELF->material_asset->ambient_texture_reference->object);
    dx_val_texture* ambient_texture = NULL;
    if (dx_val_context_create_texture(&ambient_texture, SELF->context)) {
      return DX_FAILURE;
    }
    if (dx_val_texture_set_data(ambient_texture, asset_ambient_texture)) {
      DX_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
      return DX_FAILURE;
    }
    SELF->ambient_texture = ambient_texture;
  }
  return DX_SUCCESS;
}

static void remove_from_backend(dx_val_material* SELF) {
  if (SELF->ambient_texture) {
    DX_UNREFERENCE(SELF->ambient_texture);
    SELF->ambient_texture = NULL;
  }
}

static void dx_val_material_destruct(dx_val_material* SELF) {
  remove_from_backend(SELF);
  DX_UNREFERENCE(SELF->material_asset);
  SELF->material_asset = NULL;
}

static void dx_val_material_dispatch_construct(dx_val_material_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_material_construct(dx_val_material* SELF, dx_val_context* context, dx_assets_material* material_asset) {
  if (!SELF || !context || !material_asset) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_val_material_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->context = context;
  SELF->material_asset = material_asset;
  DX_REFERENCE(material_asset);
  dx_rgb_n8_to_rgba_f32(&DX_ASSETS_COLOR_RGB_N8(material_asset->ambient_color->object)->value, 1.f, &SELF->ambient_color);
  SELF->ambient_texture = NULL;
  if (add_to_backend(SELF)) {
    DX_UNREFERENCE(SELF->material_asset);
    SELF->material_asset = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_material_create(dx_val_material** RETURN, dx_val_context* context, dx_assets_material* material_asset) {
  DX_CREATE_PREFIX(dx_val_material)
  if (dx_val_material_construct(SELF, context, material_asset)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
