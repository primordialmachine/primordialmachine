#include "dx/val/gl/wgl/system_factory.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

DX_DEFINE_OBJECT_TYPE("dx.val.gl.wgl.system_factory",
                      dx_val_gl_wgl_system_factory,
                      dx_val_gl_system_factory);

static dx_result create_system(dx_val_gl_wgl_system** RETURN, dx_val_gl_wgl_system_factory* SELF, dx_msg_queue* msg_queue);

static dx_result create_system(dx_val_gl_wgl_system** RETURN, dx_val_gl_wgl_system_factory* SELF, dx_msg_queue* msg_queue) {
  return dx_val_gl_wgl_system_create(RETURN, msg_queue);
}

static void dx_val_gl_wgl_system_factory_destruct(dx_val_gl_wgl_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_wgl_system_factory_dispatch_construct(dx_val_gl_wgl_system_factory_dispatch* SELF) {
  DX_SYSTEM_FACTORY_DISPATCH(SELF)->create_system = (dx_result(*)(dx_system**, dx_system_factory*, dx_msg_queue*)) & create_system;
}

dx_result dx_val_gl_wgl_system_factory_construct(dx_val_gl_wgl_system_factory* SELF) {
  dx_rti_type* TYPE = dx_val_gl_wgl_system_factory_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_gl_system_factory_construct(DX_VAL_GL_SYSTEM_FACTORY(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_gl_wgl_system_factory_create(dx_val_gl_wgl_system_factory** RETURN) {
  DX_CREATE_PREFIX(dx_val_gl_wgl_system_factory)
  if (dx_val_gl_wgl_system_factory_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
