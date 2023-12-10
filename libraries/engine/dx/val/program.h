#if !defined(DX_PROGRAM_H_INCLUDED)
#define DX_PROGRAM_H_INCLUDED

#include "dx/val/context.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.val.program",
                       dx_val_program,
                       Core_Object);

static inline dx_val_program* DX_VAL_PROGRAM(void* p) {
  return (dx_val_program*)p;
}

struct dx_val_program {
  Core_Object _parent;
  dx_val_context* ctx;
};

static inline dx_val_program_dispatch* DX_VAL_PROGRAM_DISPATCH(void* p) {
  return (dx_val_program_dispatch*)p;
}

struct dx_val_program_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*activate)(dx_val_program*);
  Core_Result (*bind)(dx_val_program*, dx_val_cbinding*);
};

Core_Result dx_val_program_construct(dx_val_program* SELF, dx_val_context* ctx);

static inline Core_Result dx_val_program_activate(dx_val_program* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_program, activate, SELF);
}

/// @brief Apply the specified constant binding to this program.
/// @param SELF A pointer to this program.
/// @param binding A pointer to the specified constant binding.
/// @method-call
static inline Core_Result dx_val_program_bind(dx_val_program* SELF, dx_val_cbinding* cbinding) {
  DX_OBJECT_VIRTUALCALL(dx_val_program, bind, SELF, cbinding);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_VAL_PROGRAM_H_INCLUDED
