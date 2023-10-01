#if !defined(DX_ADL_NAMES_COLOR_H_INCLUDED)
#define DX_ADL_NAMES_COLOR_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_ENUMERATION_TYPE("dx.adl.name_index",
                            dx_adl_name_index)

enum dx_semantical_name_index {
#define DEFINE(NAME, STRING) dx_adl_name_index_##NAME,
#include "dx/adl/names.i"
#undef DEFINE
};

#define DX_ADL_NAMES_NUMBER_OF_NAMES (((dx_size)dx_adl_name_index_z_key) + 1)

DX_DECLARE_OBJECT_TYPE("dx.adl.names",
                       dx_adl_names,
                       dx_object)

static inline dx_adl_names* DX_ADL_NAMES(void* p) {
  return (dx_adl_names*)p;
}

struct dx_adl_names {
  dx_object parent;
  dx_string** names;
  dx_size counter;
};

struct dx_adl_names_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_adl_names_construct(dx_adl_names* SELF);

dx_result dx_adl_names_create(dx_adl_names** RETURN);

dx_string* dx_adl_names_create_unique_name(dx_adl_names* self);

#endif // DX_ADL_NAMES_COLOR_H_INCLUDED
