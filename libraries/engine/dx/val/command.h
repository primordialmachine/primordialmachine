#if !defined(DX_VAL_COMMAND_H_INCLUDED)
#define DX_VAL_COMMAND_H_INCLUDED

#include "dx/val/vbinding.h"
#include "dx/val/cbinding.h"
#include "dx/val/material.h"
#include "dx/val/program.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Kind of a command that does nothing.
#define DX_VAL_COMMAND_KIND_IDLE (0)

/// @brief Kind of a command that clears the color buffer.
#define DX_VAL_COMMAND_KIND_CLEAR_COLOR (1)

/// @brief Kind of a command that clears the depth buffer.
#define DX_VAL_COMMAND_KIND_CLEAR_DEPTH (2)

/// @brief Kind of a command that draws triangles.
#define DX_VAL_COMMAND_KIND_DRAW (3)

/// @brief Kind of a command that sets the viewport.
#define DX_VAL_COMMAND_KIND_VIEWPORT (4)

/// @brief Kind of a command that sets the pipeline state.
#define DX_VAL_COMMAND_KIND_PIPELINE_STATE (5)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.val.command",
                       dx_val_command,
                       dx_object);

static inline dx_val_command* DX_VAL_COMMAND(void* p) {
  return (dx_val_command*)p;
}

struct dx_val_command {
  dx_object _parent;
  
  uint8_t kind;
  
  union {
    
    struct {
      struct {
        dx_f32 l, b, w, h;
      } rectangle;
      DX_RGBA_F32 color;
    } clear_color_command;
    
    struct {
      struct {
        dx_f32 l, b, w, h;
      } rectangle;
      dx_f32 depth;
    } clear_depth_command;
    
    struct {
      dx_val_cbinding* cbinding;
      dx_val_texture* texture;
      dx_val_program* program;
      dx_val_vbinding* vbinding;
      dx_n32 start;
      dx_n32 length;
    } draw_command;

    struct {
      
      /// @brief The left side of the viewport.
      /// @default @a 0.
      dx_f32 l;
      
      /// @brief The bottom side of the viewport.
      /// @default @a 0.
      dx_f32 b;
      
      /// @brief The width of the viewport.
      /// @default The width of the canvas.
      dx_f32 w;
      
      /// @brief The height of the viewport.
      /// @default The height of the canvas.
      dx_f32 h;
    
    } viewport_command;

    struct {
      
      /// @brief The cull mode.
      /// @default DX_CULL_MODE_BACK.
      DX_CULL_MODE cull_mode;
      
      /// @brief The depth test function.
      /// @default DX_DEPTH_TEST_FUNCTION_LESS_THAN.
      DX_DEPTH_TEST_FUNCTION depth_test_function;
      
      /// @brief Is depth write enable.
      /// @default DX_TRUE.
      dx_bool depth_write_enabled;

    } pipeline_state;
  };
};

static inline dx_val_command_dispatch* DX_VAL_COMMAND_DISPATCH(void* p) {
  return (dx_val_command_dispatch*)p;
}

struct dx_val_command_dispatch {
  dx_object_dispatch _parent;
};


dx_result dx_val_command_construct_clear_color(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_RGBA_F32 const* color);

/// Create a clear color (buffer) command.
/// @param RETURN A pointer to a <code>dx_val_command</code> variable.
/// @param color The value to clear the color buffer with.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_command object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
dx_result dx_val_command_create_clear_color(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_RGBA_F32 const* color);


dx_result dx_val_command_construct_clear_depth(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth);

/// Create a clear color (buffer) command.
/// @param RETURN A pointer to a <code>dx_val_command</code> variable.
/// @param depth The value to clear the color buffer with.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_command object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
dx_result dx_val_command_create_clear_depth(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth);


dx_result dx_val_command_construct_draw(dx_val_command* SELF, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, dx_n32 start, dx_n32 length);

/// Create a draw command.
/// @param RETURN A pointer to a <code>dx_val_command</code> variable.
/// @param vbinding The variable binding.
/// @param texture The texture.
/// @param cbinding The constant binding.
/// @param program The program.
/// @param start The index at which the vertex sequence starts at.
/// @param length The length of the vertex sequence.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_vbinding object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
dx_result dx_val_command_create_draw(dx_val_command** RETURN, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, dx_n32 start, dx_n32 length);


dx_result dx_val_command_construct_viewport(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h);

/// Create a viewport command.
/// @param RETURN A pointer to a <code>dx_val_command</code> variable.
/// @param binding The binding.
/// @param program The program.
/// @param l, b position along the x- (left) and y-axis (bottom) of the viewport.
/// @param w, h extends along the x- (width) and y-axis (height) of the viewport.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_command object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
/// @remarks
/// This assumes
/// - the origin (0,0) maps to the left bottom of the canvas.
/// - the positive x-axis goes from the left side of the canvas to the right side.
/// - the positive y-axis goes from the bottom side of the canvas to the top side.
dx_result dx_val_command_create_viewport(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h);


dx_result dx_val_command_construct_pipeline_state(dx_val_command* SELF, DX_CULL_MODE cull_mode, DX_DEPTH_TEST_FUNCTION depth_test_function, dx_bool depth_write_enabled);

/// Create a pipeline state command.
/// @param RETURN A pointer to a <code>dx_val_command</code> variable.
/// @param cull_mode The cull mode.
/// @param depth_test_function The depth test function.
/// @param depth_write_enabled If depth write is enabled.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the created dx_val_command object.
/// The caller has acquired a reference to the returned object.
/// @default-return
/// @default-failure
dx_result dx_val_command_create_pipeline_state(dx_val_command** RETURN, DX_CULL_MODE cull_mode, DX_DEPTH_TEST_FUNCTION depth_test_function, dx_bool depth_write_enabled);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.val.command_list",
                       dx_val_command_list,
                       dx_object);

static inline dx_val_command_list* DV_VAL_COMMAND_LIST(void* p) {
  return (dx_val_command_list*)p;
}

struct dx_val_command_list {
  dx_object _parent;
  dx_inline_object_array backend;
};

static inline dx_val_command_list_dispatch* DV_VAL_COMMAND_LIST_DISPATCH(void* p) {
  return (dx_val_command_list_dispatch*)p;
}

struct dx_val_command_list_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_val_command_list_construct(dx_val_command_list* SELF);

dx_result dx_val_command_list_create(dx_val_command_list** RETURN);

dx_result dx_val_command_list_append(dx_val_command_list* SELF, dx_val_command* command);

dx_result dx_val_command_list_clear(dx_val_command_list* SELF);

dx_size dx_val_command_list_get_size(dx_val_command_list const* SELF);

dx_val_command* dx_val_command_list_get_at(dx_val_command_list const* SELF, dx_size index);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_VAL_COMMAND_H_INCLUDED
