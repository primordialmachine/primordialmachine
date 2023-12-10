#if !defined(DX_FPS_COUNTER_H_INCLUDED)
#define DX_FPS_COUNTER_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.fps_counter",
                       dx_fps_counter,
                       Core_Object);

static inline dx_fps_counter* DX_FPS_COUNTER(void* p) {
  return (dx_fps_counter*)p;
}

struct dx_fps_counter {
  Core_Object _parent;
};

static inline dx_fps_counter_dispatch* DX_FPS_COUNTER_DISPATCH(void* p) {
  return (dx_fps_counter_dispatch*)p;
}

struct dx_fps_counter_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*on_enter_frame)(dx_fps_counter*);
  Core_Result(*on_leave_frame)(dx_fps_counter*);
  Core_Result(*get_fps)(Core_Real64*, dx_fps_counter*);
};

Core_Result dx_fps_counter_construct(dx_fps_counter* SELF);

static inline Core_Result dx_fps_counter_on_enter_frame(dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, on_enter_frame, SELF);
}

static inline Core_Result dx_fps_counter_on_leave_frame(dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, on_leave_frame, SELF);
}

static inline Core_Result dx_fps_counter_get_fps(Core_Real64* RETURN, dx_fps_counter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_fps_counter, get_fps, RETURN, SELF);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.default_fps_counter",
                       dx_default_fps_counter,
                       Core_Object);

static inline dx_default_fps_counter* DX_DEFAULT_FPS_COUNTER(void* p) {
  return (dx_default_fps_counter*)p;
}

struct dx_default_fps_counter {
  dx_fps_counter _parent;

  /// @brief List of frame durations.
  Core_Natural64 durations[256];
  /// @brief The position to write an incoming duration to.
  Core_Size write;
  /// @brief The number of durations.
  Core_Size size;

  /// @brief Are we inside a frame?
  bool started;
  /// @brief Point in time at which the frame started if we are inside a frame.
  Core_Natural64 start;
};

static inline dx_default_fps_counter_dispatch* DX_DEFAULT_FPS_COUNTER_DISPATCH(void* p) {
  return (dx_default_fps_counter_dispatch*)p;
}

struct dx_default_fps_counter_dispatch {
  dx_fps_counter_dispatch _parent;
};

Core_Result dx_default_fps_counter_construct(dx_default_fps_counter* SELF);

Core_Result dx_default_fps_counter_create(dx_default_fps_counter** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_FPS_COUNTER_H_INCLUDED
