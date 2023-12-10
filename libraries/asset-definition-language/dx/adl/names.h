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

#define DX_ADL_NAMES_NUMBER_OF_NAMES (((Core_Size)dx_adl_name_index_z_key) + 1)

DX_DECLARE_OBJECT_TYPE("dx.adl.names",
                       dx_adl_names,
                       Core_Object);

static inline dx_adl_names* DX_ADL_NAMES(void* p) {
  return (dx_adl_names*)p;
}

struct dx_adl_names {
  Core_Object _parent;
  Core_String** names;
  Core_Size counter;
};

struct dx_adl_names_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_adl_names_construct(dx_adl_names* SELF);

Core_Result dx_adl_names_create(dx_adl_names** RETURN);

Core_String* dx_adl_names_create_unique_name(dx_adl_names* self);

#endif // DX_ADL_NAMES_COLOR_H_INCLUDED
