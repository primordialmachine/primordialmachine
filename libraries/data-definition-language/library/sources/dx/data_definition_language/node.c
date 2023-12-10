#include "dx/data_definition_language/node.h"

#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ddl.node",
                      dx_ddl_node,
                      Core_Object);

static void on_added(Core_Object** a);

static void on_removed(Core_Object** a);

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a);

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static void on_added(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_removed(Core_Object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static void dx_ddl_node_destruct(dx_ddl_node* self) {
  DX_DEBUG_ASSERT(NULL != self);
  switch (self->kind) {
    case dx_ddl_node_kind_error: {
    } break;
    case dx_ddl_node_kind_list: {
      dx_inline_pointer_array_uninitialize(&self->list);
    } break;
    case dx_ddl_node_kind_map: {
      dx_inline_pointer_hashmap_uninitialize(&self->map);
    } break;
    case dx_ddl_node_kind_number: {
      DX_DEBUG_ASSERT(NULL != self->number);
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->number);
      self->number = NULL;
    } break;
    case dx_ddl_node_kind_string: {
      DX_DEBUG_ASSERT(NULL != self->string);
      DX_UNREFERENCE(self->string);
      self->string = NULL;
    } break;
  };
}

static void dx_ddl_node_constructDispatch(dx_ddl_node_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_ddl_node_construct(dx_ddl_node* SELF, dx_ddl_node_kind kind) {
  DX_CONSTRUCT_PREFIX(dx_ddl_node);
  SELF->kind = kind;
  switch (SELF->kind) {
    case dx_ddl_node_kind_error: {
      /*Intentionally empty.*/
    } break;
    case dx_ddl_node_kind_list: {
      DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
        .added_callback = (void(*)(void*)) & on_added,
        .removed_callback = (void(*)(void*)) & on_removed,
      };
      if (dx_inline_pointer_array_initialize(&SELF->list, 0, &configuration)) {
        return Core_Failure;
      }
    } break;
    case dx_ddl_node_kind_map: {
      static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
        .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&on_compare_keys,
        .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&on_hash_key,
        .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&on_added,
        .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&on_removed,
        .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&on_added,
        .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&on_removed,
      };
      if (dx_inline_pointer_hashmap_initialize(&SELF->map, &configuration)) {
        return Core_Failure;
      }
    } break;
    case dx_ddl_node_kind_number: {
      if (Core_String_create(&SELF->number, "0", sizeof("0") - 1)) {
        return Core_Failure;
      }
    } break;
    case dx_ddl_node_kind_string: {
      if (Core_String_create(&SELF->string, "", sizeof("") - 1)) {
        return Core_Failure;
      }
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    } break;
  };
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ddl_node_create(dx_ddl_node** RETURN, dx_ddl_node_kind kind) {
  DX_CREATE_PREFIX(dx_ddl_node);
  if (dx_ddl_node_construct(SELF, kind)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_ddl_node_get_kind(dx_ddl_node_kind* RETURN, dx_ddl_node const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->kind;
  return Core_Success;
}

Core_Result dx_ddl_node_map_set(dx_ddl_node* SELF, Core_String* name, dx_ddl_node* value) {
  if (!SELF || !name || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_map) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  return dx_inline_pointer_hashmap_set(&SELF->map, name, value);
}

Core_Result dx_ddl_node_map_get(dx_ddl_node** RETURN, dx_ddl_node const* SELF, Core_String* name) {
  if (!RETURN || !SELF || !name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_map) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  dx_ddl_node* temporary = NULL;
  if (dx_inline_pointer_hashmap_get(&temporary, &SELF->map, name)) {
    return Core_Failure;
  }
  DX_REFERENCE(temporary);
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_ddl_node_map_get_size(Core_Size* RETURN, dx_ddl_node* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_map) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  return dx_inline_pointer_hashmap_get_size(RETURN, &SELF->map);
}

Core_Result dx_ddl_node_list_append(dx_ddl_node* SELF, dx_ddl_node* value) {
  if (!SELF || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  return dx_inline_pointer_array_append(&SELF->list, (void*)value);
}

Core_Result dx_ddl_node_list_prepend(dx_ddl_node* SELF, dx_ddl_node* value) {
  if (!SELF || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  return dx_inline_pointer_array_prepend(&SELF->list, (void*)value);
}

Core_Result dx_ddl_node_list_insert(dx_ddl_node* SELF, dx_ddl_node* value, Core_Size index) {
  if (!SELF || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  return dx_inline_pointer_array_insert(&SELF->list, (void*)value, index);
}

Core_Result dx_ddl_node_list_get(dx_ddl_node** RETURN, dx_ddl_node* SELF, Core_Size index) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  dx_ddl_node* temporary = NULL;
  if (dx_inline_pointer_array_get_at(&temporary, &SELF->list, index)) {
    return Core_Failure;
  }
  DX_REFERENCE(temporary);
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_ddl_node_list_get_size(Core_Size* RETURN, dx_ddl_node* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  Core_Size temporary;
  if (dx_inline_pointer_array_get_size(&temporary, &SELF->list)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_ddl_node_get_string(Core_String** RETURN, dx_ddl_node const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_string) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(SELF->string);
  *RETURN = SELF->string;
  return Core_Success;
}

Core_Result dx_ddl_node_set_string(dx_ddl_node* SELF, Core_String* string) {
  if (!SELF || !string) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_string) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(string);
  DX_UNREFERENCE(SELF->string);
  SELF->string = string;
  return Core_Success;
}

Core_Result dx_ddl_node_get_number(Core_String** RETURN, dx_ddl_node const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_number) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(SELF->number);
  *RETURN = SELF->number;
  return Core_Success;
}

Core_Result dx_ddl_node_set_number(dx_ddl_node* SELF, Core_String* number) {
  if (!SELF || !number) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->kind != dx_ddl_node_kind_number) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  DX_REFERENCE(number);
  DX_UNREFERENCE(SELF->number);
  SELF->number = number;
  return Core_Success;
}
