#include "Core/Assets/Ref.h"

Core_defineObjectType("Core.Assets.Ref",
                      Core_Assets_Ref,
                      Core_Object);

static void Core_Assets_Ref_destruct(Core_Assets_Ref* self) {
  if (self->object) {
    CORE_UNREFERENCE(self->object);
    self->object = NULL;
  }
  CORE_UNREFERENCE(self->name);
  self->name = NULL;
}

static void Core_Assets_Ref_constructDispatch(Core_Assets_Ref_Dispatch* self)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Ref_construct(Core_Assets_Ref* SELF, Core_String* name) {
  Core_BeginConstructor(Core_Assets_Ref);
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  CORE_REFERENCE(SELF->name);
  SELF->object = NULL;
  Core_EndConstructor(Core_Assets_Ref);
}

Core_Result Core_Assets_Ref_create(Core_Assets_Ref** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(Core_Assets_Ref);
  if (Core_Assets_Ref_construct(SELF, name)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
