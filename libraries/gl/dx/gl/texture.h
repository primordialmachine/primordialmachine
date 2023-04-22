#if !defined(DX_GL_TEXTURE_H_INCLUDED)
#define DX_GL_TEXTURE_H_INCLUDED

#include "dx/val/texture.h"
#include "dx/gl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.gl.texture",
                       dx_gl_texture,
                       dx_val_texture)

static inline dx_gl_texture* DX_GL_TEXTURE(void* p) {
  return (dx_gl_texture*)p;
}

#define DX_GL_TEXTURE_MINIFICATION_FILTER_DIRTY (1<<0)
#define DX_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY (1<<1)
#define DX_GL_TEXTURE_ADDRESS_MODE_U_DIRTY (1<<2)
#define DX_GL_TEXTURE_ADDRESS_MODE_V_DIRTY (1<<3)
#define DX_GL_TEXTURE_BORDER_COLOR_DIRTY (1<<4)

struct dx_gl_texture {
  dx_val_texture _parent;
  GLuint id;
  uint8_t flags;
  DX_TEXTURE_MAGNIFICATION_FILTER texture_magnification_filter;
  DX_TEXTURE_MINIFICATION_FILTER texture_minification_filter;
  DX_TEXTURE_ADDRESS_MODE texture_address_mode_u;
  DX_TEXTURE_ADDRESS_MODE texture_address_mode_v;
  DX_VEC4 texture_border_color;
};

static inline dx_gl_texture_dispatch* DX_GL_TEXTURE_DISPATCH(void* p) {
  return (dx_gl_texture_dispatch*)p;
}

struct dx_gl_texture_dispatch {
  dx_val_texture_dispatch _parent;
};

/// @default-runtime-calling-convention
dx_result dx_gl_texture_construct(dx_gl_texture* SELF, dx_gl_context* context);

/// @default-runtime-calling-convention
dx_result dx_gl_texture_create(dx_gl_texture** RETURN, dx_gl_context* context);

dx_result dx_gl_texture_upload(dx_gl_texture* SELF);

#endif // DX_GL_TEXTURE_H_INCLUDED
