#include "dx/val/command.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.command",
                      dx_val_command,
                      dx_object);

static void dx_val_command_destruct(dx_val_command* SELF) {
  switch (SELF->kind) {
    case DX_VAL_COMMAND_KIND_DRAW: {
      DX_UNREFERENCE(SELF->draw_command.program);
      SELF->draw_command.program = NULL;
      DX_UNREFERENCE(SELF->draw_command.cbinding);
      SELF->draw_command.cbinding = NULL;
      if (SELF->draw_command.texture) {
        DX_UNREFERENCE(SELF->draw_command.texture);
        SELF->draw_command.texture = NULL;
      }
      DX_UNREFERENCE(SELF->draw_command.vbinding);
      SELF->draw_command.vbinding = NULL;
    } break;
  };
}

static void dx_val_command_dispatch_construct(dx_val_command_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_command_construct_clear_color(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_RGBA_F32 const* color) {
  dx_rti_type* TYPE = dx_val_command_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = DX_VAL_COMMAND_KIND_CLEAR_COLOR;
  SELF->clear_color_command.rectangle.l = l;
  SELF->clear_color_command.rectangle.b = b;
  SELF->clear_color_command.rectangle.w = w;
  SELF->clear_color_command.rectangle.h = h;
  SELF->clear_color_command.color = *color;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_create_clear_color(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, DX_RGBA_F32 const* color) {
  DX_CREATE_PREFIX(dx_val_command)
  if (dx_val_command_construct_clear_color(SELF, l, b, w, h, color)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_command_construct_clear_depth(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth) {
  dx_rti_type* TYPE = dx_val_command_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = DX_VAL_COMMAND_KIND_CLEAR_DEPTH;
  SELF->clear_depth_command.rectangle.l = l;
  SELF->clear_depth_command.rectangle.b = b;
  SELF->clear_depth_command.rectangle.w = w;
  SELF->clear_depth_command.rectangle.h = h;
  SELF->clear_depth_command.depth = depth;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_create_clear_depth(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h, dx_f32 depth) {
  DX_CREATE_PREFIX(dx_val_command)
  if (dx_val_command_construct_clear_depth(SELF, l, b, w, h, depth)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_command_construct_draw(dx_val_command* SELF, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, dx_n32 start, dx_n32 length) {
  dx_rti_type* TYPE = dx_val_command_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = DX_VAL_COMMAND_KIND_DRAW;
  SELF->draw_command.vbinding = vbinding;
  DX_REFERENCE(vbinding);
  if (texture) {
    SELF->draw_command.texture = texture;
    DX_REFERENCE(texture);
  } else {
    SELF->draw_command.texture = NULL;
  }
  SELF->draw_command.cbinding = cbinding;
  DX_REFERENCE(cbinding);
  SELF->draw_command.program = program;
  DX_REFERENCE(program);
  SELF->draw_command.start = start;
  SELF->draw_command.length = length;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_create_draw(dx_val_command** RETURN, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, dx_n32 start, dx_n32 length) {
  DX_CREATE_PREFIX(dx_val_command)
  if (dx_val_command_construct_draw(SELF, vbinding, texture, cbinding, program, start, length)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_command_construct_viewport(dx_val_command* SELF, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h) {
  dx_rti_type* TYPE = dx_val_command_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = DX_VAL_COMMAND_KIND_VIEWPORT;
  SELF->viewport_command.l = l;
  SELF->viewport_command.b = b;
  SELF->viewport_command.w = w;
  SELF->viewport_command.h = h;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_create_viewport(dx_val_command** RETURN, dx_f32 l, dx_f32 b, dx_f32 w, dx_f32 h) {
  DX_CREATE_PREFIX(dx_val_command)
  if (dx_val_command_construct_viewport(SELF, l, b, w, h)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_command_construct_pipeline_state(dx_val_command* SELF, dx_cull_mode cull_mode, dx_depth_test_function depth_test_function, dx_bool depth_write_enabled) {
  dx_rti_type* TYPE = dx_val_command_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = DX_VAL_COMMAND_KIND_PIPELINE_STATE;
  SELF->pipeline_state.cull_mode = cull_mode;
  SELF->pipeline_state.depth_test_function = depth_test_function;
  SELF->pipeline_state.depth_write_enabled = depth_write_enabled;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_create_pipeline_state(dx_val_command** RETURN, dx_cull_mode cull_mode, dx_depth_test_function depth_test_function, dx_bool depth_write_enabled) {
  DX_CREATE_PREFIX(dx_val_command)
  if (dx_val_command_construct_pipeline_state(SELF, cull_mode, depth_test_function, depth_write_enabled)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.command_list",
                      dx_val_command_list,
                      dx_object);

static void dx_val_command_list_destruct(dx_val_command_list* SELF) {
  dx_inline_object_array_uninitialize(&SELF->backend);
}

static void dx_val_command_list_dispatch_construct(dx_val_command_list_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_command_list_construct(dx_val_command_list* SELF) {
  dx_rti_type* TYPE = dx_val_command_list_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(&SELF->backend, 0)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_command_list_create(dx_val_command_list** RETURN) {
  DX_CREATE_PREFIX(dx_val_command_list)
  if (dx_val_command_list_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_command_list_append(dx_val_command_list* SELF, dx_val_command* command) {
  return dx_inline_object_array_append(&SELF->backend, DX_OBJECT(command));
}

dx_result dx_val_command_list_clear(dx_val_command_list* SELF) {
  return dx_inline_object_array_clear(&SELF->backend);
}

dx_size dx_val_command_list_get_size(dx_val_command_list const* SELF) {
  dx_size temporary;
  if (dx_inline_object_array_get_size(&temporary, &SELF->backend)) {
    return 0;
  }
  return temporary;
}

dx_val_command* dx_val_command_list_get_at(dx_val_command_list const* SELF, dx_size index) {
  dx_val_command* val_command = NULL;
  if (dx_inline_object_array_get_at((dx_object**) & val_command, &SELF->backend, index)) {
    return NULL;
  }
  return val_command;
}
