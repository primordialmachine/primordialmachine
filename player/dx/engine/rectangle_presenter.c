#include "dx/engine/rectangle_presenter.h"

#include "dx/engine/utilities_2d.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"

static dx_result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

static dx_result stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

DX_DEFINE_OBJECT_TYPE("dx.rectangle_presenter",
                      dx_rectangle_presenter,
                      dx_object);

static dx_result fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color) {
  dx_val_command* command;

  if (dx_val_cbinding_set_rgba_f32(SELF->val_cbinding, "vs_rgba", color)) {
    return DX_FAILURE;
  }

  dx_val_command_list_clear(SELF->val_command_list);

  {
    DX_VEC3 vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->top,    target_depth, },

        { target_rectangle->right, target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, DX_CULL_MODE_BACK, DX_DEPTH_TEST_FUNCTION_ALWAYS, DX_FALSE)) {
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
  if (dx_val_context_execute_commands(SELF->val_context, SELF->val_command_list)) {
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

  {
    DX_VEC3 vertices[] = {
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->bottom, target_depth, },
        { target_rectangle->right, target_rectangle->top,    target_depth, },

        { target_rectangle->right, target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->top,    target_depth, },
        { target_rectangle->left,  target_rectangle->bottom, target_depth, },
    };
    dx_val_buffer_set_data(SELF->val_buffer, &vertices, sizeof(vertices));
  }

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, DX_CULL_MODE_BACK, DX_DEPTH_TEST_FUNCTION_ALWAYS, DX_FALSE)) {
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
  if (dx_val_context_execute_commands(SELF->val_context, SELF->val_command_list)) {
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

  DX_UNREFERENCE(SELF->val_context);
  SELF->val_context = NULL;
}

static void dx_rectangle_presenter_dispatch_construct(dx_rectangle_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_rectangle_presenter_construct(dx_rectangle_presenter* SELF, dx_val_context* val_context) {
  dx_rti_type* TYPE = dx_rectangle_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (!val_context) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->val_context = val_context;
  DX_REFERENCE(SELF->val_context);

  dx_string* path = NULL;
  if (dx_string_create(&path, "assets/gl/", sizeof("assets/gl/") - 1)) {
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  dx_string* filename = NULL;
  if (dx_string_create(&filename, "rectangle", sizeof("rectangle") - 1)) {
    DX_UNREFERENCE(path);
    path = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  dx_val_program_text* val_program_text = NULL;
  if (dx_engine_utilities_2d_create_program_text(&val_program_text, SELF->val_context, path, filename)) {
    DX_UNREFERENCE(filename);
    filename = NULL;
    //
    DX_UNREFERENCE(path);
    path = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(filename);
  filename = NULL;
  //
  DX_UNREFERENCE(path);
  path = NULL;
  //
  if (dx_val_context_create_program(&SELF->val_program, SELF->val_context, val_program_text)) {
    DX_UNREFERENCE(val_program_text);
    val_program_text = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  DX_UNREFERENCE(val_program_text);
  val_program_text = NULL;
  //
  SELF->val_cbinding = dx_val_cbinding_create();
  if (!SELF->val_cbinding) {
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  if (dx_val_context_create_buffer(&SELF->val_buffer, SELF->val_context)) {
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  //
  if (dx_val_context_create_vbinding(&SELF->val_vbinding, SELF->val_context, DX_VERTEX_FORMAT_POSITION_XYZ, SELF->val_buffer)) {
    DX_UNREFERENCE(SELF->val_buffer);
    SELF->val_buffer = NULL;
    //
    DX_UNREFERENCE(SELF->val_cbinding);
    SELF->val_cbinding = NULL;
    //
    DX_UNREFERENCE(SELF->val_program);
    SELF->val_program = NULL;
    //
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
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
    DX_UNREFERENCE(SELF->val_context);
    SELF->val_context = NULL;
    //
    return DX_FAILURE;
  }
  //
  {
    if (dx_engine_utilities_2d_create_material(&SELF->val_material, SELF->val_context, "<font-material>")) {
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
      DX_UNREFERENCE(SELF->val_context);
      SELF->val_context = NULL;
      //
      return DX_FAILURE;
    }
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  //
  return DX_SUCCESS;
}

dx_result dx_rectangle_presenter_create(dx_rectangle_presenter** RETURN, dx_val_context* val_context) {
  dx_rectangle_presenter* SELF = DX_RECTANGLE_PRESENTER(dx_object_alloc(sizeof(dx_rectangle_presenter)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_rectangle_presenter_construct(SELF, val_context)) {
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
