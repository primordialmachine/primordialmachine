#if !defined(DX_VAL_GL_TEXTURE_H_INCLUDED)
#define DX_VAL_GL_TEXTURE_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/texture.h"
typedef struct dx_val_gl_context dx_val_gl_context;

Core_declareObjectType("dx.val.gl.texture",
                       dx_val_gl_texture,
                       dx_val_texture);

static inline dx_val_gl_texture* DX_VAL_GL_TEXTURE(void* p) {
  return (dx_val_gl_texture*)p;
}

#define DX_VAL_GL_TEXTURE_MINIFICATION_FILTER_DIRTY (1<<0)
#define DX_VAL_GL_TEXTURE_MAGNIFICATION_FILTER_DIRTY (1<<1)
#define DX_VAL_GL_TEXTURE_ADDRESS_MODE_U_DIRTY (1<<2)
#define DX_VAL_GL_TEXTURE_ADDRESS_MODE_V_DIRTY (1<<3)
#define DX_VAL_GL_TEXTURE_BORDER_COLOR_DIRTY (1<<4)

struct dx_val_gl_texture {
  Core_Visuals_Texture _parent;
  GLuint id;
  uint8_t flags;
  Core_TextureFilter texture_magnification_filter;
  Core_TextureFilter texture_minification_filter;
  Core_TextureAddressMode texture_address_mode_u;
  Core_TextureAddressMode texture_address_mode_v;
  Core_InlineRgbaR32 texture_border_color;
};

static inline dx_val_gl_texture_Dispatch* DX_VAL_GL_TEXTURE_DISPATCH(void* p) {
  return (dx_val_gl_texture_Dispatch*)p;
}

struct dx_val_gl_texture_Dispatch {
  Core_Visuals_Texture_Dispatch _parent;
};

/// @method-call
Core_Result dx_val_gl_texture_construct(dx_val_gl_texture* SELF, dx_val_gl_context* context);

/// @method-call
Core_Result dx_val_gl_texture_create(dx_val_gl_texture** RETURN, dx_val_gl_context* context);

Core_Result dx_val_gl_texture_upload(dx_val_gl_texture* SELF);

#endif // DX_VAL_GL_TEXTURE_H_INCLUDED
