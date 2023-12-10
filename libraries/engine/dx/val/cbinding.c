#include "dx/val/cbinding.h"

// strcmp
#include <string.h>

DX_DECLARE_OBJECT_TYPE("dx.val.cbinding_node",
                       dx_val_cbinding_node,
                       Core_Object);

dx_val_cbinding_node* DX_VAL_CBINDING_NODE(void* p) {
  return (dx_val_cbinding_node*)p;
}

struct dx_val_cbinding_node {
  Core_Object _parent;
  Core_String* name;
  uint8_t tag;
  union {
    DX_VEC3 vec3;
    DX_VEC4 vec4;
    DX_MAT4 mat4;
    DX_RGBA_F32 rgba_f32;
    Core_Size texture_index;
  };
};

dx_val_cbinding_node_dispatch* DX_VAL_CBINDING_NODE_DISPATCH(void* p) {
  return (dx_val_cbinding_node_dispatch*)p;
}

struct dx_val_cbinding_node_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_val_cbinding_node_construct(dx_val_cbinding_node* SELF, Core_String* name);

Core_Result dx_val_cbinding_node_create(dx_val_cbinding_node** RETURN, Core_String* name);

DX_DEFINE_OBJECT_TYPE("dx.val.cbinding_node",
                      dx_val_cbinding_node,
                      Core_Object);

static void dx_val_cbinding_node_destruct(dx_val_cbinding_node* SELF) {
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_val_cbinding_node_constructDispatch(dx_val_cbinding_node_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_cbinding_node_construct(dx_val_cbinding_node* SELF, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_val_cbinding_node);

  SELF->tag = DX_VAL_CBINDING_TYPE_EMPTY;
  
  SELF->name = name;
  DX_REFERENCE(SELF->name);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_cbinding_node_create(dx_val_cbinding_node** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(dx_val_cbinding_node);
  if (dx_val_cbinding_node_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static inline dx_val_cbinding_node* get_or_create(dx_val_cbinding* SELF, Core_String* name) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_get(&node, &SELF->kvs, name)) {
    if (Core_Error_NotFound != Core_getError()) {
      return NULL;
    } else {
      Core_setError(Core_Error_NoError);
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

Core_Result dx_val_cbinding_set_vec3(dx_val_cbinding* SELF, char const* name, DX_VEC3 const* value) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name) + 1)) {
    return Core_Failure;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return Core_Failure;
  }
  node->tag = DX_VAL_CBINDING_TYPE_VEC3;
  node->vec3 = *value;
  DX_UNREFERENCE(node);
  return Core_Success;
}

Core_Result dx_val_cbinding_set_vec4(dx_val_cbinding* SELF, char const* name, DX_VEC4 const* value) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name) + 1)) {
    return Core_Failure;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return Core_Failure;
  }
  node->tag = DX_VAL_CBINDING_TYPE_VEC4;
  node->vec4 = *value;
  DX_UNREFERENCE(node);
  return Core_Success;
}

Core_Result dx_val_cbinding_set_mat4(dx_val_cbinding* SELF, char const *name, DX_MAT4 const* value) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name) + 1)) {
    return Core_Failure;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return Core_Failure;
  }
  node->tag = DX_VAL_CBINDING_TYPE_MAT4;
  node->mat4 = *value;
  DX_UNREFERENCE(node);
  return Core_Success;
}

Core_Result dx_val_cbinding_set_rgba_f32(dx_val_cbinding* SELF, char const* name, DX_RGBA_F32 const* value) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name) + 1)) {
    return Core_Failure;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return Core_Failure;
  }
  node->tag = DX_VAL_CBINDING_TYPE_RGBA_F32;
  node->rgba_f32 = *value;
  DX_UNREFERENCE(node);
  return Core_Success;
}

Core_Result dx_val_cbinding_set_texture_index(dx_val_cbinding* SELF, char const* name, Core_Size value) {
  Core_String* name1 = NULL;
  if (Core_String_create(&name1, name, strlen(name) + 1)) {
    return Core_Failure;
  }
  dx_val_cbinding_node* node = get_or_create(SELF, name1);
  DX_UNREFERENCE(name1);
  name1 = NULL;
  if (!node) {
    return Core_Failure;
  }
  node->tag = DX_VAL_CBINDING_TYPE_TEXTURE_INDEX;
  node->texture_index = value;
  DX_UNREFERENCE(node);
  return Core_Success;
}

DX_DEFINE_OBJECT_TYPE("dx.val.cbinding",
                      dx_val_cbinding,
                      Core_Object);

static void dx_val_cbinding_destruct(dx_val_cbinding* self) {
  dx_inline_pointer_hashmap_uninitialize(&self->kvs);
}

static void dx_val_cbinding_constructDispatch(dx_val_cbinding_dispatch* self)
{/*Intentionally empty.*/}

static Core_Result kvs_hash_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result kvs_keys_equal(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static void kvs_key_added(Core_String** a) {
  DX_REFERENCE(*a);
}

static void kvs_key_removed(Core_String** a) {
  DX_UNREFERENCE(*a);
}

static void kvs_value_added(Core_Object** a) {
  DX_REFERENCE(*a);
}

static void kvs_value_removed(Core_Object** a) {
  DX_UNREFERENCE(*a);
}

Core_Result dx_val_cbinding_construct(dx_val_cbinding* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_cbinding);
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&kvs_keys_equal,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&kvs_hash_key,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&kvs_key_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&kvs_key_removed,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&kvs_value_added,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&kvs_value_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->kvs, &configuration)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_cbinding_create(dx_val_cbinding** RETURN) {
  DX_CREATE_PREFIX(dx_val_cbinding);
  if (dx_val_cbinding_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

int dx_val_cbinding_iter_initialize(dx_val_cbinding_iter* SELF, dx_val_cbinding* target) {
  return dx_inline_pointer_hashmap_iterator_initialize(SELF,&target->kvs);
}

void dx_val_cbinding_iter_uninitialize(dx_val_cbinding_iter* SELF) {
  dx_inline_pointer_hashmap_iterator_uninitialize(SELF);
}

int dx_val_cbinding_iter_next(dx_val_cbinding_iter* SELF) {
  return dx_inline_pointer_hashmap_iterator_next(SELF);
}

bool dx_val_cbinding_iter_has_entry(dx_val_cbinding_iter* SELF) {
  Core_Boolean temporary = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&temporary, SELF)) {
    return false;
  }
  return temporary;
}

uint8_t dx_val_cbinding_iter_get_tag(dx_val_cbinding_iter* SELF) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return 0;
  }
  if (!node) {
    return 0;
  }
  return node->tag;
}

Core_String* dx_val_cbinding_iter_get_name(dx_val_cbinding_iter* SELF) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return NULL;
  }
  if (!node) {
    return NULL;
  }
  DX_REFERENCE(node->name);
  return node->name;
}

Core_Result dx_val_cbinding_iter_get_vec3(dx_val_cbinding_iter* SELF, DX_VEC3* v) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return Core_Failure;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_VEC3) {
    if (!Core_getError()) {
      Core_setError(Core_Error_OperationInvalid);
    }
    return Core_Failure;
  }
  *v = node->vec3;
  return Core_Success;
}

Core_Result dx_val_cbinding_iter_get_vec4(dx_val_cbinding_iter* SELF, DX_VEC4* v) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return Core_Failure;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_VEC4) {
    if (!Core_getError()) {
      Core_setError(Core_Error_OperationInvalid);
    }
    return Core_Failure;
  }
  *v = node->vec4;
  return Core_Success;
}

Core_Result dx_val_cbinding_iter_get_mat4(dx_val_cbinding_iter* SELF, DX_MAT4* a) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return Core_Failure;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_MAT4) {
    if (!Core_getError()) {
      Core_setError(Core_Error_OperationInvalid);
    }
    return Core_Failure;
  }
  *a = node->mat4;
  return Core_Success;
}

Core_Result dx_val_cbinding_iter_get_rgba_f32(dx_val_cbinding_iter* SELF, DX_RGBA_F32* c) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return Core_Failure;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_RGBA_F32) {
    if (!Core_getError()) {
      Core_setError(Core_Error_OperationInvalid);
    }
    return Core_Failure;
  }
  *c = node->rgba_f32;
  return Core_Success;
}

Core_Result dx_val_cbinding_iter_get_texture_index(dx_val_cbinding_iter* SELF, Core_Size* i) {
  dx_val_cbinding_node* node = NULL;
  if (dx_inline_pointer_hashmap_iterator_get_value(&node, SELF)) {
    return Core_Failure;
  }
  if (!node || node->tag != DX_VAL_CBINDING_TYPE_TEXTURE_INDEX) {
    if (!Core_getError()) {
      Core_setError(Core_Error_OperationInvalid);
    }
    return Core_Failure;
  }
  *i = node->texture_index;
  return Core_Success;
}
