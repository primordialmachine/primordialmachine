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

Core_declareObjectType("dx.val.context",
                       dx_val_context,
                       dx_context);

static inline dx_val_context* DX_VAL_CONTEXT(void* p) {
  return (dx_val_context*)p;
}

struct dx_val_context {
  dx_context _parent;
};

static inline dx_val_context_Dispatch* DX_VAL_CONTEXT_DISPATCH(void* p) {
  return (dx_val_context_Dispatch*)p;
}

struct dx_val_context_Dispatch {
  dx_context_Dispatch _parent;
  Core_Result (*bind_texture)(dx_val_context*, Core_Size, dx_val_texture*);
  Core_Result (*create_buffer)(dx_val_buffer**,dx_val_context*);
  Core_Result (*create_cbinding)(dx_val_cbinding**,dx_val_context*);
  Core_Result (*create_vbinding)(dx_val_vbinding**,dx_val_context*, Core_VertexFormat, dx_val_buffer*);
  Core_Result (*create_program)(dx_val_program**,dx_val_context*, dx_val_program_text*);
  Core_Result (*create_texture)(dx_val_texture**,dx_val_context*);
  Core_Result (*execute_commands)(dx_val_context*, dx_val_command_list*);
  Core_String* (*get_information)(dx_val_context*);
  Core_Result (*enter_frame)(dx_val_context*);
  Core_Result (*leave_frame)(dx_val_context*);
  Core_Result (*get_canvas_size)(dx_val_context*, Core_Integer32*, Core_Integer32*);
  Core_Result (*set_vsync_enabled)(dx_val_context*, Core_Boolean);
  Core_Result (*get_vsync_enabled)(Core_Boolean*, dx_val_context*);
  Core_Result (*get_canvas_dpi)(dx_val_context*, Core_Integer32*, Core_Integer32*);
};

Core_Result dx_val_context_construct(dx_val_context* context);

/// @brief Get information on the backend in a human-readable form.
/// @return A pointer to a string with information on the backend in human-readable form on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned string object.
/// @method{dx_val_context}
static inline Core_String* dx_val_context_get_information(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_information, SELF);
}

/// @brief Bind the specified texture to the specified texture unit.
/// @param unit The texture unit index.
/// @param texture A pointer to the texture or the null pointer.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_bind_texture(dx_val_context* SELF, Core_Size unit, dx_val_texture* texture) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, bind_texture, SELF, unit, texture);
}

/// Create a buffer.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned buffer.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_create_buffer(dx_val_buffer** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_buffer, RETURN, SELF);
}

/// Create a constant binding.
/// @param RETURN A pointer to a <code>dx_val_cbinding*</code> variable.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_cbinding object.
/// The caller has acquired a reference to the returned object.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_create_cbinding(dx_val_cbinding** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_cbinding, RETURN, SELF);
}

/// Create a variable binding.
/// @param RETURN A pointer to a <code>dx_val_vbinding*</code> variable.
/// @param vertex_format The vertex format of the variable binding.
/// @param buffer The buffer of the variable binding.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_vbinding object.
/// The caller has acquired a reference to the returned object.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_create_vbinding(dx_val_vbinding** RETURN, dx_val_context* SELF, Core_VertexFormat vertex_format, dx_val_buffer* buffer) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_vbinding, RETURN, SELF, vertex_format, buffer);
}

/// Create a program.
/// @param RETURN A pointer to a <code>dx_val_program</code> variable.
/// @param program_text A program text of type #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_program object.
/// The caller has acquired a reference to the returned object.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_create_program(dx_val_program** RETURN, dx_val_context* SELF, dx_val_program_text* program_text) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_program, RETURN, SELF, program_text);
}

/// Create a texture.
/// @param RETURN A pointer to a <code>dx_val_texture</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_texture object.
/// The caller has acquired a reference to the returned object.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_create_texture(dx_val_texture** RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, create_texture, RETURN, SELF);
}

/// Execute a command list.
/// @param commands A pointer to the command_list.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_execute_commands(dx_val_context* SELF, dx_val_command_list* commands) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, execute_commands, SELF, commands);
}

/// @brief Must be invoked before entering a frame.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_enter_frame(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, enter_frame, SELF);
}

/// @brief Must be invoked after leaving a frame.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_leave_frame(dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, leave_frame, SELF);
}

/// Get the size of the canvas of this context.
/// @param SELF A pointer to this context.
/// @param horizontal A pointer to a <code>Core_Integer32</code> variable.
/// @param vertical A pointer to a <code>Core_Integer32</code> variable.
/// @method{dx_val_context}
/// @success
/// <code>*horizontal</code> was assigned the size, in pixels, of the canvas along the horizontal axis.
/// <code>*vertical</code> was assigned the size, in pixels, of the canvas long the vertical axis.
static inline Core_Result dx_val_context_get_canvas_size(dx_val_context* SELF, Core_Integer32* horizontal, Core_Integer32* vertical) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_canvas_size, SELF, horizontal, vertical);
}

/// @brief Set if VSYNC is enabled.
/// @param vsync_enabled #DX_TRUE enables VSYNC. #DX_FALSE disables VSYNC.
/// @method{dx_val_context}
static inline Core_Result dx_val_context_set_vsync_enabled(dx_val_context* SELF, Core_Boolean vsync_enabled) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, set_vsync_enabled, SELF, vsync_enabled);
}

/// @brief Get if VSYNC is enabled.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @success <code>*RETURN</code> was assigned #DX_TRUE if VSYNC is enabled and was assigned #DX_FALSE if VSYNC is disabled.
/// @method{dx_val_context}
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
static inline Core_Result dx_val_context_get_vsync_enabled(Core_Boolean* RETURN, dx_val_context* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_vsync_enabled, RETURN, SELF);
}

/// Get the dots per inch (DPI) of the canvas of this context.
/// @param horizontal A pointer to a <code>Core_Integer32</code> variable.
/// @param vertical A pointer to a <code>Core_Integer32</code> variable.
/// @method{dx_val_context}
/// @success
/// <code>*horizontal</code> was assigned the DPI along the horizontal axis.
/// <code>*vertical</code> was assigned the DPI along the vertical axis.
/// @remarks
/// DPI are used to scale the GUIs, in particular, to scale the point size of fonts.
/// This function typically returns the same value in both <code>*x</code> and <code>*y</code>.
static inline Core_Result dx_val_context_get_canvas_dpi(dx_val_context* SELF, Core_Integer32* horizontal, Core_Integer32* vertical) {
  DX_OBJECT_VIRTUALCALL(dx_val_context, get_canvas_dpi, SELF, horizontal, vertical);
}

#endif // DX_CONTEXT_H_INCLUDED
