#include "dx/assets/material_controller.h"

#include "Core/Assets/ColorRgbN8.h"
#include "dx/assets/material.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.assets.material_controller",
                      dx_assets_material_controller,
                      Core_Object);

static void dx_assets_material_controller_destruct(dx_assets_material_controller* SELF)
{/*Intentionally empty.*/}

static void dx_assets_material_controller_constructDispatch(dx_assets_material_controller_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_material_controller_construct(dx_assets_material_controller* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_material_controller);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.assets.material_controllers.ambient_color",
                      dx_assets_material_controllers_ambient_color,
                      dx_assets_material_controller);

/// color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Capri".
static const DX_VEC4 start = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f };
/// color called "Amber" (255, 192, 0) from "Capri"'s Split Complementary Palette.
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Amber".
static const DX_VEC4 end = { 255.f / 255.f, 192.f / 255.f, 0.f / 255.f };

static Core_Result dx_assets_material_controllers_ambient_color_update(dx_assets_material_controllers_ambient_color* SELF, dx_assets_material* material, Core_Real32 delta_seconds) {
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
  CORE_ASSETS_COLORRGBN8(material->ambient_color->object)->value = SELF->current;
  return Core_Success;
}

static void dx_assets_material_controllers_ambient_color_destruct(dx_assets_material_controllers_ambient_color* SELF)
{/*Intentionally empty.*/}

static void dx_assets_material_controllers_ambient_color_constructDispatch(dx_assets_material_controllers_ambient_color_Dispatch* SELF) {
  DX_ASSETS_MATERIAL_CONTROLLER_DISPATCH(SELF)->update = (Core_Result(*)(dx_assets_material_controller*, dx_assets_material*, Core_Real32)) & dx_assets_material_controllers_ambient_color_update;
}

Core_Result dx_assets_material_controllers_ambient_color_construct(dx_assets_material_controllers_ambient_color* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_material_controllers_ambient_color);
  if (dx_assets_material_controller_construct(DX_ASSETS_MATERIAL_CONTROLLER(SELF))) {
    return Core_Failure;
  }
  SELF->time = 0.f;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_material_controllers_ambient_color_create(dx_assets_material_controllers_ambient_color** RETURN) {
  DX_CREATE_PREFIX(dx_assets_material_controllers_ambient_color);
  if (dx_assets_material_controllers_ambient_color_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
