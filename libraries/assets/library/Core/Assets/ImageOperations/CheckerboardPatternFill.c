#include "Core/Assets/ImageOperations/CheckerboardPatternFill.h"

Core_defineObjectType("Core.Assets.ImageOperations.CheckerboardPatternFill",
                      Core_Assets_ImageOperations_CheckerboardPatternFill,
                      Core_Assets_ImageOperation);

static void Core_Assets_ImageOperations_CheckerboardPatternFill_destruct(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  if (SELF->second_checker_color) {
    CORE_UNREFERENCE(SELF->second_checker_color);
    SELF->second_checker_color = NULL;
  }
  if (SELF->first_checker_color) {
    CORE_UNREFERENCE(SELF->first_checker_color);
    SELF->first_checker_color = NULL;
  }
}

static void Core_Assets_ImageOperations_CheckerboardPatternFill_constructDispatch(Core_Assets_ImageOperations_CheckerboardPatternFill_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_construct(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperations_CheckerboardPatternFill);
  if (Core_Assets_ImageOperation_construct(CORE_ASSETS_IMAGEOPERATION(SELF))) {
    return Core_Failure;
  }
  SELF->first_checker_color = NULL;
  SELF->second_checker_color = NULL;
  Core_EndConstructor(Core_Assets_ImageOperations_CheckerboardPatternFill);
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_create(Core_Assets_ImageOperations_CheckerboardPatternFill** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_ImageOperations_CheckerboardPatternFill);
  if (Core_Assets_ImageOperations_CheckerboardPatternFill_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

#define PROPERTY(TYPE, PROPERTY_TYPE, PROPERTY_NAME) \
  Core_Result TYPE##_set_##PROPERTY_NAME(TYPE* SELF, PROPERTY_TYPE PROPERTY_NAME) { \
    SELF->PROPERTY_NAME = PROPERTY_NAME; \
    return Core_Success; \
  } \
  Core_Result TYPE##_get_##PROPERTY_NAME##(PROPERTY_TYPE* RETURN, TYPE* SELF) { \
    *RETURN = SELF->PROPERTY_NAME; \
    return Core_Success; \
  }

PROPERTY(Core_Assets_ImageOperations_CheckerboardPatternFill, Core_Size, number_of_checkers_horizontal)
PROPERTY(Core_Assets_ImageOperations_CheckerboardPatternFill, Core_Size, number_of_checkers_vertical)
PROPERTY(Core_Assets_ImageOperations_CheckerboardPatternFill, Core_Size, checker_size_horizontal)
PROPERTY(Core_Assets_ImageOperations_CheckerboardPatternFill, Core_Size, checker_size_vertical)

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_first_checker_color(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* first_checker_color) {
  if (!SELF->first_checker_color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (Core_Assets_Ref_create(&SELF->first_checker_color, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  CORE_REFERENCE(first_checker_color);
  if (SELF->first_checker_color->object) {
    CORE_UNREFERENCE(SELF->first_checker_color->object);
  }
  SELF->first_checker_color->object = CORE_OBJECT(first_checker_color);
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_first_checker_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  *RETURN = CORE_ASSETS_COLORRGBN8(SELF->first_checker_color->object)->value;
  return Core_Success;
}

#undef PROPERTY

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_second_checker_color(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* second_checker_color) {
  if (!SELF->second_checker_color) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (Core_Assets_Ref_create(&SELF->second_checker_color, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  CORE_REFERENCE(second_checker_color);
  if (SELF->second_checker_color->object) {
    CORE_UNREFERENCE(SELF->second_checker_color->object);
  }
  SELF->second_checker_color->object = CORE_OBJECT(second_checker_color);
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_second_checker_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  *RETURN = CORE_ASSETS_COLORRGBN8(SELF->second_checker_color)->value;
  return Core_Success;
}
