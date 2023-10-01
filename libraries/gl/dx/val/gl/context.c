#include "dx/val/gl/context.h"

#include "dx/val/gl/buffer.h"
#include "dx/val/gl/vbinding.h"
#include "dx/val/gl/program.h"
#include "dx/val/gl/texture.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"
#include "dx/val/program_text.h"

static dx_result get_renderer(dx_string** RETURN, dx_val_gl_context* SELF) {
  GLubyte const* p = SELF->glGetString(GL_RENDERER);
  return dx_string_create(RETURN, p, strlen(p));
}

static dx_result get_vendor(dx_string** RETURN, dx_val_gl_context* SELF) {
  GLubyte const* p = SELF->glGetString(GL_VENDOR);
  return dx_string_create(RETURN, p, strlen(p));
}

static dx_result get_major_version(int32_t* RETURN, dx_val_gl_context* SELF) {
  GLint v;
  SELF->glGetIntegerv(GL_MAJOR_VERSION, &v);
  *RETURN = v;
  return DX_SUCCESS;
}

static dx_result get_minor_version(int32_t* RETURN, dx_val_gl_context* SELF) {
  GLint v;
  SELF->glGetIntegerv(GL_MINOR_VERSION, &v);
  *RETURN = v;
  return DX_SUCCESS;
}

static dx_string* get_information(dx_val_gl_context* self) {
  dx_string* renderer = NULL;
  if (get_renderer(&renderer, self)) {
    return NULL;
  }
  dx_string* vendor = NULL;
  if (get_vendor(&vendor, self)) {
    DX_UNREFERENCE(renderer);
    renderer = NULL;
    return NULL;
  }
  dx_bool vsync_enabled;
  if (dx_val_context_get_vsync_enabled(&vsync_enabled, DX_VAL_CONTEXT(self))) {
    DX_UNREFERENCE(vendor);
    vendor = NULL;
    DX_UNREFERENCE(renderer);
    renderer = NULL;
    return NULL;
  }
  dx_i32 major_version, minor_version;
  if (get_major_version(&major_version, self) || get_minor_version(&minor_version, self)) {
    DX_UNREFERENCE(vendor);
    vendor = NULL;
    DX_UNREFERENCE(renderer);
    renderer = NULL;
    return NULL;
  }
  static const char* FORMAT = "renderer: ${s}\nvendor: ${s}\nOpenGL version: ${i32}.${i32}\nvsync enabled: ${i32}\n";
  dx_string* format = NULL;
  if (dx_string_create(&format, FORMAT, strlen(FORMAT))) {
    DX_UNREFERENCE(vendor);
    vendor = NULL;
    DX_UNREFERENCE(renderer);
    renderer = NULL;
    return NULL;
  }
  dx_string* information = dx_string_printf(format,
                                            renderer,
                                            vendor,
                                            major_version,
                                            minor_version,
                                            vsync_enabled ? 1 : 0);
  DX_UNREFERENCE(format);
  format = NULL;
  DX_UNREFERENCE(vendor);
  vendor = NULL;
  DX_UNREFERENCE(renderer);
  renderer = NULL;
  return information;
}

DX_DEFINE_OBJECT_TYPE("dx.val.gl.context",
                      dx_val_gl_context,
                      dx_val_context);

static dx_result enter_frame(dx_val_gl_context* SELF) {
  SELF->glDepthMask(SELF->depth_write_enabled ? GL_TRUE : GL_FALSE);
  return DX_SUCCESS;
}

static dx_result bind_texture(dx_val_gl_context* ctx, dx_size unit, dx_val_gl_texture* texture) {
  if (texture) {
    if (dx_val_gl_texture_upload(texture)) {
      return DX_FAILURE;
    }
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, texture->id);
  } else {
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, 0);
  }
  return DX_SUCCESS;
}

static dx_result create_buffer(dx_val_gl_buffer** RETURN, dx_val_gl_context* SELF) {
  return dx_val_gl_buffer_create(RETURN, SELF);
}

static dx_result create_cbinding(dx_val_cbinding** RETURN, dx_val_gl_context* SELF) {
  return dx_val_cbinding_create(RETURN);
}

static dx_result create_vbinding(dx_val_gl_vbinding** RETURN, dx_val_gl_context* SELF, dx_vertex_format vertex_format, dx_val_gl_buffer* buffer) {
  return dx_val_gl_vbinding_create(RETURN, vertex_format, buffer);
}

static dx_result create_program(dx_val_gl_program** RETURN, dx_val_gl_context* SELF, dx_val_program_text* program_text) {
  if (!program_text || DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT != program_text->type) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_val_gl_program_create(RETURN, SELF, program_text->vertex_program_text->program_text, program_text->fragment_program_text->program_text);
}

static dx_result create_texture(dx_val_gl_texture** RETURN, dx_val_gl_context* SELF) {
  return dx_val_gl_texture_create(RETURN, SELF);
}

static dx_result execute_commands(dx_val_gl_context* SELF, dx_val_command_list* commands) {
  for (dx_size i = 0, n = dx_val_command_list_get_size(commands); i < n; ++i) {
    dx_val_command* command = dx_val_command_list_get_at(commands, i);
    switch (command->kind) {
    case DX_VAL_COMMAND_KIND_CLEAR_COLOR: {
      SELF->glEnable(GL_SCISSOR_TEST);
      SELF->glScissor(command->clear_color_command.rectangle.l,
                      command->clear_color_command.rectangle.b,
                      command->clear_color_command.rectangle.w,
                      command->clear_color_command.rectangle.h);
      SELF->glClearColor(command->clear_color_command.color.r,
                         command->clear_color_command.color.g,
                         command->clear_color_command.color.b,
                         command->clear_color_command.color.a);
      SELF->glClear(GL_COLOR_BUFFER_BIT);
    } break;
    case DX_VAL_COMMAND_KIND_CLEAR_DEPTH: {
      SELF->glEnable(GL_SCISSOR_TEST);
      SELF->glScissor(command->clear_depth_command.rectangle.l,
                      command->clear_depth_command.rectangle.b,
                      command->clear_depth_command.rectangle.w,
                      command->clear_depth_command.rectangle.h);
      SELF->glClearDepthf(1.f);
      SELF->glDepthMask(GL_TRUE);
      SELF->glClear(GL_DEPTH_BUFFER_BIT);
      // Restore as this is part of the command-modifiable state.
      SELF->glDepthMask(SELF->depth_write_enabled ? GL_TRUE : GL_FALSE);
    } break;
    case DX_VAL_COMMAND_KIND_DRAW: {
      // apply the constant binding to the program.
      if (dx_val_program_bind(command->draw_command.program, command->draw_command.cbinding)) {
        return DX_FAILURE;
      }
      // assign texture to texture unit
      if (command->draw_command.texture) {
        dx_val_context_bind_texture(DX_VAL_CONTEXT(SELF), 0, command->draw_command.texture);
      } else {
        dx_val_context_bind_texture(DX_VAL_CONTEXT(SELF), 0, NULL);
      }
      // activate binding and activate program, then render.
      if (dx_val_program_activate(command->draw_command.program)) {
        return DX_FAILURE;
      }
      if (dx_val_vbinding_activate(command->draw_command.vbinding)) {
        return DX_FAILURE;
      }
      SELF->glDrawArrays(GL_TRIANGLES, command->draw_command.start, command->draw_command.length);
    } break;
    case DX_VAL_COMMAND_KIND_VIEWPORT: {
      SELF->glViewport(command->viewport_command.l,
                       command->viewport_command.b,
                       command->viewport_command.w,
                       command->viewport_command.h);
    } break;
    case DX_VAL_COMMAND_KIND_PIPELINE_STATE: {
      SELF->glEnable(GL_BLEND);
      SELF->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      SELF->glEnable(GL_DEPTH_TEST);
      switch (command->pipeline_state.depth_test_function) {
        case dx_depth_test_function_always: {
          SELF->glDepthFunc(GL_ALWAYS);
        } break;
        case dx_depth_test_function_never: {
          SELF->glDepthFunc(GL_NEVER);
        } break;
        case dx_depth_test_function_less_than: {
          SELF->glDepthFunc(GL_LESS);
        } break;
        case dx_depth_test_function_less_than_or_equal_to: {
          SELF->glDepthFunc(GL_LEQUAL);
        } break;
        case dx_depth_test_function_greater_than: {
          SELF->glDepthFunc(GL_GREATER);
        } break;
        case dx_depth_test_function_greater_than_or_equal_to: {
          SELF->glDepthFunc(GL_GEQUAL);
        } break;
        case dx_depth_test_function_equal: {
          SELF->glDepthFunc(GL_EQUAL);
        } break;
        case dx_depth_test_function_not_equal: {
          SELF->glDepthFunc(GL_NOTEQUAL);
        } break;
        default: {
          dx_set_error(DX_ERROR_INVALID_ARGUMENT);
          return DX_FAILURE;
        } break;
      };
      SELF->glDepthMask(command->pipeline_state.depth_write_enabled ? GL_TRUE : GL_FALSE);
      switch (command->pipeline_state.cull_mode) {
        case dx_cull_mode_none: {
          SELF->glDisable(GL_CULL_FACE);
        } break;
        case dx_cull_mode_back: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_BACK);
        } break;
        case dx_cull_mode_front: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_FRONT);
        } break;
        case dx_cull_mode_front_and_back: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_FRONT_AND_BACK);
        } break;
        default: {
          dx_set_error(DX_ERROR_INVALID_ARGUMENT);
          return DX_FAILURE;
        } break;
      };
    } break;
    case DX_VAL_COMMAND_KIND_IDLE: {
      // Do nothing.
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
    }
  }
  return DX_SUCCESS;
}

static void dx_val_gl_context_destruct(dx_val_gl_context* SELF) {
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/functions.i"
#undef DEFINE
}

static void dx_val_gl_context_dispatch_construct(dx_val_gl_context_dispatch* SELF) {
  DX_VAL_CONTEXT_DISPATCH(SELF)->get_information = (dx_string * (*)(dx_val_context*)) & get_information;
  DX_VAL_CONTEXT_DISPATCH(SELF)->bind_texture = (dx_result(*)(dx_val_context*, dx_size, dx_val_texture*)) & bind_texture;
  DX_VAL_CONTEXT_DISPATCH(SELF)->create_buffer = (dx_result (*)(dx_val_buffer**, dx_val_context*)) & create_buffer;
  DX_VAL_CONTEXT_DISPATCH(SELF)->create_cbinding = (dx_result(*)(dx_val_cbinding**, dx_val_context*)) & create_cbinding;
  DX_VAL_CONTEXT_DISPATCH(SELF)->create_vbinding = (dx_result (*)(dx_val_vbinding**, dx_val_context*, dx_vertex_format, dx_val_buffer*)) & create_vbinding;
  DX_VAL_CONTEXT_DISPATCH(SELF)->create_program = (dx_result (*)(dx_val_program**,dx_val_context*, dx_val_program_text*)) & create_program;
  DX_VAL_CONTEXT_DISPATCH(SELF)->create_texture = (dx_result (*)(dx_val_texture**, dx_val_context*)) & create_texture;
  DX_VAL_CONTEXT_DISPATCH(SELF)->execute_commands = (dx_result(*)(dx_val_context*, dx_val_command_list*)) & execute_commands;
  DX_VAL_CONTEXT_DISPATCH(SELF)->enter_frame = (dx_result(*)(dx_val_context*)) & enter_frame;
}

dx_result dx_val_gl_context_construct(dx_val_gl_context* SELF, void *(*link)(char const *name)) {
  dx_rti_type* TYPE = dx_val_gl_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_context_construct(DX_VAL_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/functions.i"
#undef DEFINE

#define DEFINE(TYPE, NAME, EXTENSION_NAME) \
  (SELF)->NAME = (TYPE)link(#NAME); \
  if (!(SELF)->NAME) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n") - 1); \
    return DX_FAILURE; \
  }
#include "dx/val/gl/functions.i"
#undef DEFINE

  SELF->depth_write_enabled = DX_TRUE;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
