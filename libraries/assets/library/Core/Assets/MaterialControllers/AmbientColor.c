// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/MaterialControllers/AmbientColor.h"

#include "Core/Assets/ColorRgbN8.h"
#include "Core/Assets/Material.h"

Core_defineObjectType("Core.Assets.MaterialControllers.AmbientColor",
                      Core_Assets_MaterialControllers_AmbientColor,
                      Core_Assets_MaterialController);

/// color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Capri".
static const DX_VEC4 start = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f };
/// color called "Amber" (255, 192, 0) from "Capri"'s Split Complementary Palette.
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Amber".
static const DX_VEC4 end = { 255.f / 255.f, 192.f / 255.f, 0.f / 255.f };

static Core_Result Core_Assets_MaterialControllers_AmbientColor_update(Core_Assets_MaterialControllers_AmbientColor* SELF, Core_Assets_Material* material, Core_Real32 delta_seconds) {
   // fix the situation in which the sum might overflow
  Core_Boolean a;
  if (Core_isInfinityR32(&a, SELF->time + delta_seconds)) {
    return Core_Failure;
  }
  if (a) {
    // this is effectively equivalen to
    // time = (time + available) % DXF_32_GREATEST
    Core_Real32 available = (Core_Real32_Greatest - SELF->time);
    SELF->time = delta_seconds - available;
  } else {
    SELF->time += delta_seconds;
  }
  static Core_Real32 const shift = 0.5f;
  static Core_Real32 const amplitude = 0.5f;
  static Core_Real32 const period = 8;
  static Core_Real32 const DX_TWO_PI_F32 = DX_PI_F32 * 2.f;
  Core_Real32 b = 0.f;
  if (Core_sinR32(&b, SELF->time / period * DX_TWO_PI_F32)) {
    return Core_Failure;
  }
  Core_Real32 t = amplitude * b + shift;
  Core_InlineRgbN8 start = dx_colors_malachite;
  Core_InlineRgbN8 end = dx_colors_amber;
  Core_InlineRgbN8 current;
  dx_rgb_u8_lerp(&start, &end, t, &SELF->current);
  CORE_ASSETS_COLORRGBN8(material->ambientColor->object)->value = SELF->current;
  return Core_Success;
}

static void Core_Assets_MaterialControllers_AmbientColor_destruct(Core_Assets_MaterialControllers_AmbientColor* SELF) {/*Intentionally empty.*/
}

static void Core_Assets_MaterialControllers_AmbientColor_constructDispatch(Core_Assets_MaterialControllers_AmbientColor_Dispatch* SELF) {
  CORE_ASSETS_MATERIAL_CONTROLLER_DISPATCH(SELF)->update = (Core_Result(*)(Core_Assets_MaterialController*, Core_Assets_Material*, Core_Real32)) & Core_Assets_MaterialControllers_AmbientColor_update;
}

Core_Result Core_Assets_MaterialControllers_AmbientColor_construct(Core_Assets_MaterialControllers_AmbientColor* SELF) {
  Core_BeginConstructor(Core_Assets_MaterialControllers_AmbientColor);
  if (Core_Assets_MaterialController_construct(CORE_ASSETS_MATERIAL_CONTROLLER(SELF))) {
    return Core_Failure;
  }
  SELF->time = 0.f;
  Core_EndConstructor(Core_Assets_MaterialControllers_AmbientColor);
}

Core_Result Core_Assets_MaterialControllers_AmbientColor_create(Core_Assets_MaterialControllers_AmbientColor** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_MaterialControllers_AmbientColor);
  if (Core_Assets_MaterialControllers_AmbientColor_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}