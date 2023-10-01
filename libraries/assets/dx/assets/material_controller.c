#include "dx/assets/material_controller.h"

//#include <math.h>
#include "dx/assets/color_rgb_n8.h"
#include "dx/assets/material.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.material_controller",
                      dx_assets_material_controller,
                      dx_object);

static void dx_assets_material_controller_destruct(dx_assets_material_controller* SELF)
{/*Intentionally empty.*/}

static void dx_assets_material_controller_dispatch_construct(dx_assets_material_controller_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_material_controller_construct(dx_assets_material_controller* SELF) {
  dx_rti_type *TYPE = dx_assets_material_controller_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.material_controllers.ambient_color",
                      dx_assets_material_controllers_ambient_color,
                      dx_assets_material_controller);

/// color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Capri".
static const DX_VEC4 start = { 0.f, 255.f / 255.f, 64.f / 255.f, 1.0f };
/// color called "Amber" (255, 192, 0) from "Capri"'s Split Complementary Palette.
/// @todo Do not hard-code this. If no color is specified, then resolution phase should default to "Amber".
static const DX_VEC4 end = { 255.f / 255.f, 192.f / 255.f, 0.f / 255.f };

static dx_result dx_assets_material_controllers_ambient_color_update(dx_assets_material_controllers_ambient_color* SELF, dx_assets_material* material, dx_f32 delta_seconds) {
   // fix the situation in which the sum might overflow
  bool a;
  if (dx_is_infinity_f32(&a, SELF->time + delta_seconds)) {
    return DX_FAILURE;
  }
  if (a) {
    // this is effectively equivalen to
    // time = (time + available) % DXF_32_GREATEST
    dx_f32 available = (DX_F32_GREATEST - SELF->time);
    SELF->time = delta_seconds - available;
  } else {
    SELF->time += delta_seconds;
  }
  static dx_f32 const shift = 0.5f;
  static dx_f32 const amplitude = 0.5f;
  static dx_f32 const period = 8;
  static dx_f32 const DX_TWO_PI_F32 = DX_PI_F32 * 2.f;
  dx_f32 b = 0.f;
  if (dx_sin_f32(&b, SELF->time / period * DX_TWO_PI_F32)) {
    return DX_FAILURE;
  }
  dx_f32 t = amplitude * b + shift;
  DX_RGB_N8 start = dx_colors_malachite;
  DX_RGB_N8 end = dx_colors_amber;
  DX_RGB_N8 current;
  dx_rgb_u8_lerp(&start, &end, t, &SELF->current);
  DX_ASSETS_COLOR_RGB_N8(material->ambient_color->object)->value = SELF->current;
  return DX_SUCCESS;
}

static void dx_assets_material_controllers_ambient_color_destruct(dx_assets_material_controllers_ambient_color* SELF)
{/*Intentionally empty.*/}

static void dx_assets_material_controllers_ambient_color_dispatch_construct(dx_assets_material_controllers_ambient_color_dispatch* SELF) {
  DX_ASSETS_MATERIAL_CONTROLLER_DISPATCH(SELF)->update = (dx_result(*)(dx_assets_material_controller*, dx_assets_material*, dx_f32)) & dx_assets_material_controllers_ambient_color_update;
}

dx_result dx_assets_material_controllers_ambient_color_construct(dx_assets_material_controllers_ambient_color* SELF) {
  dx_rti_type* TYPE = dx_assets_material_controllers_ambient_color_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_assets_material_controller_construct(DX_ASSETS_MATERIAL_CONTROLLER(SELF))) {
    return DX_FAILURE;
  }
  SELF->time = 0.f;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_material_controllers_ambient_color_create(dx_assets_material_controllers_ambient_color** RETURN) {
  DX_CREATE_PREFIX(dx_assets_material_controllers_ambient_color)
  if (dx_assets_material_controllers_ambient_color_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
