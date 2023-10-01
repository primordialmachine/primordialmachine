#if !defined(DX_VAL_GL_TEXTURE_H_INCLUDED)
#define DX_VAL_GL_TEXTURE_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/texture.h"
typedef struct dx_val_gl_context dx_val_gl_context;

DX_DECLARE_OBJECT_TYPE("dx.val.gl.texture",
                       dx_val_gl_texture,
                       dx_val_texture)

static inline dx_val_gl_texture* DX_VAL_GL_TEXTURE(void* p) {
  return (dx_val_gl_texture*)p;
}

#define DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY (1<<0)
#define DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY (1<<1)
#define DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY (1<<2)
#define DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY (1<<3)
#define DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY (1<<4)

struct dx_val_gl_texture {
  dx_val_texture _parent;
  GLuint id;
  uint8_t flags;
  dx_texture_magnification_filter texture_magnification_filter;
  dx_texture_minification_filter texture_minification_filter;
  dx_texture_address_mode texture_address_mode_u;
  dx_texture_address_mode texture_address_mode_v;
  /// @todo This should be DX_RGBA_F32.
  DX_VEC4 texture_border_color;
};

static inline dx_val_gl_texture_dispatch* DX_VAL_GL_TEXTURE_DISPATCH(void* p) {
  return (dx_val_gl_texture_dispatch*)p;
}

struct dx_val_gl_texture_dispatch {
  dx_val_texture_dispatch _parent;
};

/// @method-call
dx_result dx_val_gl_texture_construct(dx_val_gl_texture* SELF, dx_val_gl_context* context);

/// @method-call
dx_result dx_val_gl_texture_create(dx_val_gl_texture** RETURN, dx_val_gl_context* context);

dx_result dx_val_gl_texture_upload(dx_val_gl_texture* SELF);

#endif // DX_VAL_GL_TEXTURE_H_INCLUDED
