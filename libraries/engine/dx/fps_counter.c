#include "dx/fps_counter.h"

// GetTickCount64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

DX_DEFINE_OBJECT_TYPE("dx.fps_counter",
                      dx_fps_counter,
                      dx_object)

static void dx_fps_counter_destruct(dx_fps_counter* self)
{/*Intentionally empty.*/}

static void dx_fps_counter_dispatch_construct(dx_fps_counter_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_fps_counter_construct(dx_fps_counter* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_fps_counter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.default_fps_counter",
                      dx_default_fps_counter,
                      dx_fps_counter);

static dx_result dx_default_fps_counter_on_enter_frame(dx_default_fps_counter* SELF) {
  if (!SELF) {
    return DX_FAILURE;
  }
  SELF->started = true;
  SELF->start = GetTickCount64();
  return DX_SUCCESS;
}

static dx_result dx_default_fps_counter_on_leave_frame(dx_default_fps_counter* SELF) {
  if (!SELF) {
    return DX_FAILURE;
  }
  if (!SELF->started) {
    return DX_FAILURE;
  }
  dx_n64 delta = GetTickCount64() - SELF->start;
  SELF->started = false;
  SELF->durations[SELF->write] = delta;
  SELF->write = (SELF->write + 1) % 256;
  if (SELF->size < 256) {
    SELF->size++;
  }
  return DX_SUCCESS;
}

static dx_result dx_default_fps_counter_get_fps(dx_f64* RETURN, dx_default_fps_counter* SELF) {
  if (!SELF->size) {
    *RETURN = 1000.f;
    return DX_SUCCESS;
  } else {
    dx_f64 average_duration = 0.;
    for (dx_size i = 0, n = SELF->size; i < n; ++i) {
      average_duration += (dx_f64)SELF->durations[i];
    }
    average_duration /= (dx_f64)SELF->size;
    // if average duration is zero,
    // then theoretically we have a
    // infinite number of frames per
    // second :D
    if (average_duration == 0.f) {
      *RETURN = INFINITE;
    } else {
      *RETURN = 1000. / average_duration;
    }
    return DX_SUCCESS;
  }
}

static void dx_default_fps_counter_destruct(dx_default_fps_counter* self)
{/*Intentionally empty.*/}

static void dx_default_fps_counter_dispatch_construct(dx_default_fps_counter_dispatch* self) {
  DX_FPS_COUNTER_DISPATCH(self)->get_fps = (dx_result(*)(dx_f64*, dx_fps_counter*))dx_default_fps_counter_get_fps;
  DX_FPS_COUNTER_DISPATCH(self)->on_enter_frame = (dx_result(*)(dx_fps_counter*))dx_default_fps_counter_on_enter_frame;
  DX_FPS_COUNTER_DISPATCH(self)->on_leave_frame = (dx_result(*)(dx_fps_counter*))dx_default_fps_counter_on_leave_frame;
}

dx_result dx_default_fps_counter_construct(dx_default_fps_counter* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_default_fps_counter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_fps_counter_construct(DX_FPS_COUNTER(SELF))) {
    return DX_FAILURE;
  }
  SELF->started = 0;
  SELF->start = 0;
  SELF->write = 0;
  SELF->size = 0;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_default_fps_counter_create(dx_default_fps_counter** RETURN) {
  DX_CREATE_PREFIX(dx_default_fps_counter)
  if (dx_default_fps_counter_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
