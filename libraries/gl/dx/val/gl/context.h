#if !defined(DX_VAL_GL_CONTEXT_H_INCLUDED)
#define DX_VAL_GL_CONTEXT_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/context.h"
#include <GL/glext.h>

typedef void (APIENTRY *PFNGLGETINTEGERVPROC)(GLenum, GLint*);
typedef const GLubyte * (APIENTRY *PFNGLGETSTRINGPROC)(GLenum);
typedef void (APIENTRY *PFNGLVIEWPORTPROC)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY *PFNGLCLEARPROC)(GLbitfield);
typedef void (APIENTRY *PFNGLCLEARCOLORPROC)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (APIENTRY *PFNGLCLEARDEPTHPROC)(GLdouble);
typedef GLenum (APIENTRY *PFNGLGETERRORPROC)();
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);
typedef void (APIENTRY *PFNGLSCISSORPROC)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei, GLuint*);
typedef void (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint*);
typedef void (APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum, GLenum, GLint);
typedef void (APIENTRY *PFNGLTEXPARAMETERFVPROC)(GLenum, GLenum, const GLfloat*);
typedef void (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef void (APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);

typedef void (APIENTRY *PFNGLPIXELSTOREIPROC)(GLenum, GLint);
typedef void (APIENTRY *PFNGLENABLEPROC)(GLenum);
typedef void (APIENTRY *PFNGLDISABLEPROC)(GLenum);
typedef void (APIENTRY *PFNGLDEPTHMASKPROC)(GLboolean);
typedef void (APIENTRY *PFNGLDEPTHFUNCPROC)(GLenum);
typedef void (APIENTRY *PFNGLCULLFACEPROC)(GLenum);
typedef void (APIENTRY* PFNGLBLENDFUNCPROC)(GLenum, GLenum);
typedef void (APIENTRY *PFNGLFLUSHPROC)();

Core_declareObjectType("dx.val.gl.context",
                       dx_val_gl_context,
                       dx_val_context);

static inline dx_val_gl_context* DX_VAL_GL_CONTEXT(void* p) {
  return (dx_val_gl_context*)p;
}

struct dx_val_gl_context {
  dx_val_context _parent;
  // remember as this is part of the command-modifiable state and must be restored when modified during the execution of certain commands (e.g., clear depth).
  Core_Boolean depth_write_enabled;
#define DEFINE(TYPE, NAME, EXTENSION_NAME) TYPE NAME;
#include "dx/val/gl/functions.i"
#undef DEFINE
};

struct dx_val_gl_context_Dispatch {
  dx_val_context_Dispatch _parent;
};

Core_Result dx_val_gl_context_construct(dx_val_gl_context* SELF, Core_Result (*link)(void** RETURN, char const* name));

#endif // DX_VAL_GL_CONTEXT_H_INCLUDED
