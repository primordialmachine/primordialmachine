#include "dx/val/gl/texture.h"

#include "dx/val/gl/context.h"
#include "dx/assets.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.texture",
                      dx_val_gl_texture,
                      dx_val_texture);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_data(dx_val_gl_texture* SELF, dx_assets_texture* texture) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
  switch (DX_ASSETS_IMAGE(texture->image_reference->object)->pixel_format) {
  case dx_pixel_format_ln8: {
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
  case dx_pixel_format_rn8_gn8_bn8: {
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
  case dx_pixel_format_bn8_gn8_rn8: {
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
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  } break;
  };
  if (context->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_texture_address_mode_u(dx_val_gl_texture* SELF, dx_texture_address_mode texture_address_mode_u) {
  if (SELF->texture_address_mode_u != texture_address_mode_u) {
    SELF->texture_address_mode_u = texture_address_mode_u;
    SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;
  }
  return DX_SUCCESS;
}

static dx_result dx_val_gl_texture_get_texture_address_mode_u(dx_texture_address_mode* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_address_mode_u;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_texture_address_mode_v(dx_val_gl_texture* SELF, dx_texture_address_mode texture_address_mode_v) {
  if (SELF->texture_address_mode_v != texture_address_mode_v) {
    SELF->texture_address_mode_v = texture_address_mode_v;
    SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;
  }
  return DX_SUCCESS;
}

static dx_result dx_val_gl_texture_get_texture_address_mode_v(dx_texture_address_mode* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_address_mode_v;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_texture_border_color(dx_val_gl_texture* SELF, DX_VEC4 const* texture_border_color) {
  if (!dx_vec4_are_equal(&SELF->texture_border_color, texture_border_color)) {
    SELF->texture_border_color = *texture_border_color;
    SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;
  }
  return DX_SUCCESS;
}

static dx_result dx_val_gl_texture_get_texture_border_color(DX_VEC4* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_border_color;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_texture_minification_filter(dx_val_gl_texture* SELF, dx_texture_minification_filter texture_minification_filter) {
  if (SELF->texture_minification_filter != texture_minification_filter) {
    SELF->texture_minification_filter = texture_minification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;
  }
  return DX_SUCCESS;
}

static dx_result dx_val_gl_texture_get_texture_minification_filter(dx_texture_minification_filter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_minification_filter;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result dx_val_gl_texture_set_texture_magnification_filter(dx_val_gl_texture* SELF, dx_texture_magnification_filter texture_magnification_filter) {
  if (SELF->texture_magnification_filter != texture_magnification_filter) {
    SELF->texture_magnification_filter = texture_magnification_filter;
    SELF->flags |= DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;
  }
  return DX_SUCCESS;
}

static dx_result dx_val_gl_texture_get_texture_magnification_filter(dx_texture_magnification_filter* RETURN, dx_val_gl_texture* SELF) {
  *RETURN = SELF->texture_magnification_filter;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result upload(dx_val_gl_texture* SELF) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
  
  // bind the texture
  context->glBindTexture(GL_TEXTURE_2D, SELF->id);

  if (SELF->flags & DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY) {

    switch (SELF->texture_address_mode_u) {
      case dx_texture_address_mode_repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      } break;
      case dx_texture_address_mode_mirrored_repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      } break;
      case dx_texture_address_mode_clamp_to_border: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      } break;
      default: {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
        return DX_FAILURE;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY) {

    switch (SELF->texture_address_mode_v) {
      case dx_texture_address_mode_repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      } break;
      case dx_texture_address_mode_mirrored_repeat: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      } break;
      case dx_texture_address_mode_clamp_to_border: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      } break;
      default: {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
        return DX_FAILURE;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY) {

    switch (SELF->texture_minification_filter) {
      case dx_texture_minification_filter_linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      } break;
      case dx_texture_minification_filter_nearest: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      } break;
      default: {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
        return DX_FAILURE;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY) {

    switch (SELF->texture_magnification_filter) {
      case dx_texture_magnification_filter_linear: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      } break;
      case dx_texture_magnification_filter_nearest: {
        context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      } break;
      default: {
        dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
        return DX_FAILURE;
      } break;
    };

    if (GL_NO_ERROR != context->glGetError()) {
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;
  }

  if (SELF->flags & DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY) {

    context->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (GLfloat const*)&SELF->texture_border_color);

    if (GL_NO_ERROR != context->glGetError()) {
      dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
      return DX_FAILURE;
    }

    SELF->flags &= ~DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;
  }

  return DX_SUCCESS;
}

static void dx_val_gl_texture_destruct(dx_val_gl_texture* SELF) {
  if (SELF->id) {
    dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_TEXTURE(SELF)->context);
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_val_gl_texture_dispatch_construct(dx_val_gl_texture_dispatch* self) {
  DX_VAL_TEXTURE_DISPATCH(self)->set_data = (dx_result(*)(dx_val_texture*, dx_assets_texture*)) & dx_val_gl_texture_set_data;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_address_mode_u = (dx_result(*)(dx_val_texture*, dx_texture_address_mode)) & dx_val_gl_texture_set_texture_address_mode_u;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_address_mode_u = (dx_result(*)(dx_texture_address_mode*, dx_val_texture*)) & dx_val_gl_texture_get_texture_address_mode_u;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_address_mode_v = (dx_result(*)(dx_val_texture*, dx_texture_address_mode)) & dx_val_gl_texture_set_texture_address_mode_v;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_address_mode_v = (dx_result(*)(dx_texture_address_mode*, dx_val_texture*)) & dx_val_gl_texture_get_texture_address_mode_v;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_minification_filter = (dx_result(*)(dx_val_texture*, dx_texture_minification_filter)) & dx_val_gl_texture_set_texture_minification_filter;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_minification_filter = (dx_result(*)(dx_texture_minification_filter*, dx_val_texture*)) & dx_val_gl_texture_get_texture_minification_filter;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_magnification_filter = (dx_result(*)(dx_val_texture*, dx_texture_minification_filter)) & dx_val_gl_texture_set_texture_magnification_filter;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_magnification_filter = (dx_result(*)(dx_texture_minification_filter*, dx_val_texture*)) & dx_val_gl_texture_get_texture_magnification_filter;

  DX_VAL_TEXTURE_DISPATCH(self)->set_texture_border_color = (dx_result(*)(dx_val_texture*, DX_VEC4 const*)) & dx_val_gl_texture_set_texture_border_color;
  DX_VAL_TEXTURE_DISPATCH(self)->get_texture_border_color = (dx_result(*)(DX_VEC4*, dx_val_texture*)) & dx_val_gl_texture_get_texture_border_color;
}

static dx_result fill_amber(dx_assets_image* image_value) {
  dx_assets_color_rgb_n8* color_value = NULL;
  if (dx_assets_color_rgb_n8_create(&color_value, &dx_colors_amber)) {
    return DX_FAILURE;
  }
  dx_assets_image_operations_color_fill* image_operation_value = NULL;
  if (dx_assets_image_operations_color_fill_create(&image_operation_value)) {
    DX_UNREFERENCE(color_value);
    color_value = NULL;
    return DX_FAILURE;
  }
  if (dx_assets_image_operations_color_fill_set_color(image_operation_value, color_value)) {
    DX_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    DX_UNREFERENCE(color_value);
    color_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(color_value);
  color_value = NULL;
  if (dx_assets_image_apply(image_value, 0, 0, image_value->width, image_value->height, DX_ASSETS_IMAGE_OPERATION(image_operation_value))) {
    DX_UNREFERENCE(image_operation_value);
    image_operation_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(image_operation_value);
  image_operation_value = NULL;
  return DX_SUCCESS;
}

dx_result dx_val_gl_texture_construct(dx_val_gl_texture* SELF, dx_val_gl_context* context) {
  dx_rti_type* TYPE = dx_val_gl_texture_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_texture_construct(DX_VAL_TEXTURE(SELF), DX_VAL_CONTEXT(context))) {
    return DX_FAILURE;
  }

  SELF->flags = 0;

  SELF->texture_address_mode_u = dx_texture_address_mode_repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY;

  SELF->texture_address_mode_v = dx_texture_address_mode_repeat;
  SELF->flags |= DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY;

  SELF->texture_border_color = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f, };
  SELF->flags |= DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY;

  SELF->texture_minification_filter = dx_texture_minification_filter_linear;
  SELF->flags |= DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY;

  SELF->texture_magnification_filter = dx_texture_magnification_filter_linear;
  SELF->flags |= DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY;

  context->glGetError(); // clear the error variable
  context->glGenTextures(1, &SELF->id);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  context->glBindTexture(GL_TEXTURE_2D, SELF->id);
  // Create the default 8x8 pixels amber texture.
  dx_string* name = NULL;
  if (dx_string_create(&name, "<default image>", sizeof("<default image>") - 1)) {
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }
  dx_assets_image* image = NULL;
  if (dx_assets_image_create(&image, name, dx_pixel_format_rn8_gn8_bn8, 8, 8)) {
    DX_UNREFERENCE(name);
    name = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(name);
  name = NULL;
  if (fill_amber(image)) {
    DX_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }
  switch (image->pixel_format) {
  case dx_pixel_format_rn8_gn8_bn8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  } break;
  default: {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    DX_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  } break;
  }
  DX_UNREFERENCE(image);
  image = NULL;

  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }

  if (upload(SELF)) {
    context->glDeleteTextures(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;

  return DX_SUCCESS;
}

dx_result dx_val_gl_texture_create(dx_val_gl_texture** RETURN, dx_val_gl_context* context) {
  DX_CREATE_PREFIX(dx_val_gl_texture)
  if (dx_val_gl_texture_construct(SELF, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_gl_texture_upload(dx_val_gl_texture* SELF) {
  return upload(SELF);
}
