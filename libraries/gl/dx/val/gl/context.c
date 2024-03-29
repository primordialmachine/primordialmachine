#include "dx/val/gl/context.h"

#include "dx/val/gl/buffer.h"
#include "dx/val/gl/vbinding.h"
#include "dx/val/gl/program.h"
#include "dx/val/gl/texture.h"
#include "dx/val/cbinding.h"
#include "dx/val/command.h"
#include "dx/val/program_text.h"

static Core_Result get_renderer(Core_String** RETURN, dx_val_gl_context* SELF) {
  GLubyte const* p = SELF->glGetString(GL_RENDERER);
  return Core_String_create(RETURN, p, strlen(p));
}

static Core_Result get_vendor(Core_String** RETURN, dx_val_gl_context* SELF) {
  GLubyte const* p = SELF->glGetString(GL_VENDOR);
  return Core_String_create(RETURN, p, strlen(p));
}

static Core_Result get_major_version(int32_t* RETURN, dx_val_gl_context* SELF) {
  GLint v;
  SELF->glGetIntegerv(GL_MAJOR_VERSION, &v);
  *RETURN = v;
  return Core_Success;
}

static Core_Result get_minor_version(int32_t* RETURN, dx_val_gl_context* SELF) {
  GLint v;
  SELF->glGetIntegerv(GL_MINOR_VERSION, &v);
  *RETURN = v;
  return Core_Success;
}

static Core_Result get_information(Core_String** RETURN, dx_val_gl_context* self) {
  Core_String* renderer = NULL;
  if (get_renderer(&renderer, self)) {
    return Core_Failure;
  }
  Core_String* vendor = NULL;
  if (get_vendor(&vendor, self)) {
    CORE_UNREFERENCE(renderer);
    renderer = NULL;
    return Core_Failure;
  }
  Core_Boolean vsync_enabled;
  if (Core_Visuals_Context_getVsyncEnabled(&vsync_enabled, CORE_VISUALS_CONTEXT(self))) {
    CORE_UNREFERENCE(vendor);
    vendor = NULL;
    CORE_UNREFERENCE(renderer);
    renderer = NULL;
    return Core_Failure;
  }
  Core_Integer32 major_version, minor_version;
  if (get_major_version(&major_version, self) || get_minor_version(&minor_version, self)) {
    CORE_UNREFERENCE(vendor);
    vendor = NULL;
    CORE_UNREFERENCE(renderer);
    renderer = NULL;
    return Core_Failure;
  }
  static const char* FORMAT = "renderer: ${s}\nvendor: ${s}\nOpenGL version: ${i32}.${i32}\nvsync enabled: ${i32}\n";
  Core_String* format = NULL;
  if (Core_String_create(&format, FORMAT, strlen(FORMAT))) {
    CORE_UNREFERENCE(vendor);
    vendor = NULL;
    CORE_UNREFERENCE(renderer);
    renderer = NULL;
    return Core_Failure;
  }
  Core_String* information = NULL;
  if (Core_String_printf(&information, format,
                         renderer,
                         vendor,
                         major_version,
                         minor_version,
                         vsync_enabled ? 1 : 0)) {
    CORE_UNREFERENCE(format);
    format = NULL;
    CORE_UNREFERENCE(vendor);
    vendor = NULL;
    CORE_UNREFERENCE(renderer);
    renderer = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  CORE_UNREFERENCE(vendor);
  vendor = NULL;
  CORE_UNREFERENCE(renderer);
  renderer = NULL;
  *RETURN = information;
  return Core_Success;
}

Core_defineObjectType("dx.val.gl.context",
                      dx_val_gl_context,
                      Core_Visuals_Context);

static Core_Result enter_frame(dx_val_gl_context* SELF) {
  SELF->glDepthMask(SELF->depth_write_enabled ? GL_TRUE : GL_FALSE);
  return Core_Success;
}

static Core_Result bind_texture(dx_val_gl_context* ctx, Core_Size unit, dx_val_gl_texture* texture) {
  if (texture) {
    if (dx_val_gl_texture_upload(texture)) {
      return Core_Failure;
    }
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, texture->id);
  } else {
    ctx->glActiveTexture(GL_TEXTURE0 + unit);
    ctx->glBindTexture(GL_TEXTURE_2D, 0);
  }
  return Core_Success;
}

static Core_Result create_buffer(dx_val_gl_buffer** RETURN, dx_val_gl_context* SELF) {
  return dx_val_gl_buffer_create(RETURN, SELF);
}

static Core_Result create_cbinding(dx_val_cbinding** RETURN, dx_val_gl_context* SELF) {
  return dx_val_cbinding_create(RETURN);
}

static Core_Result create_vbinding(dx_val_gl_vbinding** RETURN, dx_val_gl_context* SELF, Core_VertexFormat vertex_format, dx_val_gl_buffer* buffer) {
  return dx_val_gl_vbinding_create(RETURN, vertex_format, buffer);
}

static Core_Result create_program(dx_val_gl_program** RETURN, dx_val_gl_context* SELF, dx_val_program_text* program_text) {
  if (!program_text || DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT != program_text->type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_val_gl_program_create(RETURN, SELF, program_text->vertex_program_text->program_text, program_text->fragment_program_text->program_text);
}

static Core_Result create_texture(dx_val_gl_texture** RETURN, dx_val_gl_context* SELF) {
  return dx_val_gl_texture_create(RETURN, SELF);
}

static Core_Result execute_commands(dx_val_gl_context* SELF, dx_val_command_list* commands) {
  for (Core_Size i = 0, n = dx_val_command_list_get_size(commands); i < n; ++i) {
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
        return Core_Failure;
      }
      // assign texture to texture unit
      if (command->draw_command.texture) {
        Core_Visuals_Context_bindTexture(CORE_VISUALS_CONTEXT(SELF), 0, command->draw_command.texture);
      } else {
        Core_Visuals_Context_bindTexture(CORE_VISUALS_CONTEXT(SELF), 0, NULL);
      }
      // activate binding and activate program, then render.
      if (dx_val_program_activate(command->draw_command.program)) {
        return Core_Failure;
      }
      if (dx_val_vbinding_activate(command->draw_command.vbinding)) {
        return Core_Failure;
      }
      SELF->glDrawArrays(GL_TRIANGLES, command->draw_command.start, command->draw_command.length);
    } break;
    case DX_VAL_COMMAND_KIND_VIEWPORT: {
      SELF->glViewport(command->viewport_command.l,
                       command->viewport_command.b,
                       command->viewport_command.w,
                       command->viewport_command.h);
    } break;
    case DX_VAL_COMMAND_KIND_SCISSOR_TEST: {
      if (command->scissor_test.enabled) {
        SELF->glEnable(GL_SCISSOR_TEST);
        SELF->glScissor(command->scissor_test.rectangle.l,
                        command->scissor_test.rectangle.b,
                        command->scissor_test.rectangle.w,
                        command->scissor_test.rectangle.h);
      } else {
        SELF->glDisable(GL_SCISSOR_TEST);
      }
    } break;
    case DX_VAL_COMMAND_KIND_PIPELINE_STATE: {
      SELF->glEnable(GL_BLEND);
      SELF->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      SELF->glEnable(GL_DEPTH_TEST);
      switch (command->pipeline_state.depthCompareMode) {
        case Core_DepthCompareMode_Always: {
          SELF->glDepthFunc(GL_ALWAYS);
        } break;
        case Core_DepthCompareMode_Never: {
          SELF->glDepthFunc(GL_NEVER);
        } break;
        case Core_DepthCompareMode_LessThan: {
          SELF->glDepthFunc(GL_LESS);
        } break;
        case Core_DepthCompareMode_LessThanOrEqualTo: {
          SELF->glDepthFunc(GL_LEQUAL);
        } break;
        case Core_DepthCompareMode_GreaterThan: {
          SELF->glDepthFunc(GL_GREATER);
        } break;
        case Core_DepthCompareMode_GreaterThanOrEqualTo: {
          SELF->glDepthFunc(GL_GEQUAL);
        } break;
        case Core_DepthCompareMode_Equal: {
          SELF->glDepthFunc(GL_EQUAL);
        } break;
        case Core_DepthCompareMode_NotEqual: {
          SELF->glDepthFunc(GL_NOTEQUAL);
        } break;
        default: {
          Core_setError(Core_Error_ArgumentInvalid);
          return Core_Failure;
        } break;
      };
      SELF->glDepthMask(command->pipeline_state.depthWriteEnabled ? GL_TRUE : GL_FALSE);
      switch (command->pipeline_state.cullMode) {
        case Core_CullMode_None: {
          SELF->glDisable(GL_CULL_FACE);
        } break;
        case Core_CullMode_Back: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_BACK);
        } break;
        case Core_CullMode_Front: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_FRONT);
        } break;
        case  Core_CullMode_FrontAndBack: {
          SELF->glEnable(GL_CULL_FACE);
          SELF->glCullFace(GL_FRONT_AND_BACK);
        } break;
        default: {
          Core_setError(Core_Error_ArgumentInvalid);
          return Core_Failure;
        } break;
      };
    } break;
    case DX_VAL_COMMAND_KIND_IDLE: {
      // Do nothing.
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
    }
  }
  return Core_Success;
}

static void dx_val_gl_context_destruct(dx_val_gl_context* SELF) {
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/functions.i"
#undef DEFINE
}

static void dx_val_gl_context_constructDispatch(dx_val_gl_context_Dispatch* SELF) {
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->getInformation = (Core_Result (*)(Core_String**, Core_Visuals_Context*)) & get_information;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->bindTexture = (Core_Result(*)(Core_Visuals_Context*, Core_Size, Core_Visuals_Texture*)) & bind_texture;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->createBuffer = (Core_Result (*)(dx_val_buffer**, Core_Visuals_Context*)) & create_buffer;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->createConstantBinding = (Core_Result(*)(dx_val_cbinding**, Core_Visuals_Context*)) & create_cbinding;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->createVariableBinding = (Core_Result (*)(dx_val_vbinding**, Core_Visuals_Context*, Core_VertexFormat, dx_val_buffer*)) & create_vbinding;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->createProgram = (Core_Result (*)(dx_val_program**, Core_Visuals_Context*, dx_val_program_text*)) & create_program;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->createTexture = (Core_Result (*)(Core_Visuals_Texture**, Core_Visuals_Context*)) & create_texture;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->executeCommands = (Core_Result(*)(Core_Visuals_Context*, dx_val_command_list*)) & execute_commands;
  CORE_VISUALS_CONTEXT_DISPATCH(SELF)->enterFrame = (Core_Result(*)(Core_Visuals_Context*)) & enter_frame;
}

Core_Result dx_val_gl_context_construct(dx_val_gl_context* SELF, Core_Result(*link)(void** RETURN, char const* name)) {
  Core_BeginConstructor(dx_val_gl_context);
  if (Core_Visuals_Context_construct(CORE_VISUALS_CONTEXT(SELF))) {
    return Core_Failure;
  }
#define DEFINE(TYPE, NAME, EXTENSION_NAME) (SELF)->NAME = NULL;
#include "dx/val/gl/functions.i"
#undef DEFINE
#define DEFINE(TYPE, NAME, EXTENSION_NAME) \
  if (link((void**)&((SELF)->NAME), #NAME)) { \
    dx_log("unable to link " #NAME "\n", sizeof("unable to link " #NAME "\n") - 1); \
    return Core_Failure; \
  }
#include "dx/val/gl/functions.i"
#undef DEFINE
  SELF->depth_write_enabled = Core_True;
  Core_EndConstructor(dx_val_gl_context);
}
