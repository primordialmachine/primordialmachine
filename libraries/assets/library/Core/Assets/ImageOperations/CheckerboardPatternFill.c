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


Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setNumberOfCheckersHorizontal(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size numberOfCheckersHorizontal) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->numberOfCheckersHorizontal = numberOfCheckersHorizontal;
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getNumberOfCheckersHorizontal(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->numberOfCheckersHorizontal;
  return Core_Success;
}


Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setNumberOfCheckersVertical(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size numberOfCheckersVertical) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->numberOfCheckersVertical = numberOfCheckersVertical;
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getNumberOfCheckersVertical(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->numberOfCheckersVertical;
  return Core_Success;
}


Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setCheckerSizeHorizontal(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size checkerSizeHorizontal) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->checkerSizeHorizontal = checkerSizeHorizontal;
  return Core_Success;
}

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getCheckerSizeHorizontal(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->checkerSizeHorizontal;
  return Core_Success;
}


Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setCheckerSizeVertical(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size checkerSizeVertical) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->checkerSizeVertical = checkerSizeVertical;
  return Core_Success;
  }

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getCheckerSizeVertical(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->checkerSizeVertical;
  return Core_Success;
}


Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setFirstCheckerColor(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* first_checker_color) {
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

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getFirstCheckerColor(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  *RETURN = CORE_ASSETS_COLORRGBN8(SELF->first_checker_color->object)->value;
  return Core_Success;
}

#undef PROPERTY

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_setSecondCheckerColor(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* second_checker_color) {
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

Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_getSecondCheckerColor(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF) {
  *RETURN = CORE_ASSETS_COLORRGBN8(SELF->second_checker_color)->value;
  return Core_Success;
}
