#include "dx/val/texture.h"

Core_defineObjectType("Core.Visuals.Texture",
                      Core_Visuals_Texture,
                      Core_Object);

static void Core_Visuals_Texture_destruct(Core_Visuals_Texture* SELF) {
  SELF->context = NULL;
}

static void Core_Visuals_Texture_constructDispatch(Core_Visuals_Texture_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Visuals_Texture_construct(Core_Visuals_Texture* SELF, Core_Visuals_Context* context) {
  Core_BeginConstructor(Core_Visuals_Texture);
  SELF->context = context;
  Core_EndConstructor(Core_Visuals_Texture);
}
