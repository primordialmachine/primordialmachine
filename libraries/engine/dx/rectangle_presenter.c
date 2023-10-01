#include "dx/rectangle_presenter.h"

#include "dx/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"

static dx_result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

static dx_result stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

DX_DEFINE_OBJECT_TYPE("dx.rectangle_presenter",
                      dx_rectangle_presenter,
                      dx_presenter);

static dx_result append_rect(DX_VEC3* array, dx_size* index, DX_RECT2_F32 const* xy, dx_f32 z) {
  dx_vec3_set(array + *index + 0, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_bottom(xy), z);
  dx_vec3_set(array + *index + 1, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_bottom(xy), z);
  dx_vec3_set(array + *index + 2, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_top(xy),    z);

  dx_vec3_set(array + *index + 3, dx_rect2_f32_get_right(xy), dx_rect2_f32_get_top(xy),    z);
  dx_vec3_set(array + *index + 4, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_top(xy),    z);
  dx_vec3_set(array + *index + 5, dx_rect2_f32_get_left(xy),  dx_rect2_f32_get_bottom(xy), z);

  *index = *index + 6;

  return DX_SUCCESS;
}

static dx_result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color) {
  dx_val_command* command;

  if (dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color)) {
    return DX_FAILURE;
  }

  dx_val_command_list_clear(SELF->val_command_list);

  {
    DX_VEC3 vertices[6];
    dx_size index = 0;
    append_rect(vertices, &index, target_rectangle, target_depth);
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, dx_cull_mode_back, dx_depth_test_function_always, DX_FALSE)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->val_vbinding, /*SELF->val_material ? SELF->val_material->ambient_texture :*/ NULL, SELF->val_cbinding, SELF->val_program, 0, 6)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color) {
  dx_val_command* command;

  if (dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color)) {
    return DX_FAILURE;
  }

  dx_val_command_list_clear(SELF->val_command_list);

  DX_VEC3 vertex_data[24];
  dx_size vertex_index = 0;
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
  if (dx_val_command_create_pipeline_state(&command, dx_cull_mode_back, dx_depth_test_function_always, DX_FALSE)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // draw command
  if (dx_val_command_create_draw(&command, SELF->val_vbinding, /*SELF->val_material ? SELF->val_material->ambient_texture :*/ NULL, SELF->val_cbinding, SELF->val_program, 0, 24)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->val_command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // execute the commands
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF)->val_context, SELF->val_command_list)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static void dx_rectangle_presenter_destruct(dx_rectangle_presenter* SELF) {
  DX_UNREFERENCE(SELF->val_material);
  SELF->val_material = NULL;

  DX_UNREFERENCE(SELF->val_command_list);
  SELF->val_command_list = NULL;

  DX_UNREFERENCE(SELF->val_vbinding);
  SELF->val_vbinding = NULL;

  DX_UNREFERENCE(SELF->val_buffer);
  SELF->val_buffer = NULL;

  DX_UNREFERENCE(SELF->val_cbinding);
  SELF->val_cbinding = NULL;

  DX_UNREFERENCE(SELF->val_program);
  SELF->val_program = NULL;
}

static void dx_rectangle_presenter_dispatch_construct(dx_rectangle_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_rectangle_presenter_construct(dx_rectangle_presenter* SELF, dx_val_context* val_context, dx_aal_context* aal_context) {
  dx_rti_type* TYPE = dx_rectangle_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_presenter_construct(DX_PRESENTER(SELF), val_context, aal_context)) {
    return DX_FAILURE;
  }
  dx_string* path = NULL;
  if (dx_string_create(&path, "assets/gl/", sizeof("assets/gl/") - 1)) {
    return DX_FAILURE;
  }
  dx_string* filename = NULL;
  if (dx_string_create(&filename, "rectangle", sizeof("rectangle") - 1)) {
    DX_UNREFERENCE(path);
    path = NULL;
    //
    return DX_FAILURE;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, DX_PRESENTER(SELF)->val_context, path, filename)) {
    DX_UNREFERENCE(filename);
    filename = NULL;
    //
    DX_UNREFERENCE(path);
    path = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(filename);
  filename = NULL;
  //
  DX_UNREFERENCE(path);
  path = NULL;
  //
  if (dx_val_context_create_program(&SELF->val_program, DX_PRESENTER(SELF)->val_context, val_program_text)) {
    DX_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  //
  if (dx_val_cbinding_create(&SELF->val_cbinding)) {
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return DX_FAILURE;
  }
  if (dx_val_context_create_buffer(&SELF->val_buffer, DX_PRESENTER(SELF)->val_context)) {
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return DX_FAILURE;
  }
  //
  if (dx_val_context_create_vbinding(&SELF->val_vbinding, DX_PRESENTER(SELF)->val_context, dx_vertex_format_position_xyz, SELF->val_buffer)) {
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return DX_FAILURE;
  }
  //
  if (dx_val_command_list_create(&SELF->val_command_list)) {
    DX_UNREFERENCE(SELF->val_vbinding);
    SELF->val_vbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    return DX_FAILURE;
  }
  //
  {
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, DX_PRESENTER(SELF)->val_context, "<font-material>")) {
      DX_UNREFERENCE(SELF->val_command_list);
      SELF->val_command_list = NULL;
      //
      DX_UNREFERENCE(SELF->val_vbinding);
      SELF->val_vbinding = NULL;
      //
      DX_UNREFERENCE(SELF->val_buffer);
      SELF->val_buffer = NULL;
      //
      DX_UNREFERENCE(SELF->val_cbinding);
      SELF->val_cbinding = NULL;
      //
      DX_UNREFERENCE(SELF->val_program);
      SELF->val_program = NULL;
      //
      return DX_FAILURE;
    }
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  //
  return DX_SUCCESS;
}

dx_result dx_rectangle_presenter_create(dx_rectangle_presenter** RETURN, dx_val_context* val_context, dx_aal_context* aal_context) {
  DX_CREATE_PREFIX(dx_rectangle_presenter)
  if (dx_rectangle_presenter_construct(SELF, val_context, aal_context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_rectangle_presenter_fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color) {
  return fill_rectangle(SELF, target_rectangle, target_depth, color);
}

dx_result dx_rectangle_presenter_stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color) {
  return stroke_rectangle(SELF, target_rectangle, target_depth, color);
}
