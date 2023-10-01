/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ADL_TYPE_HANDLER_H_INCLUDED)
#define DX_ADL_TYPE_HANDLER_H_INCLUDED

#include "dx/adl/symbols.h"
#include "dx/data_definition_language.h"
typedef struct dx_adl_context dx_adl_context;

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handler",
                       dx_adl_type_handler,
                       dx_object)

static inline dx_adl_type_handler* DX_ADL_TYPE_HANDLER(void* p) {
  return (dx_adl_type_handler*)p;
}

struct dx_adl_type_handler {
  dx_object _parent;
};

static inline dx_adl_type_handler_dispatch* DX_ADL_TYPE_HANDLER_DISPATCH(void* p) {
  return (dx_adl_type_handler_dispatch*)p;
}


struct dx_adl_type_handler_dispatch {
  dx_object_dispatch _parent;
  dx_result(*read)(dx_object** RETURN, dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context);
  dx_result (*resolve)(dx_adl_type_handler* SELF, dx_adl_symbol* symbol, dx_adl_context* context);
};

dx_result dx_adl_type_handler_construct(dx_adl_type_handler* SELF);

static inline dx_result dx_adl_type_handler_read(dx_object** RETURN, dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_adl_type_handler, read, RETURN, SELF, node, context);
}

static inline dx_result dx_adl_type_handler_resolve(dx_adl_type_handler* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_adl_type_handler, resolve, SELF, symbol, context);
}

#endif // DX_ADL_TYPE_HANDLER_H_INCLUDED
