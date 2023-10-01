#include "dx/ui/manager.h"

#include "dx/val/command.h"

#include "dx/ui/group.h"
#include "dx/ui/type_handlers/group.h"

#include "dx/ui/text_field.h"
#include "dx/ui/type_handlers/text_field.h"

#include "dx/data_definition_language.h"

// strlen()
#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ui.manager",
                      dx_ui_manager,
                      dx_object);

/// @ingroup type_handlers
static dx_result compare_keys_callback(dx_bool* RETURN, dx_string** key1, dx_string** key2);

/// @ingroup type_handlers
static dx_result hash_key_callback(dx_size* RETURN, dx_string** key);

/// @ingroup type_handlers
static void key_added_callback(dx_string** key);

/// @ingroup type_handlers
static void key_removed_callback(dx_string** key);

/// @ingroup type_handlers
static void value_added_callback(dx_object** value);

/// @ingroup type_handlers
static void value_removed_callback(dx_object** value);

/// @ingroup type_handlers
static dx_result initialize_type_handlers(dx_ui_manager* SELF);

/// @ingroup type_handlers
static dx_result uninitialize_type_handlers(dx_ui_manager* SELF);

static dx_result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group);

static dx_result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget);

static dx_result compare_keys_callback(dx_bool* RETURN, dx_string** key1, dx_string** key2) {
  DX_DEBUG_ASSERT(NULL != key1);
  DX_DEBUG_ASSERT(NULL != *key1);
  DX_DEBUG_ASSERT(NULL != key2);
  DX_DEBUG_ASSERT(NULL != *key2);
  *RETURN = dx_string_is_equal_to(*key1, *key2);
  return DX_SUCCESS;
}

static dx_result hash_key_callback(dx_size* RETURN, dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  *RETURN = dx_string_get_hash_value(*key);
  return DX_SUCCESS;
}

static void key_added_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_REFERENCE(*key);
}

static void key_removed_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_UNREFERENCE(*key);
}

static void value_added_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_REFERENCE(*value);
}

static void value_removed_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_UNREFERENCE(*value);
}

static dx_result initialize_type_handlers(dx_ui_manager* SELF) {
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&compare_keys_callback,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&hash_key_callback,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&key_added_callback,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&key_removed_callback,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&value_added_callback,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&value_removed_callback,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->type_handlers, &configuration)) {
    return DX_FAILURE;
  }

#define DEFINE(KEY, VALUE) \
    { \
      dx_string* k = NULL; \
      if (dx_string_create(&k, KEY, strlen(KEY))) { \
        return DX_FAILURE; \
      } \
      dx_ui_type_handler* v = NULL; \
      if (VALUE##_create((VALUE**)&v)) { \
        DX_UNREFERENCE(k); \
        k = NULL; \
        dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers); \
        return DX_FAILURE; \
      } \
      if (dx_inline_pointer_hashmap_set(&SELF->type_handlers, k, v)) { \
        DX_UNREFERENCE(v); \
        v = NULL; \
        DX_UNREFERENCE(k); \
        k = NULL; \
        dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers); \
        return DX_FAILURE; \
      } \
      DX_UNREFERENCE(v); \
      v = NULL; \
      DX_UNREFERENCE(k); \
      k = NULL; \
    }

    DEFINE("Group", dx_ui_type_handlers_group);
    DEFINE("TextField", dx_ui_type_handlers_text_field);

  #undef DEFINE

    return DX_SUCCESS;
}

static dx_result uninitialize_type_handlers(dx_ui_manager* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers);
  return DX_SUCCESS;
}

static void dx_ui_manager_destruct(dx_ui_manager* SELF) {
  uninitialize_type_handlers(SELF);
  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;
  DX_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;
  DX_UNREFERENCE(SELF->command_list);
  SELF->command_list = NULL;
}

static void dx_ui_manager_dispatch_construct(dx_ui_manager_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_ui_manager_construct(dx_ui_manager* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  dx_rti_type* TYPE = dx_ui_manager_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (dx_val_command_list_create(&SELF->command_list)) {
    return DX_FAILURE;
  }

  // create the set viewport command
  dx_val_command* command = NULL;
  if (dx_val_command_create_viewport(&command, 0.f, 0.f, 640.f, 480.f)) {
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  SELF->font_presenter = font_presenter;
  DX_REFERENCE(font_presenter);

  SELF->rectangle_presenter = rectangle_presenter;
  DX_REFERENCE(rectangle_presenter);
  
  if (initialize_type_handlers(SELF)) {
    DX_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    DX_UNREFERENCE(SELF->font_presenter);
    SELF->font_presenter = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return DX_FAILURE;
  }

  dx_vec2_f32_set(&SELF->dpi, 96.f, 96.f);
  
  dx_vec2_f32_set(&SELF->resolution, 640, 480);
  
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_create(dx_ui_manager** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_ui_manager)
    if (dx_ui_manager_construct(SELF, font_presenter, rectangle_presenter)) {
      DX_UNREFERENCE(SELF);
      SELF = NULL;
      return DX_FAILURE;
    }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_canvas_dpi(dx_ui_manager* SELF, DX_VEC2_F32 const* dpi) {
  SELF->dpi = *dpi;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_canvas_resolution(dx_ui_manager* SELF, DX_VEC2_F32 const* resolution) {
  SELF->resolution = *resolution;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_root(dx_ui_manager* SELF, dx_ui_widget* root) {
  SELF->root = root;
  return DX_SUCCESS;
}

static dx_result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group) {
  dx_size n;
  if (dx_object_array_get_size(&n, group->children)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ui_widget* widget = NULL;
    if (dx_object_array_get_at((dx_object**)&widget, group->children, i)) {
      return DX_FAILURE;
    }
    if (on_render_widget(SELF, widget)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget) {
  if (dx_ui_widget_render(widget, SELF->resolution.e[0], SELF->resolution.e[1], SELF->dpi.e[0], SELF->dpi.e[1])) {
    return DX_FAILURE;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(widget)->type, dx_ui_group_get_type())) {
    if (on_render_group_children(SELF, DX_UI_GROUP(widget))) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

dx_result dx_ui_manager_enter_render(dx_ui_manager* SELF) {
  dx_val_command* command = NULL;

  // Update the viewport command.
  command = dx_val_command_list_get_at(SELF->command_list, 0);
  command->viewport_command.w = SELF->resolution.e[0];
  command->viewport_command.h = SELF->resolution.e[1];

  // Execute the commands.
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF->font_presenter)->val_context, SELF->command_list)) {
    return DX_FAILURE;
  }

  // Update the world, view, and projection matrices.
  DX_MAT4 world_matrix;
  dx_mat4_set_identity(&world_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);

  DX_MAT4 view_matrix;
  dx_mat4_set_identity(&view_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);

  DX_MAT4 projection_matrix;
  dx_mat4_set_ortho(&projection_matrix, 0.f, SELF->resolution.e[0], 0, SELF->resolution.e[1], -1, +1);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);

  return DX_SUCCESS;
}

dx_result dx_ui_manager_leave_render(dx_ui_manager* SELF) {
  if (SELF->root) {
    return on_render_widget(SELF, SELF->root);
  }
  return DX_SUCCESS;
}

// @code
// Ui.Widget : Ui.Group
//             Ui.Text
// @endcode
static dx_result _parse_widget(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node);

// @code
// UI.Widget
// @endcode
static dx_result _parse(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node);

static dx_result _parse_widget(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node) {
  dx_string* type_field_name = NULL;
  if (dx_string_create(&type_field_name, "type", sizeof("type") - 1)) {
    return DX_FAILURE;
  }
  dx_ddl_node* type_node = NULL;
  if (dx_ddl_node_map_get(&type_node, node, type_field_name)) {
    DX_UNREFERENCE(type_field_name);
    type_field_name = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(type_field_name);
  type_field_name = NULL;
  dx_string* received_type = NULL;
  if (dx_ddl_node_get_string(&received_type, type_node)) {
    return DX_FAILURE;
  }
  dx_ui_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &SELF->type_handlers, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_ui_widget* temporary;
  if (dx_ui_type_handler_parse(&temporary, type_handler, SELF, node)) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

static dx_result _parse(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node) {
  return _parse_widget(RETURN, SELF, node);
}

dx_result dx_ui_manager_load(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_string* path) {
  char* bytes = NULL;
  dx_size number_of_bytes = 0;
  if (dx_get_file_contents_s(path, &bytes, &number_of_bytes)) {
    return DX_FAILURE;
  }
  dx_ddl_node* ddl_node = dx_ddl_compile(bytes, number_of_bytes);
  dx_memory_deallocate(bytes);
  bytes = NULL;
  if (!ddl_node) {
    return DX_FAILURE;
  }
  dx_ui_widget* widget = NULL;
  if (_parse(&widget, SELF, ddl_node)) {
    DX_UNREFERENCE(ddl_node);
    ddl_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  *RETURN = widget;
  return DX_SUCCESS;
}
