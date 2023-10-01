#if !defined(DX_SAL_CONTEXT_H_INCLUDED)
#define DX_SAL_CONTEXT_H_INCLUDED

#include "dx/core.h"

typedef struct dx_val_command dx_val_command;
typedef struct dx_val_command_list dx_val_command_list;

typedef struct dx_val_buffer dx_val_buffer;
typedef struct dx_val_cbinding dx_val_cbinding;
typedef struct dx_val_program dx_val_program;
typedef struct dx_val_program_text dx_val_program_text;
typedef struct dx_val_texture dx_val_texture;
typedef struct dx_val_vbinding dx_val_vbinding;

DX_DECLARE_OBJECT_TYPE("dx.val.context",
                       dx_val_context,
                       dx_object);

static inline dx_val_context* DX_VAL_CONTEXT(void* p) {
  return (dx_val_context*)p;
}

struct dx_val_context {
  dx_object _parent;
};

static inline dx_val_context_dispatch* DX_VAL_CONTEXT_DISPATCH(void* p) {
  return (dx_val_context_dispatch*)p;
}

struct dx_val_context_dispatch {
  dx_object_dispatch _parent;
  dx_result (*bind_texture)(dx_val_context*, dx_size, dx_val_texture*);
  dx_result (*create_buffer)(dx_val_buffer**,dx_val_context*);
  dx_result (*create_cbinding)(dx_val_cbinding**,dx_val_context*);
  dx_result (*create_vbinding)(dx_val_vbinding**,dx_val_context*, dx_vertex_format, dx_val_buffer*);
  dx_result (*create_program)(dx_val_program**,dx_val_context*, dx_val_program_text*);
  dx_result (*create_texture)(dx_val_texture**,dx_val_context*);
  dx_result (*execute_commands)(dx_val_context*, dx_val_command_list*);
  dx_string* (*get_information)(dx_val_context*);
  dx_result (*enter_frame)(dx_val_context*);
  dx_result (*leave_frame)(dx_val_context*);
  dx_result (*get_canvas_size)(dx_val_context*, dx_i32*, dx_i32*);
  dx_result (*set_vsync_enabled)(dx_val_context*, dx_bool);
  dx_result (*get_vsync_enabled)(dx_bool*, dx_val_context*);
  dx_result (*get_dpi)(dx_val_context*, dx_i32*, dx_i32*);
};

dx_result dx_val_context_construct(dx_val_context* context);

/// @brief Get information on the backend in a human-readable form.
/// @param SELF A pointer to this context.
/// @return A pointer to a string with information on the backend in human-readable form on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned string object.
/// @default-failure
static inline dx_string* dx_val_context_get_information(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_information, SELF);
}

/// @brief Bind the specified texture to the specified texture unit.
/// @param SELF A pointer to this context.
/// @param unit The texture unit index.
/// @param texture A pointer to the texture or the null pointer.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_bind_texture(dx_val_context* SELF, dx_size unit, dx_val_texture* texture) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, bind_texture, SELF, unit, texture);
}

/// Create a buffer.
/// @param SELF A pointer to this context.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned buffer.
/// @default-failure
static inline dx_result dx_val_context_create_buffer(dx_val_buffer** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_buffer, RETURN, SELF);
}

/// Create a constant binding.
/// @param RETURN A pointer to a <code>dx_val_cbinding*</code> variable.
/// @param SELF A pointer to this context.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_cbinding object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_create_cbinding(dx_val_cbinding** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_cbinding, RETURN, SELF);
}

/// Create a variable binding.
/// @param RETURN A pointer to a <code>dx_val_vbinding*</code> variable.
/// @param SELF A pointer to this context.
/// @param vertex_format The vertex format of the variable binding.
/// @param buffer The buffer of the variable binding.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_vbinding object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_create_vbinding(dx_val_vbinding** RETURN, dx_val_context* SELF, dx_vertex_format vertex_format, dx_val_buffer* buffer) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_vbinding, RETURN, SELF, vertex_format, buffer);
}

/// Create a program.
/// @param RETURN A pointer to a <code>dx_val_program</code> variable.
/// @param SELF A pointer to this context.
/// @param program_text A program text of type #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_program object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_create_program(dx_val_program** RETURN, dx_val_context* SELF, dx_val_program_text* program_text) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_program, RETURN, SELF, program_text);
}

/// Create a texture.
/// @param RETURN A pointer to a <code>dx_val_texture</code> variable.
/// @param SELF A pointer to this context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_texture object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_create_texture(dx_val_texture** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_texture, RETURN, SELF);
}

/// Execute a command list.
/// @param SELF A pointer to this context.
/// @param commands A pointer to the command_list.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_execute_commands(dx_val_context* SELF, dx_val_command_list* commands) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, execute_commands, SELF, commands);
}

/// @brief Must be invoked before entering a frame.
/// @param SELF A pointer to this context.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_enter_frame(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, enter_frame, SELF);
}

/// @brief Must be invoked after leaving a frame.
/// @param SELF A pointer to this context.
/// @default-return
/// @default-failure
static inline dx_result dx_val_context_leave_frame(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, leave_frame, SELF);
}

/// Get the size of the canvas of this context.
/// @param SELF A pointer to this context.
/// @param width A pointer to a <code>dx_i32</code> variable.
/// @param height A pointer to a <code>dx_i32</code> variable.
/// @method-call
/// @success
/// <code>*width</code> was assigned the width of the canvas.
/// <code>*height</code> was assigned the height of the canvas.
static inline dx_result dx_val_context_get_canvas_size(dx_val_context* SELF, dx_i32* width, dx_i32* height) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_canvas_size, SELF, width, height);
}

/// @brief Set if VSYNC is enabled.
/// @param SELF A pointer to this context.
/// @param vsync_enabled #DX_TRUE enables VSYNC. #DX_FALSE disables VSYNC.
/// @method-call
static inline dx_result dx_val_context_set_vsync_enabled(dx_val_context* SELF, dx_bool vsync_enabled) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, set_vsync_enabled, SELF, vsync_enabled);
}

/// @brief Get if VSYNC is enabled.
/// @param RETURN A pointer to a <code>dx_bool</code> variable.
/// @param SELF a pointer to this context.
/// @success <code>*RETURN</code> was assigned #DX_TRUE if VSYNC is enabled and was assigned #DX_FALSE if VSYNC is disabled.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer
static inline dx_result dx_val_context_get_vsync_enabled(dx_bool* RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_vsync_enabled, RETURN, SELF);
}

/// @brief Get if VSYNC is enabled.
/// @param SELF a pointer to this context.
/// @param x A pointer to a <code>dx_n32</code> variable.
/// @param y A pointer to a <code>dx_n32</code> variable.
/// @method-call
/// @success
/// <code>*x</code> was assigned the DPI along the x-axis.
/// <code>*y</code> was assigned the DPI along the y-axis.
/// @remarks
/// DPI are used to scale the GUIs, in particular, to scale the point size of fonts.
/// This function typically returns the same value in both <code>*x</code> and <code>*y</code>.
static inline dx_result dx_val_context_get_dpi(dx_val_context* SELF, dx_i32* x, dx_i32* y) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_dpi, SELF, x, y);
}

#endif // DX_CONTEXT_H_INCLUDED
