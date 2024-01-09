#if !defined(DX_VAL_GL_PROGRAM_H_INCLUDED)
#define DX_VAL_GL_PROGRAM_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/program.h"
typedef struct dx_val_gl_context dx_val_gl_context;

Core_declareObjectType("dx.val.gl.program",
                       dx_val_gl_program,
                       dx_val_program);

static inline dx_val_gl_program* DX_VAL_GL_PROGRAM(void* p) {
  return (dx_val_gl_program*)p;
}

struct dx_val_gl_program {
  dx_val_program _parent;
  GLuint vertex_program_id;
  GLuint fragment_program_id;
  GLuint program_id;
};

static inline dx_val_gl_program_Dispatch* DX_VAL_GL_PROGRAM_DISPATCH(void* p) {
  return (dx_val_gl_program_Dispatch*)p;
}

struct dx_val_gl_program_Dispatch {
  dx_val_program _parent;
};

Core_Result dx_val_gl_program_create(dx_val_gl_program** RETURN,
                                   dx_val_gl_context* ctx,
                                   Core_String* vertex_program_text,
                                   Core_String* fragment_program_text);

#endif // DX_VAL_GL_PROGRAM_H_INCLUDED
