#include "dx/val/material.h"

#include "dx/val/texture.h"

Core_defineObjectType("dx.val.material",
                      dx_val_material,
                      Core_Object);

static Core_Result add_to_backend(dx_val_material* SELF) {
  if (SELF->material_asset->ambient_texture_reference && SELF->material_asset->ambient_texture_reference->object) {
    Core_Assets_Texture* asset_ambient_texture = CORE_ASSETS_TEXTURE(SELF->material_asset->ambient_texture_reference->object);
    Core_Visuals_Texture* ambient_texture = NULL;
    if (Core_Visuals_Context_createTexture(&ambient_texture, SELF->context)) {
      return Core_Failure;
    }
    if (Core_Visuals_Texture_set_data(ambient_texture, asset_ambient_texture)) {
      CORE_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
      return Core_Failure;
    }
    SELF->ambient_texture = ambient_texture;
  }
  return Core_Success;
}

static void remove_from_backend(dx_val_material* SELF) {
  if (SELF->ambient_texture) {
    CORE_UNREFERENCE(SELF->ambient_texture);
    SELF->ambient_texture = NULL;
  }
}

static void dx_val_material_destruct(dx_val_material* SELF) {
  remove_from_backend(SELF);
  CORE_UNREFERENCE(SELF->material_asset);
  SELF->material_asset = NULL;
}

static void dx_val_material_constructDispatch(dx_val_material_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_material_construct(dx_val_material* SELF, Core_Visuals_Context* context, dx_assets_material* material_asset) {
  DX_CONSTRUCT_PREFIX(dx_val_material);
  if (!context || !material_asset) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->context = context;
  SELF->material_asset = material_asset;
  CORE_REFERENCE(material_asset);
  dx_rgb_n8_to_rgba_f32(&CORE_ASSETS_COLORRGBN8(material_asset->ambient_color->object)->value, 1.f, &SELF->ambient_color);
  SELF->ambient_texture = NULL;
  if (add_to_backend(SELF)) {
    CORE_UNREFERENCE(SELF->material_asset);
    SELF->material_asset = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_material_create(dx_val_material** RETURN, Core_Visuals_Context* context, dx_assets_material* material_asset) {
  DX_CREATE_PREFIX(dx_val_material);
  if (dx_val_material_construct(SELF, context, material_asset)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
