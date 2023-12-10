#include "dx/val/gl/texture.h"

#include "dx/val/gl/context.h"
#include "dx/assets.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.texture",
                      dx_val_gl_texture,
                      dx_val_texture);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_data(dx_val_gl_texture* SELF, dx_assets_texture* texture) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
  switch (DX_ASSETS_IMAGE(texture->image_reference->object)->pixel_format) {
  case Core_PixelFormat_L8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RED,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->width,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->height,
                          0,
                          GL_RED,
                          GL_UNSIGNED_BYTE,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->pixels);
  } break;
  case Core_PixelFormat_Rgb8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->width,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->height,
                          0,
                          GL_RGB,
                          GL_UNSIGNED_BYTE,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->pixels);
  } break;
  case Core_PixelFormat_Bgr8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->width,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->height, 0,
                          GL_BGR,
                          GL_UNSIGNED_BYTE,
                          DX_ASSETS_IMAGE(texture->image_reference->object)->pixels);
  } break;
  default: {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  } break;
  };
  if (context->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_address_mode_u(dx_val_gl_texture* SELF, Core_TextureAddressMode texture_address_mode_u) {
  if (SELF->texture_address_mode_u != texture_address_mode_u) {
    SELF->texture_address_mode_u = texture_address_mode_u;
    SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_address_mode_u(Core_TextureAddressMode* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_address_mode_u;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_address_mode_v(dx_val_gl_texture* SELF, Core_TextureAddressMode texture_address_mode_v) {
  if (SELF->texture_address_mode_v != texture_address_mode_v) {
    SELF->texture_address_mode_v = texture_address_mode_v;
    SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_address_mode_v(Core_TextureAddressMode* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_address_mode_v;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_border_color(dx_val_gl_texture* SELF, DX_VEC4 const* texture_border_color) {
  if (!dx_vec4_are_equal(&SELF->texture_border_color, texture_border_color)) {
    SELF->texture_border_color = *texture_border_color;
    SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_border_color(DX_VEC4* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_border_color;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_minification_filter(dx_val_gl_texture* SELF, Core_TextureMinificationFilter texture_minification_filter) {
  if (SELF->texture_minification_filter != texture_minification_filter) {
    SELF->texture_minification_filter = texture_minification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_minification_filter(Core_TextureMinificationFilter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_minification_filter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_magnification_filter(dx_val_gl_texture* SELF, Core_TextureMagnificationFilter texture_magnification_filter) {
  if (SELF->texture_magnification_filter != texture_magnification_filter) {
    SELF->texture_magnification_filter = texture_magnification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_magnification_filter(Core_TextureMagnificationFilter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_magnification_filter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result upload(dx_val_gl_texture* SELF) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
  
  // bind the texture
  context->glBindTexture(GL_TEXTURE_2D, SELF->id);

  if (SELF->flags & DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY) {

    switch (SELF->texture_address_mode_u) {
      case Core_TextureAddressMode_Repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      } break;
      case Core_TextureAddressMode_MirroredRepeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      } break;
      case Core_TextureAddressMode_ClampToEdge: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      } break;
      case Core_TextureAddressMode_ClampToBorder: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      } break;
      default: {
        Core_setError(Core_Error_EnvironmentFailed);
        return Core_Failure;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY) {

    switch (SELF->texture_address_mode_v) {
      case Core_TextureAddressMode_Repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      } break;
      case Core_TextureAddressMode_MirroredRepeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      } break;
      case Core_TextureAddressMode_ClampToEdge: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      } break;
      case Core_TextureAddressMode_ClampToBorder: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      } break;
      default: {
        Core_setError(Core_Error_EnvironmentFailed);
        return Core_Failure;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY) {

    switch (SELF->texture_minification_filter) {
      case Core_TextureMinificationFilter_Linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      } break;
      case Core_TextureMinificationFilter_Nearest: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      } break;
      default: {
        Core_setError(Core_Error_EnvironmentFailed);
        return Core_Failure;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY) {

    switch (SELF->texture_magnification_filter) {
      case Core_TextureMagnificationFilter_Linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      } break;
      case Core_TextureMagnificationFilter_Nearest: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      } break;
      default: {
        Core_setError(Core_Error_EnvironmentFailed);
        return Core_Failure;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY) {

    context->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat const*)&SELF->texture_border_color);

    if (GL_NO_ERROR != context->glGetError()) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;
  }

  return Core_Success;
}

static void dx_val_gl_texture_destruct(dx_val_gl_texture* SELF) {
  if (SELF->id) {
    dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_val_gl_texture_constructDispatch(dx_val_gl_texture_dispatch* self) {
  DX_VAL_TEXTURE_DISPATCH(self)->set_data = (Core_Result(*)(dx_val_texture*, dx_assets_texture*)) & dx_val_gl_texture_set_data;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_address_mode_u = (Core_Result(*)(dx_val_texture*, Core_TextureAddressMode)) & dx_val_gl_texture_set_texture_address_mode_u;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_address_mode_u = (Core_Result(*)(Core_TextureAddressMode*, dx_val_texture*)) & dx_val_gl_texture_get_texture_address_mode_u;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_address_mode_v = (Core_Result(*)(dx_val_texture*, Core_TextureAddressMode)) & dx_val_gl_texture_set_texture_address_mode_v;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_address_mode_v = (Core_Result(*)(Core_TextureAddressMode*, dx_val_texture*)) & dx_val_gl_texture_get_texture_address_mode_v;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_minification_filter = (Core_Result(*)(dx_val_texture*, Core_TextureMinificationFilter)) & dx_val_gl_texture_set_texture_minification_filter;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_minification_filter = (Core_Result(*)(Core_TextureMinificationFilter*, dx_val_texture*)) & dx_val_gl_texture_get_texture_minification_filter;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_magnification_filter = (Core_Result(*)(dx_val_texture*, Core_TextureMagnificationFilter)) & dx_val_gl_texture_set_texture_magnification_filter;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_magnification_filter = (Core_Result(*)(Core_TextureMagnificationFilter*, dx_val_texture*)) & dx_val_gl_texture_get_texture_magnification_filter;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_border_color = (Core_Result(*)(dx_val_texture*, DX_VEC4 const*)) & dx_val_gl_texture_set_texture_border_color;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_border_color = (Core_Result(*)(DX_VEC4*, dx_val_texture*)) & dx_val_gl_texture_get_texture_border_color;
}

static Core_Result fill_amber(dx_assets_image* image_value) {
  dx_assets_color_rgb_n8* color_value = NULL;
  if (dx_assets_color_rgb_n8_create(&color_value, &dx_colors_amber)) {
    return Core_Failure;
  }
  dx_assets_image_operations_color_fill* image_operation_value = NULL;
  if (dx_assets_image_operations_color_fill_create(&image_operation_value)) {
    DX_UNREFERENCE(color_value);
    color_value = NULL;
    return Core_Failure;
  }
  if (dx_assets_image_operations_color_fill_set_color(image_operation_value, color_value)) {
    DX_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    DX_UNREFERENCE(color_value);
    color_value = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(color_value);
  color_value = NULL;
  if (dx_assets_image_apply(image_value, 0, 0, image_value->width, image_value->height, DX_ASSETS_IMAGE_OPERATION(image_operation_value))) {
    DX_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(image_operation_value);
  image_operation_value = NULL;
  return Core_Success;
}

Core_Result dx_val_gl_texture_construct(dx_val_gl_texture* SELF, dx_val_gl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_texture);
  if (dx_val_texture_construct(DX_VAL_TEXTURE(SELF), DX_VAL_CONTEXT(context))) {
    return Core_Failure;
  }

  SELF->flags = 0;

  SELF->texture_address_mode_u = Core_TextureAddressMode_Repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;

  SELF->texture_address_mode_v = Core_TextureAddressMode_Repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;

  SELF->texture_border_color = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f, };
  SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;

  SELF->texture_minification_filter = Core_TextureMinificationFilter_Linear;
  SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;

  SELF->texture_magnification_filter = Core_TextureMagnificationFilter_Linear;
  SELF->flags |= DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;

  context->glGetError(); // clear the error variable
  context->glGenTextures(1, &SELF->id);
  if (GL_NO_ERROR != context->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  context->glBindTexture(GL_TEXTURE_2D, SELF->id);
  // Create the default 8x8 pixels amber texture.
  Core_String* name = NULL;
  if (Core_String_create(&name, "<default image>", sizeof("<default image>") - 1)) {
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  dx_assets_image* image = NULL;
  if (dx_assets_image_create(&image, name, Core_PixelFormat_Rgb8, 8, 8)) {
    DX_UNREFERENCE(name);
    name = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  DX_UNREFERENCE(name);
  name = NULL;
  if (fill_amber(image)) {
    DX_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  switch (image->pixel_format) {
  case Core_PixelFormat_Rgb8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  } break;
  default: {
    Core_setError(Core_Error_EnvironmentFailed);
    DX_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  } break;
  }
  DX_UNREFERENCE(image);
  image = NULL;

  if (GL_NO_ERROR != context->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }

  if (upload(SELF)) {
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;

  return Core_Success;
}

Core_Result dx_val_gl_texture_create(dx_val_gl_texture** RETURN, dx_val_gl_context* context) {
  DX_CREATE_PREFIX(dx_val_gl_texture);
  if (dx_val_gl_texture_construct(SELF, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_gl_texture_upload(dx_val_gl_texture* SELF) {
  return upload(SELF);
}
