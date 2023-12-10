/// @file dx/core/canvas_msgs.h
/// @brief Messages pertaining to canvans.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_CANVAS_MSGS_H_INCLUDED)
#define DX_CORE_CANVAS_MSGS_H_INCLUDED

#include "dx/core/msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An enumeration of the different kinds of canvas messages.
DX_DECLARE_ENUMERATION_TYPE("dx.canvas_msg_kind",
                            dx_canvas_msg_kind);

enum dx_canvas_msg_kind {
  /// @brief Kind of a a canvas activated message.
  dx_canvas_msg_kind_activated,
  /// @brief Kind of a canvas deactivated message.
  dx_canvas_msg_kind_deactivated,
  /// @brief Kind of a canvas size changed message.
  dx_canvas_msg_kind_size_changed,
  /// @brief Kind of a canvas DPI changed message.
  dx_canvas_msg_kind_dpi_changed,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.canvas_msg",
                       dx_canvas_msg,
                       dx_msg);

static inline dx_canvas_msg* DX_CANVAS_MSG(void* p) {
  return (dx_canvas_msg*)p;
}

struct dx_canvas_msg {
  Core_Message _parent;
  uint8_t kind;
};

static inline dx_canvas_msg_dispatch* DX_CANVAS_MSG_DISPATCH(void* p) {
  return (dx_canvas_msg_dispatch*)p;
}

struct dx_canvas_msg_dispatch {
  Core_Message_dispatch _parent;
};

/// @param kind The kind of this canvas message.
/// @constructor{dx_canvas_msg}
Core_Result dx_canvas_msg_construct(dx_canvas_msg* SELF, dx_canvas_msg_kind kind);

Core_Result dx_canvas_msg_create(dx_canvas_msg** RETURN, dx_canvas_msg_kind kind);

/// @brief Get the kind of this canvas message.
/// @param RETURN A pointer to a <code>dx_canvas_msg_kind</code> variable.
/// @success <code>*RETURN</code> was assigned the kind.
/// @method{dx_canvas_msg}
Core_Result dx_canvas_msg_get_kind(dx_canvas_msg_kind* RETURN, dx_canvas_msg* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Send when the size of a canvas changed.
DX_DECLARE_OBJECT_TYPE("dx.canvas_size_changed_msg",
                       dx_canvas_size_changed_msg,
                       dx_canvas_msg);

static inline dx_canvas_size_changed_msg* DX_CANVAS_SIZE_CHANGED_MSG(void* p) {
  return (dx_canvas_size_changed_msg*)p;
}

struct dx_canvas_size_changed_msg {
  dx_canvas_msg _parent;
  Core_Real32 width;
  Core_Real32 height;
};

static inline dx_canvas_size_changed_msg_dispatch* DX_CANVAS_SIZE_CHANGED_MSG_DISPATCH(void* p) {
  return (dx_canvas_size_changed_msg_dispatch*)p;
}

struct dx_canvas_size_changed_msg_dispatch {
  dx_canvas_msg_dispatch _parent;
};

Core_Result dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* SELF, Core_Real32 width, Core_Real32 height);

Core_Result dx_canvas_size_changed_msg_create(dx_canvas_size_changed_msg** RETURN, Core_Real32 width, Core_Real32 height);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Send when the DPI of a canvas changed.
DX_DECLARE_OBJECT_TYPE("dx.canvas_dpi_changed_msg",
                       dx_canvas_dpi_changed_msg,
                       dx_canvas_msg);

static inline dx_canvas_dpi_changed_msg* DX_CANVAS_DPI_CHANGED_MSG(void* p) {
  return (dx_canvas_dpi_changed_msg*)p;
}

struct dx_canvas_dpi_changed_msg {
  dx_canvas_msg _parent;
  Core_Real32 horizontal_dpi;
  Core_Real32 vertical_dpi;
};

static inline dx_canvas_dpi_changed_msg_dispatch* DX_CANVAS_DPI_CHANGED_MSG_DISPATCH(void* p) {
  return (dx_canvas_dpi_changed_msg_dispatch*)p;
}

struct dx_canvas_dpi_changed_msg_dispatch {
  dx_canvas_msg_dispatch _parent;
};

Core_Result dx_canvas_dpi_changed_msg_construct(dx_canvas_dpi_changed_msg* SELF, Core_Real32 horizontal_dpi, Core_Real32 vertical_dpi);

Core_Result dx_canvas_dpi_changed_msg_create(dx_canvas_dpi_changed_msg** RETURN, Core_Real32 horizontal_dpi, Core_Real32 vertical_dpi);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_CANVAS_MSGS_H_INCLUDED
