#include "dx/fps_counter.h"

// GetTickCount64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DEFINE_OBJECT_TYPE("dx.fps_counter",
                      dx_fps_counter,
                      Core_Object);

static void dx_fps_counter_destruct(dx_fps_counter* self)
{/*Intentionally empty.*/}

static void dx_fps_counter_constructDispatch(dx_fps_counter_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_fps_counter_construct(dx_fps_counter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_fps_counter);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.default_fps_counter",
                      dx_default_fps_counter,
                      dx_fps_counter);

static Core_Result dx_default_fps_counter_on_enter_frame(dx_default_fps_counter* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->started = true;
  SELF->start = GetTickCount64();
  if (Core_getNow(&SELF->start)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result dx_default_fps_counter_on_leave_frame(dx_default_fps_counter* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!SELF->started) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  Core_Natural64 now;
  if (Core_getNow(&now)) {
    return Core_Failure;
  }
  Core_Natural64 delta = now - SELF->start;
  SELF->started = false;
  SELF->durations[SELF->write] = delta;
  SELF->write = (SELF->write + 1) % 256;
  if (SELF->size < 256) {
    SELF->size++;
  }
  return Core_Success;
}

static Core_Result dx_default_fps_counter_get_fps(Core_Real64* RETURN, dx_default_fps_counter* SELF) {
  if (!SELF->size) {
    *RETURN = 1000.f;
    return Core_Success;
  } else {
    Core_Real64 average_duration = 0.;
    for (Core_Size i = 0, n = SELF->size; i < n; ++i) {
      average_duration += (Core_Real64)SELF->durations[i];
    }
    average_duration /= (Core_Real64)SELF->size;
    // if average duration is zero,
    // then theoretically we have a
    // infinite number of frames per
    // second :D
    if (average_duration == 0.f) {
      *RETURN = INFINITE;
    } else {
      *RETURN = 1000. / average_duration;
    }
    return Core_Success;
  }
}

static void dx_default_fps_counter_destruct(dx_default_fps_counter* self)
{/*Intentionally empty.*/}

static void dx_default_fps_counter_constructDispatch(dx_default_fps_counter_dispatch* self) {
  DX_FPS_COUNTER_DISPATCH(self)->get_fps = (Core_Result(*)(Core_Real64*, dx_fps_counter*))dx_default_fps_counter_get_fps;
  DX_FPS_COUNTER_DISPATCH(self)->on_enter_frame = (Core_Result(*)(dx_fps_counter*))dx_default_fps_counter_on_enter_frame;
  DX_FPS_COUNTER_DISPATCH(self)->on_leave_frame = (Core_Result(*)(dx_fps_counter*))dx_default_fps_counter_on_leave_frame;
}

Core_Result dx_default_fps_counter_construct(dx_default_fps_counter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_default_fps_counter);
  if (dx_fps_counter_construct(DX_FPS_COUNTER(SELF))) {
    return Core_Failure;
  }
  SELF->started = 0;
  SELF->start = 0;
  SELF->write = 0;
  SELF->size = 0;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_default_fps_counter_create(dx_default_fps_counter** RETURN) {
  DX_CREATE_PREFIX(dx_default_fps_counter)
  if (dx_default_fps_counter_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
