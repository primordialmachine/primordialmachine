/// @file dx/core/hapticals.c
/// @brief Mouse buttons and keyboard keys.
#include "dx/core/hapticals.h"

#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

char const* dx_keyboard_key_to_string(dx_keyboard_key SELF) {
  //if (Nucleus_Unlikely(!string)) return Nucleus_Status_InvalidArgument;
  switch (SELF) {
  #define ALIAS(aliased,alias)
  #define DEFINE(name,value,description)\
    case dx_keyboard_key_##name: {\
      static const char *STRING = description;\
      return STRING;\
    };
  #include "dx/core/keyboard_keys.i"
  #undef DEFINE
  #undef ALIAS
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return NULL;
    };
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

char const* dx_mouse_button_to_string(dx_mouse_button SELF) {
  switch (SELF) {
  #define ALIAS(aliased,alias)
  #define DEFINE(name,value,description)\
    case dx_mouse_button_##name: {\
      static const char *STRING = description;\
      return STRING;\
    };
  #include "dx/core/mouse_buttons.i"
  #undef DEFINE
  #undef ALIAS
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return NULL;
    };
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.input_msg",
                      dx_input_msg,
                      Core_Message);

static void dx_input_msg_destruct(dx_input_msg* SELF)
{/*Intentionally empty.*/}

static void dx_input_msg_constructDispatch(dx_input_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_input_msg_construct(dx_input_msg* SELF, dx_input_msg_kind kind, uint8_t modifiers) {
  DX_CONSTRUCT_PREFIX(dx_input_msg);
  if (Core_Message_construct(CORE_MESSAGE(SELF))) {
    return Core_Failure;
  }
  SELF->kind = kind;
  SELF->modifiers = modifiers;
  CORE_MESSAGE(SELF)->flags = DX_MSG_TYPE_INPUT;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

dx_input_msg_kind dx_input_msg_get_kind(dx_input_msg* SELF) {
  return SELF->kind;
}

Core_Result dx_input_msg_get_modifiers(uint8_t* RETURN, dx_input_msg* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->modifiers;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.keyboard_key_action",
                           dx_keyboard_key_action);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.keyboard_key_msg",
                      dx_keyboard_key_msg,
                      dx_input_msg);

static void dx_keyboard_key_msg_destruct(dx_keyboard_key_msg* SELF)
{/*Intentionally empty.*/}

static void dx_keyboard_key_msg_constructDispatch(dx_keyboard_key_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_keyboard_key_msg_construct(dx_keyboard_key_msg* SELF, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers) {
  DX_CONSTRUCT_PREFIX(dx_keyboard_key_msg);
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_KEYBOARD_KEY, modifiers)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->key = key;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_keyboard_key_msg_get_action(dx_keyboard_key_action* RETURN, dx_keyboard_key_msg* SELF) {
  *RETURN = SELF->action;
  return Core_Success;
}

Core_Result dx_keyboard_key_msg_get_key(dx_keyboard_key* RETURN, dx_keyboard_key_msg* SELF) {
  *RETURN = SELF->key;
  return Core_Success;
}

Core_Result dx_keyboard_key_msg_create(dx_keyboard_key_msg** RETURN, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers) {
  DX_CREATE_PREFIX(dx_keyboard_key_msg);
  if (dx_keyboard_key_msg_construct(SELF, action, key, modifiers)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.mouse_button_action",
                           dx_mouse_button_action);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.mouse_button_msg",
                      dx_mouse_button_msg,
                      dx_input_msg);

static void dx_mouse_button_msg_destruct(dx_mouse_button_msg* SELF)
{/*Intentionally empty.*/}

static void dx_mouse_button_msg_constructDispatch(dx_mouse_button_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_mouse_button_msg_construct(dx_mouse_button_msg* SELF, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, Core_Real32 x, Core_Real32 y) {
  DX_CONSTRUCT_PREFIX(dx_mouse_button_msg);
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_MOUSE_BUTTON, modifiers)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->button = button;
  SELF->x = x;
  SELF->y = y;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

dx_mouse_button_action dx_mouse_button_msg_get_action(dx_mouse_button_msg* SELF) {
  return SELF->action;
}

dx_mouse_button dx_mouse_button_msg_get_button(dx_mouse_button_msg* SELF) {
  return SELF->button;
}

Core_Result dx_mouse_button_msg_create(dx_mouse_button_msg** RETURN, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, Core_Real32 x, Core_Real32 y) {
  DX_CREATE_PREFIX(dx_mouse_button_msg);
  if (dx_mouse_button_msg_construct(SELF, action, button, modifiers, x, y)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.mouse_pointer_action",
                           dx_mouse_pointer_action);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.mouse_pointer_msg",
                      dx_mouse_pointer_msg,
                      dx_input_msg);

static void dx_mouse_pointer_msg_destruct(dx_mouse_pointer_msg* SELF)
{/*Intentionally empty.*/}

static void dx_mouse_pointer_msg_constructDispatch(dx_mouse_pointer_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* SELF, dx_mouse_pointer_action action, uint8_t modifiers, Core_Real32 x, Core_Real32 y) {
  DX_CONSTRUCT_PREFIX(dx_mouse_pointer_msg);
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_MOUSE_POINTER, modifiers)) {
    return Core_Failure;
  }
  SELF->action = action;
  SELF->x = x;
  SELF->y = y;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_mouse_pointer_msg_get_action(dx_mouse_pointer_action* RETURN, dx_mouse_pointer_msg* SELF) {
  *RETURN = SELF->action;
  return Core_Success;
}

Core_Result dx_mouse_pointer_msg_create(dx_mouse_pointer_msg** RETURN, dx_mouse_pointer_action action, uint8_t modifiers, Core_Real32 x, Core_Real32 y) {
  DX_CREATE_PREFIX(dx_mouse_pointer_msg);
  if (dx_mouse_pointer_msg_construct(SELF, action, modifiers, x, y)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
