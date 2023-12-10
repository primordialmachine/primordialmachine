#include "dx/val/mesh_instance.h"

#include "dx/val/command.h"

DX_DEFINE_OBJECT_TYPE("dx.val.mesh_instance",
                      dx_val_mesh_instance,
                      Core_Object);

static dx_val_cbinding* create_cbinding(dx_val_mesh_instance* self) {
  dx_val_cbinding* cbinding = NULL;
  if (dx_val_cbinding_create(&cbinding)) {
    return NULL;
  }
  dx_val_cbinding_set_rgba_f32(cbinding, "vs_mesh_ambient_rgba", &self->mesh->material->ambient_color);
  dx_val_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &self->world_matrix);
  // The ambient texture sampler is always the zero-th ambient texture sampler.
  // Do this regardless of wether the mesh material has an ambient texture or not.
  dx_val_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return cbinding;
}

static Core_Result add_to_backend(dx_val_mesh_instance* self) {
  if (dx_val_command_list_create(&self->commands)) {
    return Core_Failure;
  }
  dx_val_cbinding* cbinding = create_cbinding(self);
  if (!cbinding) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return Core_Failure;
  }
  dx_val_command* command = NULL;
  if (dx_val_command_create_draw(&command, self->mesh->vbinding,
                                           self->mesh->material ? self->mesh->material->ambient_texture : NULL,
                                           cbinding,
                                           self->mesh->program,
                                           0,
                                           self->mesh->mesh_asset->number_of_vertices)) {
    DX_UNREFERENCE(cbinding);
    cbinding = NULL;
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(cbinding);
  cbinding = NULL;
  if (dx_val_command_list_append(self->commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  return Core_Success;
}

static void remove_from_backend(dx_val_mesh_instance* self) {
  if (self->commands) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
  }
}

static void dx_val_mesh_instance_destruct(dx_val_mesh_instance* self) {
  remove_from_backend(self);
  if (self->mesh) {
    DX_UNREFERENCE(self->mesh);
    self->mesh = NULL;
  }
}

static void dx_val_mesh_instance_constructDispatch(dx_val_mesh_instance_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_val_mesh_instance_construct(dx_val_mesh_instance* SELF, DX_MAT4 world_matrix, dx_val_mesh* mesh) {
  DX_CONSTRUCT_PREFIX(dx_val_mesh_instance);
  if (!mesh) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->commands = NULL;
  SELF->world_matrix = world_matrix;
  SELF->mesh = mesh;
  DX_REFERENCE(SELF->mesh);
  if (add_to_backend(SELF)) {
    DX_UNREFERENCE(SELF->mesh);
    SELF->mesh = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_mesh_instance_create(dx_val_mesh_instance** RETURN, DX_MAT4 world_matrix, dx_val_mesh* mesh) {
  DX_CREATE_PREFIX(dx_val_mesh_instance);
  if (dx_val_mesh_instance_construct(SELF, world_matrix, mesh)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

dx_val_cbinding* dx_val_mesh_instance_get_cbinding(dx_val_mesh_instance* TYPE) {
  dx_val_command* command = dx_val_command_list_get_at(TYPE->commands, 0);
  return command->draw_command.cbinding;
}

Core_Result dx_val_mesh_instance_update_cbinding(dx_val_mesh_instance* TYPE, dx_val_cbinding* cbinding) {
  dx_val_cbinding_set_rgba_f32(cbinding, "vs_mesh_ambient_rgba", &TYPE->mesh->material->ambient_color);
  dx_val_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &TYPE->world_matrix);
  dx_val_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return Core_Success;
}
