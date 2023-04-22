#include "dx/val/cbinding.h"

// strcmp
#include <string.h>

DX_DECLARE_OBJECT_TYPE("dx.val.cbinding_node",
                       dx_val_cbinding_node,
                       dx_object);

dx_val_cbinding_node* DX_VAL_CBINDING_NODE(void* p) {
  return (dx_val_cbinding_node*)p;
}

struct dx_val_cbinding_node {
  dx_object _parent;
  dx_string* name;
  uint8_t tag;
  union {
    DX_VEC3 vec3;
    DX_VEC4 vec4;
    DX_MAT4 mat4;
    DX_RGBA_F32 rgba_f32;
    dx_size texture_index;
  };
};

dx_val_cbinding_node_dispatch* DX_VAL_CBINDING_NODE_DISPATCH(void* p) {
  return (dx_val_cbinding_node_dispatch*)p;
}

struct dx_val_cbinding_node_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_val_cbinding_node_construct(dx_val_cbinding_node* SELF, dx_string* name);

dx_result dx_val_cbinding_node_create(dx_val_cbinding_node** RETURN, dx_string* name);

DX_DEFINE_OBJECT_TYPE("dx.val.cbinding_node",
                      dx_val_cbinding_node,
                      dx_object);

static void dx_val_cbinding_node_destruct(dx_val_cbinding_node* SELF) {
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_val_cbinding_node_dispatch_construct(dx_val_cbinding_node_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_cbinding_node_construct(dx_val_cbinding_node* SELF, dx_string* name) {
  dx_rti_type* TYPE = dx_val_cbinding_node_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->tag = DX_VAL_CBINDING_TYPE_EMPTY;
  
  SELF->name = name;
  DX_REFERENCE(SELF->name);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_node_create(dx_val_cbinding_node** RETURN, dx_string* name) {
  dx_val_cbinding_node* SELF = DX_VAL_CBINDING_NODE(dx_object_alloc(sizeof(dx_val_cbinding_node)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_val_cbinding_node_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static inline dx_val_cbinding_node* get_or_create(dx_val_cbinding* SELF, dx_string* name) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_get(&node, &SELF->kvs, name)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return NULL;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  }
  if (!node) {
    if (dx_val_cbinding_node_create(&node, name)) {
      return NULL;
    }
    if (dx_inline_pointer_hashmap_set(&SELF->kvs, node->name, node)) {
      DX_UNREFERENCE(node);
      node = NULL;
      return NULL;
    }
  } else {
    DX_REFERENCE(node);
  }
  return node;
}

dx_result dx_val_cbinding_set_vec3(dx_val_cbinding* SELF, char const* name, DX_VEC3 const* value) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name) + 1)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return DX_FAILURE;
  }
  node->tag = DX_VAL_CBINDING_TYPE_VEC3;
  node->vec3 = *value;
  DX_UNREFERENCE(node);
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_set_vec4(dx_val_cbinding* SELF, char const* name, DX_VEC4 const* value) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name) + 1)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return DX_FAILURE;
  }
  node->tag = DX_VAL_CBINDING_TYPE_VEC4;
  node->vec4 = *value;
  DX_UNREFERENCE(node);
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_set_mat4(dx_val_cbinding* SELF, char const *name, DX_MAT4 const* value) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name) + 1)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return DX_FAILURE;
  }
  node->tag = DX_VAL_CBINDING_TYPE_MAT4;
  node->mat4 = *value;
  DX_UNREFERENCE(node);
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_set_rgba_f32(dx_val_cbinding* SELF, char const* name, DX_RGBA_F32 const* value) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name) + 1)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return DX_FAILURE;
  }
  node->tag = DX_VAL_CBINDING_TYPE_RGBA_F32;
  node->rgba_f32 = *value;
  DX_UNREFERENCE(node);
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_set_texture_index(dx_val_cbinding* SELF, char const* name, dx_size value) {
  dx_string* name1 = NULL;
  if (dx_string_create(&name1, name, strlen(name) + 1)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return DX_FAILURE;
  }
  node->tag = DX_VAL_CBINDING_TYPE_TEXTURE_INDEX;
  node->texture_index = value;
  DX_UNREFERENCE(node);
  return DX_SUCCESS;
}

DX_DEFINE_OBJECT_TYPE("dx.val.cbinding",
                      dx_val_cbinding,
                      dx_object);

static void dx_val_cbinding_destruct(dx_val_cbinding* self) {
  dx_inline_pointer_hashmap_uninitialize(&self->kvs);
}

static void dx_val_cbinding_dispatch_construct(dx_val_cbinding_dispatch* self)
{/*Intentionally empty.*/}

static dx_result kvs_hash_key(dx_size* RETURN, dx_string** a) {
  *RETURN = dx_string_get_hash_value(*a);
  return DX_SUCCESS;
}

static dx_result kvs_keys_equal(dx_bool* RETURN, dx_string** a, dx_string** b) {
  *RETURN = dx_string_is_equal_to(*a, *b);
  return DX_SUCCESS;
}

static void kvs_key_added(dx_string** a) {
  DX_REFERENCE(*a);
}

static void kvs_key_removed(dx_string** a) {
  DX_UNREFERENCE(*a);
}

static void kvs_value_added(dx_object** a) {
  DX_REFERENCE(*a);
}

static void kvs_value_removed(dx_object** a) {
  DX_UNREFERENCE(*a);
}

dx_result dx_val_cbinding_construct(dx_val_cbinding* self) {
  dx_rti_type* TYPE = dx_val_cbinding_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&kvs_keys_equal,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&kvs_hash_key,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&kvs_key_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&kvs_key_removed,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&kvs_value_added,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&kvs_value_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&self->kvs, &configuration)) {
    return DX_FAILURE;
  }
  DX_OBJECT(self)->type = TYPE;
  return DX_SUCCESS;
}

dx_val_cbinding* dx_val_cbinding_create() {
  dx_val_cbinding* self = DX_VAL_CBINDING(dx_object_alloc(sizeof(dx_val_cbinding)));
  if (!self) {
    return NULL;
  }
  if (dx_val_cbinding_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_val_cbinding_iter_initialize(dx_val_cbinding_iter* self, dx_val_cbinding* target) {
  return dx_inline_pointer_hashmap_iterator_initialize(self,&target->kvs);
}

void dx_val_cbinding_iter_uninitialize(dx_val_cbinding_iter* self) {
  dx_inline_pointer_hashmap_iterator_uninitialize(self);
}

int dx_val_cbinding_iter_next(dx_val_cbinding_iter* self) {
  return dx_inline_pointer_hashmap_iterator_next(self);
}

bool dx_val_cbinding_iter_has_entry(dx_val_cbinding_iter* self) {
  dx_bool temporary = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&temporary, self)) {
    return false;
  }
  return temporary;
}

uint8_t dx_val_cbinding_iter_get_tag(dx_val_cbinding_iter* self) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return 0;
  }
  if (!node) {
    return 0;
  }
  return node->tag;
}

dx_string* dx_val_cbinding_iter_get_name(dx_val_cbinding_iter* self) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return NULL;
  }
  if (!node) {
    return NULL;
  }
  DX_REFERENCE(node->name);
  return node->name;
}

dx_result dx_val_cbinding_iter_get_vec3(dx_val_cbinding_iter* self, DX_VEC3* v) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return DX_FAILURE;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_VEC3) {
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_INVALID_OPERATION);
    }
    return DX_FAILURE;
  }
  *v = node->vec3;
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_iter_get_vec4(dx_val_cbinding_iter* self, DX_VEC4* v) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return DX_FAILURE;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_VEC4) {
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_INVALID_OPERATION);
    }
    return DX_FAILURE;
  }
  *v = node->vec4;
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_iter_get_mat4(dx_val_cbinding_iter* self, DX_MAT4* a) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return DX_FAILURE;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_MAT4) {
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_INVALID_OPERATION);
    }
    return DX_FAILURE;
  }
  *a = node->mat4;
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_iter_get_rgba_f32(dx_val_cbinding_iter* self, DX_RGBA_F32* c) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return DX_FAILURE;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_RGBA_F32) {
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_INVALID_OPERATION);
    }
    return DX_FAILURE;
  }
  *c = node->rgba_f32;
  return DX_SUCCESS;
}

dx_result dx_val_cbinding_iter_get_texture_index(dx_val_cbinding_iter* self, dx_size* i) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, self)) {
    return DX_FAILURE;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_TEXTURE_INDEX) {
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_INVALID_OPERATION);
    }
    return DX_FAILURE;
  }
  *i = node->texture_index;
  return DX_SUCCESS;
}
