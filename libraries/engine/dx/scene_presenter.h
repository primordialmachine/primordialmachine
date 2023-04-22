#if !defined(DX_SCENE_PRESENTER_H_INCLUDED)
#define DX_SCENE_PRESENTER_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"

/// The base of all "scene presenters".
///
/// "scenes (assets)" are independent units of descriptions (audials & visuals & haptical) input and output.
/// "scene presenters" present these "scene assets".
/// "scenes presenters" are created, destroyed, and operated by "scene execution environment".
/// "scene presenters" have a contract with the "scene execution environment".
/// 1) the "scene presenter" is created by the "scene execution environment"
/// 2) dx_scene_presenter_startup is called for the "scene presenter" by the "scene execution environment".
/// If the call succeeds, go to 3) or 4).
/// Otherwise got to 2) or 5)
/// 3) dx_scene_presenter_render is called for the "scene presenter" by the "scene execution environment".
/// If the call succeeds, go to 3) or 4).
/// Otherwise go to 4)
/// 4) dx_scene_presenter_shutdown is called for the "scene presenter" by the "scene execution environment".
/// Go to 5.
/// 5) the "scene presenter" is destroyed by the "scene execution environment".
DX_DECLARE_OBJECT_TYPE("dx.scene_presenter",
                       dx_scene_presenter,
                       dx_object)

static inline dx_scene_presenter* DX_SCENE_PRESENTER(void *p) {
  return (dx_scene_presenter*)p;
}

struct dx_scene_presenter {
  dx_object _parent;
  dx_val_context* context;
};

static inline dx_scene_presenter_dispatch* DX_SCENE_PRESENTER_DISPATCH(void* p) {
  return (dx_scene_presenter_dispatch*)p;
}

struct dx_scene_presenter_dispatch {
  dx_object_dispatch _parent;
  dx_result(*startup)(dx_scene_presenter* SELF, dx_val_context* context);
  dx_result(*render)(dx_scene_presenter* SELF, dx_val_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);
  dx_result(*shutdown)(dx_scene_presenter* SELF, dx_val_context* context);
};

/// @brief Construct this scene.
/// @param SELF A pointer to this scene presenter.
/// @default-return
dx_result dx_scene_presenter_construct(dx_scene_presenter* SELF);

/// @brief Callback. Invoked by the execution environment.
/// @param SELF A pointer to this scene presenter.
/// @default-return
static inline dx_result dx_scene_presenter_startup(dx_scene_presenter* SELF, dx_val_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_scene_presenter, startup, SELF, context);
}

/// @brief Callback. Invoked by the execution environment.
/// @param self A pointer to this scene presenter.
/// @param context A pointer to the VAL context.
/// @param delta_seconds The delta, in seconds, the visualization time has advanced.
/// @param canvas_width, canvas_height The width and height of the canvas.
/// @default-return
static inline dx_result dx_scene_presenter_render(dx_scene_presenter* SELF, dx_val_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  DX_OBJECT_VIRTUALCALL(dx_scene_presenter, render, SELF, context, delta_seconds, canvas_width, canvas_height);
}

/// @brief Callback. Invoked by the execution environment.
/// @param self A pointer to this scene.
/// @default-return
static inline dx_result dx_scene_presenter_shutdown(dx_scene_presenter* SELF, dx_val_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_scene_presenter, shutdown, SELF, context);
}

#endif // DX_SCENE_H_INCLUDED
