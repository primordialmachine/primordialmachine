#include "dx/rectangle_presenter.h"

#include "dx/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"

static Core_Result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color);

static Core_Result stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color);

Core_defineObjectType("dx.rectangle_presenter",
                      dx_rectangle_presenter,
                      dx_presenter);

static Core_Result append_rect(DX_VEC3* array, Core_Size* index, DX_RECT2_F32 const* xy, Core_Real32 z) {
  dx_vec3_set(array + *index + 0, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_bottom(xy), z);
  dx_vec3_set(array + *index + 1, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_bottom(xy), z);
  dx_vec3_set(array + *index + 2, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_top(xy),    z);

  dx_vec3_set(array + *index + 3, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_top(xy),    z);
  dx_vec3_set(array + *index + 4, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_top(xy),    z);
  dx_vec3_set(array + *index + 5, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_bottom(xy), z);

  *index = *index + 6;

  return Core_Success;
}

static Core_Result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color) {
  dx_val_command* command;

  if (dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color)) {
    return Core_Failure;
  }

  dx_val_command_list_clear(SELF->val_command_list);

  {
    DX_VEC3 vertices[6];
    Core_Size index = 0;
    append_rect(vertices, &index, target_rectangle, target_depth);
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, Core_CullMode_Back, Core_DepthCompareMode_Always, Core_False)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->val_vbinding, /*SELF->val_material ? SELF->val_material->ambient_texture :*/ NULL, SELF->val_cbinding, SELF->val_program, 0, 6)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (Core_Visuals_Context_executeCommands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color) {
  dx_val_command* command;

  if (dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color)) {
    return Core_Failure;
  }

  dx_val_command_list_clear(SELF->val_command_list);

  DX_VEC3 vertex_data[24];
  Core_Size vertex_index = 0;
  // left
  {
    DX_RECT2_F32 rectangle;
    dx_rect2_f32_set(&rectangle, dx_rect2_f32_get_left(target_rectangle),
                                 dx_rect2_f32_get_bottom(target_rectangle),
                                 dx_rect2_f32_get_left(target_rectangle) + 2.f,
                                 dx_rect2_f32_get_top(target_rectangle));
    append_rect(vertex_data, &vertex_index, &rectangle, target_depth);
  }
  // right
  {
    DX_RECT2_F32 rectangle;
    dx_rect2_f32_set(&rectangle, dx_rect2_f32_get_right(target_rectangle) - 2.f,
                                 dx_rect2_f32_get_bottom(target_rectangle),
                                 dx_rect2_f32_get_right(target_rectangle),
                                 dx_rect2_f32_get_top(target_rectangle));
    append_rect(vertex_data, &vertex_index, &rectangle, target_depth);
  }
  // top
  {
    DX_RECT2_F32 rectangle;
    dx_rect2_f32_set(&rectangle, dx_rect2_f32_get_left(target_rectangle),
                                 dx_rect2_f32_get_top(target_rectangle) - 2.f,
                                 dx_rect2_f32_get_right(target_rectangle),
                                 dx_rect2_f32_get_top(target_rectangle));
    append_rect(vertex_data, &vertex_index, &rectangle, target_depth);
  }
  // bottom
  {
    DX_RECT2_F32 rectangle;
    dx_rect2_f32_set(&rectangle, dx_rect2_f32_get_left(target_rectangle),
                                 dx_rect2_f32_get_bottom(target_rectangle),
                                 dx_rect2_f32_get_right(target_rectangle),
                                 dx_rect2_f32_get_bottom(target_rectangle) + 2.f);
    append_rect(vertex_data, &vertex_index, &rectangle, target_depth);
  }
  dx_val_buffer_set_data(SELF->val_buffer, &vertex_data, sizeof(vertex_data));

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, Core_CullMode_Back, Core_DepthCompareMode_Always, Core_False)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->val_vbinding, /*SELF->val_material ? SELF->val_material->ambient_texture :*/ NULL, SELF->val_cbinding, SELF->val_program, 0, 24)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (Core_Visuals_Context_executeCommands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return Core_Failure;
  }
  return Core_Success;
}

static void dx_rectangle_presenter_destruct(dx_rectangle_presenter* SELF) {
  CORE_UNREFERENCE(SELF->val_material);
  SELF->val_material = NULL;

  CORE_UNREFERENCE(SELF->val_command_list);
  SELF->val_command_list = NULL;

  CORE_UNREFERENCE(SELF->val_vbinding);
  SELF->val_vbinding = NULL;

  CORE_UNREFERENCE(SELF->val_buffer);
  SELF->val_buffer = NULL;

  CORE_UNREFERENCE(SELF->val_cbinding);
  SELF->val_cbinding = NULL;

  CORE_UNREFERENCE(SELF->val_program);
  SELF->val_program = NULL;
}

static void dx_rectangle_presenter_constructDispatch(dx_rectangle_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_rectangle_presenter_construct(dx_rectangle_presenter* SELF, Core_Visuals_Context* val_context, Core_Audials_Context* aal_context) {
  DX_CONSTRUCT_PREFIX(dx_rectangle_presenter);
  if (dx_presenter_construct(DX_PRESENTER(SELF), val_context, aal_context)) {
    return Core_Failure;
  }
  Core_String* path = NULL;
  if (Core_String_create(&path, "assets/gl/", sizeof("assets/gl/") - 1)) {
    return Core_Failure;
  }
  Core_String* filename = NULL;
  if (Core_String_create(&filename, "rectangle", sizeof("rectangle") - 1)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    //
    return Core_Failure;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, DX_PRESENTER(SELF)->val_context, path, filename)) {
    CORE_UNREFERENCE(filename);
    filename = NULL;
    //
    CORE_UNREFERENCE(path);
    path = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(filename);
  filename = NULL;
  //
  CORE_UNREFERENCE(path);
  path = NULL;
  //
  if (Core_Visuals_Context_createProgram(&SELF->val_program, DX_PRESENTER(SELF)->val_context, val_program_text)) {
    CORE_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    return Core_Failure;
  }
  CORE_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  //
  if (dx_val_cbinding_create(&SELF->val_cbinding)) {
    CORE_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return Core_Failure;
  }
  if (Core_Visuals_Context_createBuffer(&SELF->val_buffer, DX_PRESENTER(SELF)->val_context)) {
    CORE_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    CORE_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return Core_Failure;
  }
  //
  if (Core_Visuals_Context_createVariableBinding(&SELF->val_vbinding, DX_PRESENTER(SELF)->val_context, Core_VertexFormat_PositionXyz, SELF->val_buffer)) {
    CORE_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    CORE_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    CORE_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return Core_Failure;
  }
  //
  if (dx_val_command_list_create(&SELF->val_command_list)) {
    CORE_UNREFERENCE(SELF->val_vbinding);
    SELF->val_vbinding = NULL;
    //
    CORE_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    CORE_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    CORE_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return Core_Failure;
  }
  //
  {
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, DX_PRESENTER(SELF)->val_context, "<font-material>")) {
      CORE_UNREFERENCE(SELF->val_command_list);
      SELF->val_command_list = NULL;
      //
      CORE_UNREFERENCE(SELF->val_vbinding);
      SELF->val_vbinding = NULL;
      //
      CORE_UNREFERENCE(SELF->val_buffer);
      SELF->val_buffer = NULL;
      //
      CORE_UNREFERENCE(SELF->val_cbinding);
      SELF->val_cbinding = NULL;
      //
      CORE_UNREFERENCE(SELF->val_program);
      SELF->val_program = NULL;
      //
      return Core_Failure;
    }
  }
  //
  CORE_OBJECT(SELF)->type = TYPE;
  //
  return Core_Success;
}

Core_Result dx_rectangle_presenter_create(dx_rectangle_presenter** RETURN, Core_Visuals_Context* val_context, Core_Audials_Context* aal_context) {
  DX_CREATE_PREFIX(dx_rectangle_presenter)
  if (dx_rectangle_presenter_construct(SELF, val_context, aal_context)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_rectangle_presenter_fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color) {
  return fill_rectangle(SELF, target_rectangle, target_depth, color);
}

Core_Result dx_rectangle_presenter_stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color) {
  return stroke_rectangle(SELF, target_rectangle, target_depth, color);
}
