/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ADL_TYPE_HANDLER_H_INCLUDED)
#define DX_ADL_TYPE_HANDLER_H_INCLUDED

#include "dx/adl/symbols.h"
#include "dx/data_definition_language.h"
typedef struct dx_adl_context dx_adl_context;

Core_declareObjectType("dx.adl.type_handler",
                       dx_adl_type_handler,
                       Core_Object);

static inline dx_adl_type_handler* DX_ADL_TYPE_HANDLER(void* p) {
  return (dx_adl_type_handler*)p;
}

struct dx_adl_type_handler {
  Core_Object _parent;
};

static inline dx_adl_type_handler_Dispatch* DX_ADL_TYPE_HANDLER_DISPATCH(void* p) {
  return (dx_adl_type_handler_Dispatch*)p;
}


struct dx_adl_type_handler_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*read)(Core_Object** RETURN, dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context);
  Core_Result(*enter)(dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context);
  Core_Result (*resolve)(dx_adl_type_handler* SELF, dx_adl_symbol* symbol, dx_adl_context* context);
};

Core_Result dx_adl_type_handler_construct(dx_adl_type_handler* SELF);

static inline Core_Result dx_adl_type_handler_read(Core_Object** RETURN, dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_adl_type_handler, read, RETURN, SELF, node, context);
}

static inline Core_Result dx_adl_type_handler_enter(dx_adl_type_handler* SELF, dx_ddl_node* node, dx_adl_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_adl_type_handler, enter, SELF, node, context);
}

static inline Core_Result dx_adl_type_handler_resolve(dx_adl_type_handler* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  DX_OBJECT_VIRTUALCALL(dx_adl_type_handler, resolve, SELF, symbol, context);
}

#endif // DX_ADL_TYPE_HANDLER_H_INCLUDED
