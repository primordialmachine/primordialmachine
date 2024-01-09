#if !defined(DX_VAL_VBINDING_H_INCLUDED)
#define DX_VAL_VBINDING_H_INCLUDED

#include "dx/val/context.h"

/// A "variable binding" (dx_val_vbinding) provides inputs to "variables" in "programs"
/// (like the "constant binding" (dx_val_cbinding) provides inputs to "constants" in "programs").
///
/// A variable binding owns a single buffer (dx_val_buffer) of raw Bytes and a description (DX_VSEMANTICS) how these raw Bytes are interpreted as data assigned to variables in programs.
/// Example:
/// A binding owns a buffer of sizeof(Core_Real32)*(3+4)*4 Bytes.
/// Its description might say:
/// - interprete consecutive disjoint subsequences of sizeof(Core_Real32)*(3+4) consecutive Bytes as a vertex
/// - the vertex consists of two vertex elements such that the first sizeof(Core_Real32)*3 Bytes are interpreted as xyz position values and the last sizeof(Core_Real32)*4 Bytes are interpreted as rgba color values.
/// - map the position vertex element to the vertex variable "position" in the program and the color vertex elements to the vertex variable of name "color" in the program
///
/// Currently, DX does not check if the specified variable buffer and the specified variable semantics are compatible when presenting:
/// That is, it is not verified that
/// - the buffer is big enough to provide th specified sub-sequence of variable value to present and
/// - the program has ariables of the right types declared to receive the variables values.
Core_declareObjectType("dx.val.vbinding",
                       dx_val_vbinding,
                       Core_Object);

static inline dx_val_vbinding* DX_VAL_VBINDING(void* p) {
  return (dx_val_vbinding*)p;
}

struct dx_val_vbinding {
  Core_Object _parent;
  dx_val_buffer* buffer;
  dx_val_context* context;
};

static inline dx_val_vbinding_Dispatch* DX_VAL_VBINDING_DISPATCH(void* p) {
  return (dx_val_vbinding_Dispatch*)p;
}

struct dx_val_vbinding_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*activate)(dx_val_vbinding*);
};

Core_Result dx_val_vbinding_construct(dx_val_vbinding* SELF, dx_val_buffer* buffer);

static inline Core_Result dx_val_vbinding_activate(dx_val_vbinding* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_val_vbinding, activate, SELF);
}

#endif // DX_VAL_VBINDING_H_INCLUDED
