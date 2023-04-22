#include "dx/adl/type_handler.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handler",
                      dx_adl_type_handler,
                      dx_object)

static void dx_adl_type_handler_destruct(dx_adl_type_handler* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handler_dispatch_construct(dx_adl_type_handler_dispatch* self)
{/*Intentionally empty.*/}

int dx_adl_type_handler_construct(dx_adl_type_handler* self) {
  dx_rti_type* _type = dx_adl_type_handler_get_type();
  if (!_type) {
    return 1;
  }
  self->resolve = NULL;
  self->read = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

int dx_adl_type_handler_resolve(dx_adl_type_handler* self,
                                dx_adl_symbol* symbol,
                                dx_adl_context* context)
{
  assert(NULL != self);
  assert(NULL != self->resolve);
  return self->resolve(self, symbol, context);
}

dx_object* dx_adl_type_handler_read(dx_adl_type_handler* self,
                                    dx_ddl_node* node,
                                    dx_adl_context* context)
{
  assert(NULL != self);
  assert(NULL != self->read);
  return self->read(self, node, context); 
}
