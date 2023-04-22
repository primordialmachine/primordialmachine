#if !defined(DX_FPS_COUNTER_H_INCLUDED)
#define DX_FPS_COUNTER_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.fps_counter",
                       dx_fps_counter,
                       dx_object);

static inline dx_fps_counter* DX_FPS_COUNTER(void* p) {
  return (dx_fps_counter*)p;
}

struct dx_fps_counter {
  dx_object _parent;
};

static inline dx_fps_counter_dispatch* DX_FPS_COUNTER_DISPATCH(void* p) {
  return (dx_fps_counter_dispatch*)p;
}

struct dx_fps_counter_dispatch {
  dx_object_dispatch _parent;
  dx_result(*on_enter_frame)(dx_fps_counter*);
  dx_result(*on_leave_frame)(dx_fps_counter*);
  dx_result(*get_fps)(dx_f64*, dx_fps_counter*);
};

dx_result dx_fps_counter_construct(dx_fps_counter* SELF);

static inline dx_result dx_fps_counter_on_enter_frame(dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, on_enter_frame, SELF);
}

static inline dx_result dx_fps_counter_on_leave_frame(dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, on_leave_frame, SELF);
}

static inline dx_result dx_fps_counter_get_fps(dx_f64* RETURN, dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, get_fps, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.default_fps_counter",
                       dx_default_fps_counter,
                       dx_object);

static inline dx_default_fps_counter* DX_DEFAULT_FPS_COUNTER(void* p) {
  return (dx_default_fps_counter*)p;
}

struct dx_default_fps_counter {
  dx_fps_counter _parent;

  /// @brief List of frame durations.
  dx_n64 durations[256];
  /// @brief The position to write an incoming duration to.
  dx_size write;
  /// @brief The number of durations.
  dx_size size;

  /// @brief Are we inside a frame?
  bool started;
  /// @brief Point in time at which the frame started if we are inside a frame.
  dx_n64 start;
};

static inline dx_default_fps_counter_dispatch* DX_DEFAULT_FPS_COUNTER_DISPATCH(void* p) {
  return (dx_default_fps_counter_dispatch*)p;
}

struct dx_default_fps_counter_dispatch {
  dx_fps_counter_dispatch _parent;
};

dx_result dx_default_fps_counter_construct(dx_default_fps_counter* SELF);

dx_result dx_default_fps_counter_create(dx_default_fps_counter** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_FPS_COUNTER_H_INCLUDED
