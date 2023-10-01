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
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
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
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return NULL;
    };
  };
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.input_msg",
                      dx_input_msg,
                      dx_msg);

static void dx_input_msg_destruct(dx_input_msg* SELF)
{/*Intentionally empty.*/}

static void dx_input_msg_dispatch_construct(dx_input_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_input_msg_construct(dx_input_msg* SELF, dx_input_msg_kind kind, uint8_t modifiers) {
  dx_rti_type* TYPE = dx_input_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_msg_construct(DX_MSG(SELF))) {
    return DX_FAILURE;
  }
  SELF->kind = kind;
  SELF->modifiers = modifiers;
  DX_MSG(SELF)->flags = DX_MSG_TYPE_INPUT;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_input_msg_kind dx_input_msg_get_kind(dx_input_msg* SELF) {
  return SELF->kind;
}

dx_result dx_input_msg_get_modifiers(uint8_t* RETURN, dx_input_msg* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->modifiers;
  return DX_SUCCESS;
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

static void dx_keyboard_key_msg_dispatch_construct(dx_keyboard_key_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_keyboard_key_msg_construct(dx_keyboard_key_msg* SELF, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers) {
  dx_rti_type* TYPE = dx_keyboard_key_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_KEYBOARD_KEY, modifiers)) {
    return DX_FAILURE;
  }
  SELF->action = action;
  SELF->key = key;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_keyboard_key_msg_get_action(dx_keyboard_key_action* RETURN, dx_keyboard_key_msg* SELF) {
  *RETURN = SELF->action;
  return DX_SUCCESS;
}

dx_result dx_keyboard_key_msg_get_key(dx_keyboard_key* RETURN, dx_keyboard_key_msg* SELF) {
  *RETURN = SELF->key;
  return DX_SUCCESS;
}

dx_result dx_keyboard_key_msg_create(dx_keyboard_key_msg** RETURN, dx_keyboard_key_action action, dx_keyboard_key key, uint8_t modifiers) {
  DX_CREATE_PREFIX(dx_keyboard_key_msg)
  if (dx_keyboard_key_msg_construct(SELF, action, key, modifiers)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
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

static void dx_mouse_button_msg_dispatch_construct(dx_mouse_button_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_mouse_button_msg_construct(dx_mouse_button_msg* SELF, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_rti_type* TYPE = dx_mouse_button_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_MOUSE_BUTTON, modifiers)) {
    return DX_FAILURE;
  }
  SELF->action = action;
  SELF->button = button;
  SELF->x = x;
  SELF->y = y;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_mouse_button_action dx_mouse_button_msg_get_action(dx_mouse_button_msg* SELF) {
  return SELF->action;
}

dx_mouse_button dx_mouse_button_msg_get_button(dx_mouse_button_msg* SELF) {
  return SELF->button;
}

dx_result dx_mouse_button_msg_create(dx_mouse_button_msg** RETURN, dx_mouse_button_action action, dx_mouse_button button, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  DX_CREATE_PREFIX(dx_mouse_button_msg)
  if (dx_mouse_button_msg_construct(SELF, action, button, modifiers, x, y)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
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

static void dx_mouse_pointer_msg_dispatch_construct(dx_mouse_pointer_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_mouse_pointer_msg_construct(dx_mouse_pointer_msg* SELF, dx_mouse_pointer_action action, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  dx_rti_type* TYPE = dx_mouse_pointer_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_input_msg_construct(DX_INPUT_MSG(SELF), DX_INPUT_MSG_KIND_MOUSE_POINTER, modifiers)) {
    return DX_FAILURE;
  }
  SELF->action = action;
  SELF->x = x;
  SELF->y = y;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_mouse_pointer_msg_get_action(dx_mouse_pointer_action* RETURN, dx_mouse_pointer_msg* SELF) {
  *RETURN = SELF->action;
  return DX_SUCCESS;
}

dx_result dx_mouse_pointer_msg_create(dx_mouse_pointer_msg** RETURN, dx_mouse_pointer_action action, uint8_t modifiers, dx_f32 x, dx_f32 y) {
  DX_CREATE_PREFIX(dx_mouse_pointer_msg)
  if (dx_mouse_pointer_msg_construct(SELF, action, modifiers, x, y)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
