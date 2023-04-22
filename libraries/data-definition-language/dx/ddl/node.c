#include "dx/ddl/node.h"

#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ddl.node",
                      dx_ddl_node,
                      dx_object);

static void on_added(dx_object** a);

static void on_removed(dx_object** a);

static dx_result on_hash_key(dx_size* RETURN, dx_object** a);

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b);

static void on_added(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_REFERENCE(*a);
}

static void on_removed(dx_object** a) {
  DX_DEBUG_ASSERT(NULL != *a);
  DX_UNREFERENCE(*a);
}

static dx_result on_hash_key(dx_size* RETURN, dx_object** a) {
  *RETURN = dx_string_get_hash_value(DX_STRING(*a));
  return DX_SUCCESS;
}

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b) {
  *RETURN = dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
  return DX_SUCCESS;
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

static void dx_ddl_node_dispatch_construct(dx_ddl_node_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_ddl_node_construct(dx_ddl_node* SELF, dx_ddl_node_kind kind) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_ddl_node_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
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
        return DX_FAILURE;
      }
    } break;
    case dx_ddl_node_kind_map: {
      static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
        .compare_keys_callback = (dx_result(*)(dx_bool*,void**,void**)) &on_compare_keys,
        .hash_key_callback = (dx_result(*)(dx_size*,void**)) & on_hash_key,
        .key_added_callback = (void(*)(void**)) &on_added,
        .key_removed_callback = (void(*)(void**)) &on_removed,
        .value_added_callback = (void(*)(void**)) &on_added,
        .value_removed_callback = (void(*)(void**)) &on_removed,
      };
      if (dx_inline_pointer_hashmap_initialize(&SELF->map, &configuration)) {
        return DX_FAILURE;
      }
    } break;
    case dx_ddl_node_kind_number: {
      if (dx_string_create(&SELF->number, "0", sizeof("0") - 1)) {
        return DX_FAILURE;
      }
    } break;
    case dx_ddl_node_kind_string: {
      if (dx_string_create(&SELF->string, "", sizeof("") - 1)) {
        return DX_FAILURE;
      }
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    } break;
  };
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ddl_node_create(dx_ddl_node** RETURN, dx_ddl_node_kind kind) {
  dx_ddl_node* SELF = DX_DDL_NODE(dx_object_alloc(sizeof(dx_ddl_node)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_ddl_node_construct(SELF, kind)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_ddl_node_get_kind(dx_ddl_node_kind* RETURN, dx_ddl_node const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->kind;
  return DX_SUCCESS;
}

dx_result dx_ddl_node_map_set(dx_ddl_node* SELF, dx_string* name, dx_ddl_node* value) {
  if (!SELF || !name || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (SELF->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return DX_FAILURE;
  }
  return dx_inline_pointer_hashmap_set(&SELF->map, name, value);
}

dx_ddl_node* dx_ddl_node_map_get(dx_ddl_node const* self, dx_string* name) {
  if (!self || !name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return NULL;
  }
  dx_ddl_node* temporary = NULL;
  if (dx_inline_pointer_hashmap_get(&temporary, &self->map, name)) {
    return NULL;
  }
  return temporary;
}

int dx_ddl_node_list_append(dx_ddl_node* self, dx_ddl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 1;
  }
  return dx_inline_pointer_array_append(&self->list, (void*)value);
}

int dx_ddl_node_list_prepend(dx_ddl_node* self, dx_ddl_node* value) {
  if (!self || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 1;
  }
  return dx_inline_pointer_array_prepend(&self->list, (void*)value);
}

int dx_ddl_node_list_insert(dx_ddl_node* self, dx_ddl_node* value, dx_size index) {
  if (!self || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 1;
  }
  return dx_inline_pointer_array_insert(&self->list, (void*)value, index);
}

dx_ddl_node* dx_ddl_node_list_get(dx_ddl_node* self, dx_size index) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return NULL;
  }
  dx_ddl_node* temporary = NULL;
  if (dx_inline_pointer_array_get_at(&temporary, &self->list, index)) {
    return NULL;
  }
  return temporary;
}

dx_size dx_ddl_node_list_get_size(dx_ddl_node* self) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 0;
  }
  if (self->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 0;
  }
  dx_size temporary;
  if (dx_inline_pointer_array_get_size(&temporary, &self->list)) {
    return 0;
  }
  return temporary;
}

dx_string* dx_ddl_node_get_string(dx_ddl_node const* self) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_string) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return NULL;
  }
  DX_REFERENCE(self->string);
  return self->string;
}

int dx_ddl_node_set_string(dx_ddl_node* self, dx_string* string) {
  if (!self || !string) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_string) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(string);
  DX_UNREFERENCE(self->string);
  self->string = string;
  return 0;
}

dx_string* dx_ddl_node_get_number(dx_ddl_node const* self) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  if (self->kind != dx_ddl_node_kind_number) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return NULL;
  }
  return self->number;
}

int dx_ddl_node_set_number(dx_ddl_node* self, dx_string* number) {
  if (!self || !number) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->kind != dx_ddl_node_kind_number) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return 1;
  }
  DX_REFERENCE(number);
  DX_UNREFERENCE(self->number);
  self->number = number;
  return 0;
}
