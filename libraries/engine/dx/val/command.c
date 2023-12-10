#include "dx/val/command.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.command",
                      dx_val_command,
                      Core_Object);

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

static void dx_val_command_constructDispatch(dx_val_command_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_command_construct_clear_color(dx_val_command* SELF, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h, DX_RGBA_F32 const* color) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
  SELF->kind = DX_VAL_COMMAND_KIND_CLEAR_COLOR;
  SELF->clear_color_command.rectangle.l = l;
  SELF->clear_color_command.rectangle.b = b;
  SELF->clear_color_command.rectangle.w = w;
  SELF->clear_color_command.rectangle.h = h;
  SELF->clear_color_command.color = *color;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_clear_color(dx_val_command** RETURN, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h, DX_RGBA_F32 const* color) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_clear_color(SELF, l, b, w, h, color)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_command_construct_clear_depth(dx_val_command* SELF, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h, Core_Real32 depth) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
  SELF->kind = DX_VAL_COMMAND_KIND_CLEAR_DEPTH;
  SELF->clear_depth_command.rectangle.l = l;
  SELF->clear_depth_command.rectangle.b = b;
  SELF->clear_depth_command.rectangle.w = w;
  SELF->clear_depth_command.rectangle.h = h;
  SELF->clear_depth_command.depth = depth;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_clear_depth(dx_val_command** RETURN, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h, Core_Real32 depth) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_clear_depth(SELF, l, b, w, h, depth)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_command_construct_draw(dx_val_command* SELF, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, Core_Natural32 start, Core_Natural32 length) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
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

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_draw(dx_val_command** RETURN, dx_val_vbinding* vbinding, dx_val_texture* texture, dx_val_cbinding* cbinding, dx_val_program* program, Core_Natural32 start, Core_Natural32 length) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_draw(SELF, vbinding, texture, cbinding, program, start, length)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_command_construct_viewport(dx_val_command* SELF, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
  SELF->kind = DX_VAL_COMMAND_KIND_VIEWPORT;
  SELF->viewport_command.l = l;
  SELF->viewport_command.b = b;
  SELF->viewport_command.w = w;
  SELF->viewport_command.h = h;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_viewport(dx_val_command** RETURN, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_viewport(SELF, l, b, w, h)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_command_construct_scissor_test(dx_val_command* SELF, Core_Boolean enabled, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
  SELF->kind = DX_VAL_COMMAND_KIND_SCISSOR_TEST;
  SELF->scissor_test.enabled = enabled;
  SELF->scissor_test.rectangle.l = l;
  SELF->scissor_test.rectangle.b = b;
  SELF->scissor_test.rectangle.w = w;
  SELF->scissor_test.rectangle.h = h;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_scissor_test(dx_val_command** RETURN, Core_Boolean enabled, Core_Real32 l, Core_Real32 b, Core_Real32 w, Core_Real32 h) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_scissor_test(SELF, enabled, l, b, w, h)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}


Core_Result dx_val_command_construct_pipeline_state(dx_val_command* SELF, Core_CullMode cullMode, Core_DepthCompareFunction depthCompareFunction, Core_Boolean depthWriteEnabled) {
  DX_CONSTRUCT_PREFIX(dx_val_command);
  SELF->kind = DX_VAL_COMMAND_KIND_PIPELINE_STATE;
  SELF->pipeline_state.cullMode = cullMode;
  SELF->pipeline_state.depthCompareFunction = depthCompareFunction;
  SELF->pipeline_state.depthWriteEnabled = depthWriteEnabled;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_create_pipeline_state(dx_val_command** RETURN, Core_CullMode cull_mode, Core_DepthCompareFunction depthCompareFunction, Core_Boolean depth_write_enabled) {
  DX_CREATE_PREFIX(dx_val_command);
  if (dx_val_command_construct_pipeline_state(SELF, cull_mode, depthCompareFunction, depth_write_enabled)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.val.command_list",
                      dx_val_command_list,
                      Core_Object);

static void dx_val_command_list_destruct(dx_val_command_list* SELF) {
  dx_inline_object_array_uninitialize(&SELF->backend);
}

static void dx_val_command_list_constructDispatch(dx_val_command_list_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_command_list_construct(dx_val_command_list* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_command_list);
  if (dx_inline_object_array_initialize(&SELF->backend, 0)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_command_list_create(dx_val_command_list** RETURN) {
  DX_CREATE_PREFIX(dx_val_command_list);
  if (dx_val_command_list_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_command_list_append(dx_val_command_list* SELF, dx_val_command* command) {
  return dx_inline_object_array_append(&SELF->backend, CORE_OBJECT(command));
}

Core_Result dx_val_command_list_clear(dx_val_command_list* SELF) {
  return dx_inline_object_array_clear(&SELF->backend);
}

Core_Size dx_val_command_list_get_size(dx_val_command_list const* SELF) {
  Core_Size temporary;
  if (dx_inline_object_array_get_size(&temporary, &SELF->backend)) {
    return 0;
  }
  return temporary;
}

dx_val_command* dx_val_command_list_get_at(dx_val_command_list const* SELF, Core_Size index) {
  dx_val_command* val_command = NULL;
  if (dx_inline_object_array_get_at((Core_Object**) & val_command, &SELF->backend, index)) {
    return NULL;
  }
  return val_command;
}
