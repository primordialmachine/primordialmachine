#include "dx/val/gl/texture.h"

#include "dx/val/gl/context.h"
#include "dx/assets.h"

Core_defineObjectType("dx.val.gl.texture",
                      dx_val_gl_texture,
                      Core_Visuals_Texture);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_data(dx_val_gl_texture* SELF, Core_Assets_Texture* texture) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(CORE_VISUALS_TEXTURE(SELF)->context);
  switch (CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.pixelFormat) {
  case Core_PixelFormat_L8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RED,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.width,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.height,
                          0,
                          GL_RED,
                          GL_UNSIGNED_BYTE,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.pixels);
  } break;
  case Core_PixelFormat_Rgb8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.width,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.height,
                          0,
                          GL_RGB,
                          GL_UNSIGNED_BYTE,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.pixels);
  } break;
  case Core_PixelFormat_Bgr8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, SELF->id);
    context->glTexImage2D(GL_TEXTURE_2D,
                          0,
                          GL_RGB,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.width,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.extend.height, 0,
                          GL_BGR,
                          GL_UNSIGNED_BYTE,
                          CORE_ASSETS_IMAGE(texture->image_reference->object)->backing.pixels);
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

static Core_Result dx_val_gl_texture_set_texture_border_color(dx_val_gl_texture* SELF, Core_InlineRgbaR32 const* texture_border_color) {
  if (!Core_InlineRgbaR32_areEqual(&SELF->texture_border_color, texture_border_color)) {
    SELF->texture_border_color = *texture_border_color;
    SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_border_color(Core_InlineRgbaR32* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_border_color;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_minification_filter(dx_val_gl_texture* SELF, Core_TextureFilter texture_minification_filter) {
  if (SELF->texture_minification_filter != texture_minification_filter) {
    SELF->texture_minification_filter = texture_minification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_minification_filter(Core_TextureFilter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_minification_filter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result dx_val_gl_texture_set_texture_magnification_filter(dx_val_gl_texture* SELF, Core_TextureFilter texture_magnification_filter) {
  if (SELF->texture_magnification_filter != texture_magnification_filter) {
    SELF->texture_magnification_filter = texture_magnification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_texture_get_texture_magnification_filter(Core_TextureFilter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_magnification_filter;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result upload(dx_val_gl_texture* SELF) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(CORE_VISUALS_TEXTURE(SELF)->context);
  
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
      case Core_TextureFilter_Linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      } break;
      case Core_TextureFilter_Nearest: {
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
      case Core_TextureFilter_Linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      } break;
      case Core_TextureFilter_Nearest: {
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
    dx_val_gl_context* context = DX_VAL_GL_CONTEXT(CORE_VISUALS_TEXTURE(SELF)->context);
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_val_gl_texture_constructDispatch(dx_val_gl_texture_Dispatch* self) {
  CORE_VISUALS_TEXTURE_DISPATCH(self)->setData = (Core_Result(*)(Core_Visuals_Texture*, Core_Assets_Texture*)) & dx_val_gl_texture_set_data;

  CORE_VISUALS_TEXTURE_DISPATCH(self)->setAddressModeU = (Core_Result(*)(Core_Visuals_Texture*, Core_TextureAddressMode)) & dx_val_gl_texture_set_texture_address_mode_u;
  CORE_VISUALS_TEXTURE_DISPATCH(self)->getAddressModeU = (Core_Result(*)(Core_TextureAddressMode*, Core_Visuals_Texture*)) & dx_val_gl_texture_get_texture_address_mode_u;

  CORE_VISUALS_TEXTURE_DISPATCH(self)->setAddressModeV = (Core_Result(*)(Core_Visuals_Texture*, Core_TextureAddressMode)) & dx_val_gl_texture_set_texture_address_mode_v;
  CORE_VISUALS_TEXTURE_DISPATCH(self)->getAddressModeV = (Core_Result(*)(Core_TextureAddressMode*, Core_Visuals_Texture*)) & dx_val_gl_texture_get_texture_address_mode_v;

  CORE_VISUALS_TEXTURE_DISPATCH(self)->setMinificationFilter = (Core_Result(*)(Core_Visuals_Texture*, Core_TextureFilter)) & dx_val_gl_texture_set_texture_minification_filter;
  CORE_VISUALS_TEXTURE_DISPATCH(self)->getMinificationFilter = (Core_Result(*)(Core_TextureFilter*, Core_Visuals_Texture*)) & dx_val_gl_texture_get_texture_minification_filter;

  CORE_VISUALS_TEXTURE_DISPATCH(self)->setMagnificationFilter = (Core_Result(*)(Core_Visuals_Texture*, Core_TextureFilter)) & dx_val_gl_texture_set_texture_magnification_filter;
  CORE_VISUALS_TEXTURE_DISPATCH(self)->getMagnificationFilter = (Core_Result(*)(Core_TextureFilter*, Core_Visuals_Texture*)) & dx_val_gl_texture_get_texture_magnification_filter;

  CORE_VISUALS_TEXTURE_DISPATCH(self)->setBorderColor = (Core_Result(*)(Core_Visuals_Texture*, Core_InlineRgbaR32 const*)) & dx_val_gl_texture_set_texture_border_color;
  CORE_VISUALS_TEXTURE_DISPATCH(self)->getBorderColor = (Core_Result(*)(Core_InlineRgbaR32*, Core_Visuals_Texture*)) & dx_val_gl_texture_get_texture_border_color;
}

static Core_Result fill_amber(Core_Assets_Image* image_value) {
  Core_Assets_ColorRgbN8* color_value = NULL;
  if (Core_Assets_ColorRgbN8_create(&color_value, &dx_colors_amber)) {
    return Core_Failure;
  }
  Core_Assets_ImageOperations_ColorFill* image_operation_value = NULL;
  if (Core_Assets_ImageOperations_ColorFill_create(&image_operation_value)) {
    CORE_UNREFERENCE(color_value);
    color_value = NULL;
    return Core_Failure;
  }
  if (Core_Assets_ImageOperations_ColorFill_set_color(image_operation_value, color_value)) {
    CORE_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    CORE_UNREFERENCE(color_value);
    color_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(color_value);
  color_value = NULL;
  if (Core_Assets_Image_apply(image_value, 0, 0, image_value->backing.extend.width, image_value->backing.extend.height, CORE_ASSETS_IMAGEOPERATION(image_operation_value))) {
    CORE_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(image_operation_value);
  image_operation_value = NULL;
  return Core_Success;
}

Core_Result dx_val_gl_texture_construct(dx_val_gl_texture* SELF, dx_val_gl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_texture);
  if (Core_Visuals_Texture_construct(CORE_VISUALS_TEXTURE(SELF), CORE_VISUALS_CONTEXT(context))) {
    return Core_Failure;
  }

  SELF->flags = 0;

  SELF->texture_address_mode_u = Core_TextureAddressMode_Repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;

  SELF->texture_address_mode_v = Core_TextureAddressMode_Repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;

  SELF->texture_border_color = (Core_InlineRgbaR32){ .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f, };
  SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;

  SELF->texture_minification_filter = Core_TextureFilter_Linear;
  SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;

  SELF->texture_magnification_filter = Core_TextureFilter_Linear;
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
  Core_Assets_Image* image = NULL;
  if (Core_Assets_Image_create(&image, name, Core_PixelFormat_Rgb8, 8, 8)) {
    CORE_UNREFERENCE(name);
    name = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  CORE_UNREFERENCE(name);
  name = NULL;
  if (fill_amber(image)) {
    CORE_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  switch (image->backing.pixelFormat) {
  case Core_PixelFormat_Rgb8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->backing.extend.width, image->backing.extend.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->backing.pixels);
  } break;
  default: {
    Core_setError(Core_Error_EnvironmentFailed);
    CORE_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  } break;
  }
  CORE_UNREFERENCE(image);
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
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_gl_texture_upload(dx_val_gl_texture* SELF) {
  return upload(SELF);
}
