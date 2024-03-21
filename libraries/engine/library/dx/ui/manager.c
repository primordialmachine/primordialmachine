#include "dx/ui/manager.h"

#include "dx/val/command.h"

#include "dx/ui/group.h"
#include "dx/ui/type_handlers/group.h"

#include "dx/ui/text_field.h"
#include "dx/ui/type_handlers/text_field.h"

#include "dx/data_definition_language.h"

// strlen()
#include <string.h>

Core_defineObjectType("dx.ui.manager",
                      dx_ui_manager,
                      Core_Object);

/// @ingroup type_handlers
static Core_Result compare_keys_callback(Core_Boolean* RETURN, Core_String** a, Core_String** b);

/// @ingroup type_handlers
static Core_Result hash_key_callback(Core_Size* RETURN, Core_String** a);

/// @ingroup type_handlers
static void key_added_callback(Core_String** a);

/// @ingroup type_handlers
static void key_removed_callback(Core_String** a);

/// @ingroup type_handlers
static void value_added_callback(Core_Object** a);

/// @ingroup type_handlers
static void value_removed_callback(Core_Object** a);

/// @ingroup type_handlers
static Core_Result initialize_type_handlers(dx_ui_manager* SELF);

/// @ingroup type_handlers
static Core_Result uninitialize_type_handlers(dx_ui_manager* SELF);

static Core_Result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group);

static Core_Result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget);

static Core_Result compare_keys_callback(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result hash_key_callback(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static void key_added_callback(Core_String** a) {
  CORE_REFERENCE(*a);
}

static void key_removed_callback(Core_String** a) {
  CORE_UNREFERENCE(*a);
}

static void value_added_callback(Core_Object** a) {
  CORE_REFERENCE(*a);
}

static void value_removed_callback(Core_Object** a) {
  CORE_UNREFERENCE(*a);
}

static Core_Result initialize_type_handlers(dx_ui_manager* SELF) {
  static Core_InlineHashMapPP_Configuration const configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&compare_keys_callback,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&hash_key_callback,
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&key_added_callback,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&key_removed_callback,
    .valueAddedCallback = (Core_InlineHashMapPP_ValueAddedCallback*)&value_added_callback,
    .valueRemovedCallback = (Core_InlineHashMapPP_ValueRemovedCallback*)&value_removed_callback,
  };
  if (Core_InlineHashMapPP_initialize(&SELF->type_handlers, &configuration)) {
    return Core_Failure;
  }

#define DEFINE(KEY, VALUE) \
    { \
      Core_String* k = NULL; \
      if (Core_String_create(&k, KEY, strlen(KEY))) { \
        return Core_Failure; \
      } \
      dx_ui_type_handler* v = NULL; \
      if (VALUE##_create((VALUE**)&v)) { \
        CORE_UNREFERENCE(k); \
        k = NULL; \
        Core_InlineHashMapPP_uninitialize(&SELF->type_handlers); \
        return Core_Failure; \
      } \
      if (Core_InlineHashMapPP_set(&SELF->type_handlers, k, v)) { \
        CORE_UNREFERENCE(v); \
        v = NULL; \
        CORE_UNREFERENCE(k); \
        k = NULL; \
        Core_InlineHashMapPP_uninitialize(&SELF->type_handlers); \
        return Core_Failure; \
      } \
      CORE_UNREFERENCE(v); \
      v = NULL; \
      CORE_UNREFERENCE(k); \
      k = NULL; \
    }

    DEFINE("Group", dx_ui_type_handlers_group);
    DEFINE("TextField", dx_ui_type_handlers_text_field);

  #undef DEFINE

    return Core_Success;
}

static Core_Result uninitialize_type_handlers(dx_ui_manager* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->type_handlers);
  return Core_Success;
}

static void dx_ui_manager_destruct(dx_ui_manager* SELF) {
  uninitialize_type_handlers(SELF);
  CORE_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;
  CORE_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;
  CORE_UNREFERENCE(SELF->command_list);
  SELF->command_list = NULL;
}

static void dx_ui_manager_constructDispatch(dx_ui_manager_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_ui_manager_construct(dx_ui_manager* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CONSTRUCT_PREFIX(dx_ui_manager);

  if (dx_val_command_list_create(&SELF->command_list)) {
    return Core_Failure;
  }

  // create the set viewport command
  dx_val_command* command = NULL;
  if (dx_val_command_create_viewport(&command, 0.f, 0.f, 640.f, 480.f)) {
    CORE_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return Core_Failure;
  }
  if (dx_val_command_list_append(SELF->command_list, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    CORE_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  SELF->font_presenter = font_presenter;
  CORE_REFERENCE(font_presenter);

  SELF->rectangle_presenter = rectangle_presenter;
  CORE_REFERENCE(rectangle_presenter);
  
  if (initialize_type_handlers(SELF)) {
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    CORE_UNREFERENCE(SELF->font_presenter);
    SELF->font_presenter = NULL;
    CORE_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return Core_Failure;
  }

  dx_vec2_f32_set(&SELF->dpi, 96.f, 96.f);
  
  dx_vec2_f32_set(&SELF->resolution, 640, 480);
  
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_manager_create(dx_ui_manager** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_ui_manager)
    if (dx_ui_manager_construct(SELF, font_presenter, rectangle_presenter)) {
      CORE_UNREFERENCE(SELF);
      SELF = NULL;
      return Core_Failure;
    }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_ui_manager_set_canvas_dpi(dx_ui_manager* SELF, Core_InlineVector2R32 const* dpi) {
  SELF->dpi = *dpi;
  return Core_Success;
}

Core_Result dx_ui_manager_set_canvas_resolution(dx_ui_manager* SELF, Core_InlineVector2R32 const* resolution) {
  SELF->resolution = *resolution;
  return Core_Success;
}

Core_Result dx_ui_manager_set_root(dx_ui_manager* SELF, dx_ui_widget* root) {
  SELF->root = root;
  return Core_Success;
}

static Core_Result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group) {
  Core_Size n;
  if (dx_object_array_get_size(&n, group->children)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_ui_widget* widget = NULL;
    if (dx_object_array_get_at((Core_Object**)&widget, group->children, i)) {
      return Core_Failure;
    }
    if (on_render_widget(SELF, widget)) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static Core_Result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget) {
  if (dx_ui_widget_render(widget)) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (dx_ui_group_getType(&type)) {
    return Core_Failure;
  }
  Core_Boolean result = Core_False;
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(widget)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    if (on_render_group_children(SELF, DX_UI_GROUP(widget))) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result dx_ui_manager_enter_render(dx_ui_manager* SELF) {
  dx_val_command* command = NULL;

  // Update the viewport command.
  command = dx_val_command_list_get_at(SELF->command_list, 0);
  command->viewport_command.w = SELF->resolution.v.e[0];
  command->viewport_command.h = SELF->resolution.v.e[1];

  // Execute the commands.
  if (Core_Visuals_Context_executeCommands(DX_PRESENTER(SELF->font_presenter)->val_context, SELF->command_list)) {
    return Core_Failure;
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
  dx_mat4_set_ortho(&projection_matrix, 0.f, SELF->resolution.v.e[0], 0, SELF->resolution.v.e[1], -1, +1);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);

  return Core_Success;
}

Core_Result dx_ui_manager_leave_render(dx_ui_manager* SELF) {
  if (SELF->root) {
    return on_render_widget(SELF, SELF->root);
  }
  return Core_Success;
}

// @code
// Ui.Widget : Ui.Group
//             Ui.Text
// @endcode
static Core_Result _parse_widget(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node);

// @code
// UI.Widget
// @endcode
static Core_Result _parse(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node);

static Core_Result _parse_widget(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node) {
  Core_String* type_field_name = NULL;
  if (Core_String_create(&type_field_name, "type", sizeof("type") - 1)) {
    return Core_Failure;
  }
  dx_ddl_node* type_node = NULL;
  if (dx_ddl_node_map_get(&type_node, node, type_field_name)) {
    CORE_UNREFERENCE(type_field_name);
    type_field_name = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(type_field_name);
  type_field_name = NULL;
  Core_String* received_type = NULL;
  if (dx_ddl_node_get_string(&received_type, type_node)) {
    return Core_Failure;
  }
  dx_ui_type_handler* type_handler = NULL;
  if (Core_InlineHashMapPP_get(&type_handler, &SELF->type_handlers, received_type)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  dx_ui_widget* temporary;
  if (dx_ui_type_handler_parse(&temporary, type_handler, SELF, node)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

static Core_Result _parse(dx_ui_widget** RETURN, dx_ui_manager* SELF, dx_ddl_node* node) {
  return _parse_widget(RETURN, SELF, node);
}

Core_Result dx_ui_manager_load(dx_ui_widget** RETURN, dx_ui_manager* SELF, Core_String* path) {
  char* bytes = NULL;
  Core_Size number_of_bytes = 0;
  if (Core_getFileContents(path, &bytes, &number_of_bytes)) {
    return Core_Failure;
  }
  dx_ddl_node* ddl_node = dx_ddl_compile(bytes, number_of_bytes);
  Core_Memory_deallocate(bytes);
  bytes = NULL;
  if (!ddl_node) {
    return Core_Failure;
  }
  dx_ui_widget* widget = NULL;
  if (_parse(&widget, SELF, ddl_node)) {
    CORE_UNREFERENCE(ddl_node);
    ddl_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  *RETURN = widget;
  return Core_Success;
}
