#if !defined(DX_SAL_CONTEXT_H_INCLUDED)
#define DX_SAL_CONTEXT_H_INCLUDED

#include "dx/core.h"

typedef struct dx_val_command dx_val_command;
typedef struct dx_val_command_list dx_val_command_list;

typedef struct dx_val_buffer dx_val_buffer;
typedef struct dx_val_cbinding dx_val_cbinding;
typedef struct dx_val_program dx_val_program;
typedef struct dx_val_program_text dx_val_program_text;
typedef struct Core_Visuals_Texture Core_Visuals_Texture;
typedef struct dx_val_vbinding dx_val_vbinding;

Core_declareObjectType("Core.Visuals.Context",
                       Core_Visuals_Context,
                       Core_Context);

static inline Core_Visuals_Context* CORE_VISUALS_CONTEXT(void* p) {
  return (Core_Visuals_Context*)p;
}

struct Core_Visuals_Context {
  Core_Context _parent;
};

static inline Core_Visuals_Context_Dispatch* CORE_VISUALS_CONTEXT_DISPATCH(void* p) {
  return (Core_Visuals_Context_Dispatch*)p;
}

struct Core_Visuals_Context_Dispatch {
  Core_Context_Dispatch _parent;
  Core_Result (*bindTexture)(Core_Visuals_Context*, Core_Size, Core_Visuals_Texture*);
  Core_Result (*createBuffer)(dx_val_buffer**, Core_Visuals_Context*);
  Core_Result (*createConstantBinding)(dx_val_cbinding**, Core_Visuals_Context*);
  Core_Result (*createVariableBinding)(dx_val_vbinding**, Core_Visuals_Context*, Core_VertexFormat, dx_val_buffer*);
  Core_Result (*createProgram)(dx_val_program**, Core_Visuals_Context*, dx_val_program_text*);
  Core_Result (*createTexture)(Core_Visuals_Texture**, Core_Visuals_Context*);
  Core_Result (*executeCommands)(Core_Visuals_Context*, dx_val_command_list*);
  Core_Result (*getInformation)(Core_String** RETURN, Core_Visuals_Context*);
  Core_Result (*enterFrame)(Core_Visuals_Context*);
  Core_Result (*leaveFrame)(Core_Visuals_Context*);
  Core_Result (*getCanvasSize)(Core_Visuals_Context*, Core_Integer32*, Core_Integer32*);
  Core_Result (*setVsyncEnabled)(Core_Visuals_Context*, Core_Boolean);
  Core_Result (*getVsyncEnabled)(Core_Boolean*, Core_Visuals_Context*);
  Core_Result (*getCanvasDpi)(Core_Visuals_Context*, Core_Integer32*, Core_Integer32*);
};

Core_Result Core_Visuals_Context_construct(Core_Visuals_Context* context);

/// @brief Get information on the backend in a human-readable form.
/// @return A pointer to a string with information on the backend in human-readable form on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned string object.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_getInformation(Core_String** RETURN, Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, getInformation, RETURN, SELF);
}

/// @brief Bind the specified texture to the specified texture unit.
/// @param unit The texture unit index.
/// @param texture A pointer to the texture or the null pointer.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_bindTexture(Core_Visuals_Context* SELF, Core_Size unit, Core_Visuals_Texture* texture) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, bindTexture, SELF, unit, texture);
}

/// Create a buffer.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success The caller has acquired a reference to the returned buffer.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_createBuffer(dx_val_buffer** RETURN, Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, createBuffer, RETURN, SELF);
}

/// Create a constant binding.
/// @param RETURN A pointer to a <code>dx_val_cbinding*</code> variable.
/// @return A pointer to the buffer on success. The null pointer on failure.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_cbinding object.
/// The caller has acquired a reference to the returned object.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_createConstantBinding(dx_val_cbinding** RETURN, Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, createConstantBinding, RETURN, SELF);
}

/// Create a variable binding.
/// @param RETURN A pointer to a <code>dx_val_vbinding*</code> variable.
/// @param vertex_format The vertex format of the variable binding.
/// @param buffer The buffer of the variable binding.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_vbinding object.
/// The caller has acquired a reference to the returned object.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_createVariableBinding(dx_val_vbinding** RETURN, Core_Visuals_Context* SELF, Core_VertexFormat vertex_format, dx_val_buffer* buffer) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, createVariableBinding, RETURN, SELF, vertex_format, buffer);
}

/// Create a program.
/// @param RETURN A pointer to a <code>dx_val_program</code> variable.
/// @param program_text A program text of type #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_program object.
/// The caller has acquired a reference to the returned object.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_createProgram(dx_val_program** RETURN, Core_Visuals_Context* SELF, dx_val_program_text* program_text) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, createProgram, RETURN, SELF, program_text);
}

/// Create a texture.
/// @param RETURN A pointer to a <code>Core_Visuals_Texture</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created Core_Visuals_Texture object.
/// The caller has acquired a reference to the returned object.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_createTexture(Core_Visuals_Texture** RETURN, Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, createTexture, RETURN, SELF);
}

/// Execute a command list.
/// @param commands A pointer to the command_list.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_executeCommands(Core_Visuals_Context* SELF, dx_val_command_list* commands) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, executeCommands, SELF, commands);
}

/// @brief Must be invoked before entering a frame.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_enterFrame(Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, enterFrame, SELF);
}

/// @brief Must be invoked after leaving a frame.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_leaveFrame(Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, leaveFrame, SELF);
}

/// Get the size of the canvas of this context.
/// @param SELF A pointer to this context.
/// @param horizontal A pointer to a <code>Core_Integer32</code> variable.
/// @param vertical A pointer to a <code>Core_Integer32</code> variable.
/// @method{Core_Visuals_Context}
/// @success
/// <code>*horizontal</code> was assigned the size, in pixels, of the canvas along the horizontal axis.
/// <code>*vertical</code> was assigned the size, in pixels, of the canvas long the vertical axis.
static inline Core_Result Core_Visuals_Context_getCanvasSize(Core_Visuals_Context* SELF, Core_Integer32* horizontal, Core_Integer32* vertical) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, getCanvasSize, SELF, horizontal, vertical);
}

/// @brief Set if VSYNC is enabled.
/// @param vsync_enabled #DX_TRUE enables VSYNC. #DX_FALSE disables VSYNC.
/// @method{Core_Visuals_Context}
static inline Core_Result Core_Visuals_Context_setVsyncEnabled(Core_Visuals_Context* SELF, Core_Boolean vsync_enabled) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, setVsyncEnabled, SELF, vsync_enabled);
}

/// @brief Get if VSYNC is enabled.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @success <code>*RETURN</code> was assigned #DX_TRUE if VSYNC is enabled and was assigned #DX_FALSE if VSYNC is disabled.
/// @method{Core_Visuals_Context}
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
static inline Core_Result Core_Visuals_Context_getVsyncEnabled(Core_Boolean* RETURN, Core_Visuals_Context* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, getVsyncEnabled, RETURN, SELF);
}

/// Get the dots per inch (DPI) of the canvas of this context.
/// @param horizontal A pointer to a <code>Core_Integer32</code> variable.
/// @param vertical A pointer to a <code>Core_Integer32</code> variable.
/// @method{Core_Visuals_Context}
/// @success
/// <code>*horizontal</code> was assigned the DPI along the horizontal axis.
/// <code>*vertical</code> was assigned the DPI along the vertical axis.
/// @remarks
/// DPI are used to scale the GUIs, in particular, to scale the point size of fonts.
/// This function typically returns the same value in both <code>*x</code> and <code>*y</code>.
static inline Core_Result Core_Visuals_Context_getCanvasDpi(Core_Visuals_Context* SELF, Core_Integer32* horizontal, Core_Integer32* vertical) {
  DX_OBJECT_VIRTUALCALL(Core_Visuals_Context, getCanvasDpi, SELF, horizontal, vertical);
}

#endif // DX_CONTEXT_H_INCLUDED
