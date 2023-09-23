#if !defined(DX_VAL_CBINDING_H_INCLUDED)
#define DX_VAL_CBINDING_H_INCLUDED

#include "dx/core.h"

/// A "constant binding" (dx_val_cbinding) provides inputs to "constants" in "programs"
/// (like the "variable binding" (dx_val_vbinding) provides inputs to "variables" in "programs").
DX_DECLARE_OBJECT_TYPE("dx.val.cbinding",
                       dx_val_cbinding,
                       dx_object);

static inline dx_val_cbinding* DX_VAL_CBINDING(void* p) {
  return (dx_val_cbinding*)p;
}

struct dx_val_cbinding {
  dx_object _parent;
  dx_inline_pointer_hashmap kvs;
};

struct dx_val_cbinding_dispatch {
  dx_object_dispatch _parent;
};

#define DX_VAL_CBINDING_TYPE_EMPTY (0)
#define DX_VAL_CBINDING_TYPE_MAT4 (1)
#define DX_VAL_CBINDING_TYPE_VEC3 (2)
#define DX_VAL_CBINDING_TYPE_VEC4 (3)
#define DX_VAL_CBINDING_TYPE_RGBA_F32 (4)
#define DX_VAL_CBINDING_TYPE_TEXTURE_INDEX (5)

/// @brief Assign a variable of the specified name the specified value.
/// @param self A pointer to this constant binding.
/// @param name The name of the variable.
/// @param v The value of the variable.
/// @method-call
dx_result dx_val_cbinding_set_vec3(dx_val_cbinding* SELF, char const* name, DX_VEC3 const* value);

/// @brief Assign a variable of the specified name the specified value.
/// @param self A pointer to this constant binding.
/// @param name The name of the variable.
/// @param v The value of the variable.
/// @method-call
dx_result dx_val_cbinding_set_vec4(dx_val_cbinding* SELF, char const* name, DX_VEC4 const* value);

/// @brief Assign a variable of the specified name the specified value.
/// @param self A pointer to this constant binding.
/// @param name The name of the variable.
/// @param v The value of the variable.
/// @method-call
dx_result dx_val_cbinding_set_mat4(dx_val_cbinding* SELF, char const* name, DX_MAT4 const* value);

/// @brief Assign a variable of the specified name the specified value.
/// @param self A pointer to this constant binding.
/// @param name The name of the variable.
/// @param v The value of the variable.
/// @method-call
dx_result dx_val_cbinding_set_rgba_f32(dx_val_cbinding* SELF, char const* name, DX_RGBA_F32 const* value);

/// @brief Assign a variable of the specified name the specified value.
/// @param self A pointer to this constant binding.
/// @param name The name of the variable.
/// @param v The value of the variable.
/// @method-call
dx_result dx_val_cbinding_set_texture_index(dx_val_cbinding* SELF, char const* name, dx_size value);

dx_result dx_val_cbinding_construct(dx_val_cbinding* self);

dx_val_cbinding* dx_val_cbinding_create();

typedef dx_inline_pointer_hashmap_iterator dx_val_cbinding_iter;

int dx_val_cbinding_iter_initialize(dx_val_cbinding_iter* self, dx_val_cbinding* target);

void dx_val_cbinding_iter_uninitialize(dx_val_cbinding_iter* self);

int dx_val_cbinding_iter_next(dx_val_cbinding_iter* iter);

/// @return @a true if there is a value. @a false if there is no value. @a false is also returned on failure.
/// @default-failure
bool dx_val_cbinding_iter_has_entry(dx_val_cbinding_iter* iter);

/// @return The tag on success. DX_VAL_CBINDING_TYPE_EMPTY is also returned on failure.
uint8_t dx_val_cbinding_iter_get_tag(dx_val_cbinding_iter* iter);

/// @return A pointer to the name on success. The null poiter on failure.
/// @success The caller acquired a reference to the dx_string object.
dx_string* dx_val_cbinding_iter_get_name(dx_val_cbinding_iter* iter);

/// @default-return 
/// @default-failure
dx_result dx_val_cbinding_iter_get_vec3(dx_val_cbinding_iter* iter, DX_VEC3* v);

/// @default-return 
/// @default-failure
dx_result dx_val_cbinding_iter_get_vec4(dx_val_cbinding_iter* iter, DX_VEC4* v);

/// @brief 
/// @param iter 
/// @param v 
/// @default-return 
/// @default-failure
dx_result dx_val_cbinding_iter_get_mat4(dx_val_cbinding_iter* iter, DX_MAT4* a);

/// @default-return 
/// @default-failure
dx_result dx_val_cbinding_iter_get_rgba_f32(dx_val_cbinding_iter* iter, DX_RGBA_F32* c);

/// @default-return 
/// @default-failure
dx_result dx_val_cbinding_iter_get_texture_index(dx_val_cbinding_iter* iter, dx_size*i);

#endif // DX_VAL_CBINDING_H_INCLUDED
