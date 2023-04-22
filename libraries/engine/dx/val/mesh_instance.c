#include "dx/val/mesh_instance.h"

#include "dx/val/command.h"

DX_DEFINE_OBJECT_TYPE("dx.val.mesh_instance",
                      dx_val_mesh_instance,
                      dx_object)

static dx_val_cbinding* create_cbinding(dx_val_mesh_instance* self) {
  dx_val_cbinding* cbinding = dx_val_cbinding_create();
  if (!cbinding) {
    return NULL;
  }
  dx_val_cbinding_set_rgba_f32(cbinding, "vs_mesh_ambient_rgba", &self->mesh->material->ambient_color);
  dx_val_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &self->world_matrix);
  // The ambient texture sampler is always the zero-th ambient texture sampler.
  // Do this regardless of wether the mesh material has an ambient texture or not.
  dx_val_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return cbinding;
}

static dx_result add_to_backend(dx_val_mesh_instance* self) {
  if (dx_val_command_list_create(&self->commands)) {
    return DX_FAILURE;
  }
  dx_val_cbinding* cbinding = create_cbinding(self);
  if (!cbinding) {
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return DX_FAILURE;
  }
  dx_val_command* command = NULL;
  if (dx_val_command_create_draw(&command, self->mesh->vbinding,
                                           self->mesh->material ? self->mesh->material->ambient_texture : NULL,
                                           cbinding,
                                           self->mesh->program,
                                           0,
                                           self->mesh->asset_mesh->number_of_vertices)) {
    DX_UNREFERENCE(cbinding);
    cbinding = NULL;
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(cbinding);
  cbinding = NULL;
  if (dx_val_command_list_append(self->commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    DX_UNREFERENCE(self->commands);
    self->commands = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;
  return DX_SUCCESS;
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

static void dx_val_mesh_instance_dispatch_construct(dx_val_mesh_instance_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_val_mesh_instance_construct(dx_val_mesh_instance* self, DX_MAT4 world_matrix, dx_val_mesh* mesh) {
  if (!self || !mesh) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_val_mesh_instance_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  self->commands = NULL;
  self->world_matrix = world_matrix;
  self->mesh = mesh;
  DX_REFERENCE(self->mesh);
  if (add_to_backend(self)) {
    DX_UNREFERENCE(self->mesh);
    self->mesh = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(self)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_val_mesh_instance_create(dx_val_mesh_instance** RETURN, DX_MAT4 world_matrix, dx_val_mesh* mesh) {
  dx_val_mesh_instance* SELF = DX_VAL_MESH_INSTANCE(dx_object_alloc(sizeof(dx_val_mesh_instance)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_val_mesh_instance_construct(SELF, world_matrix, mesh)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_val_cbinding* dx_val_mesh_instance_get_cbinding(dx_val_mesh_instance* TYPE) {
  dx_val_command* command = dx_val_command_list_get_at(TYPE->commands, 0);
  return command->draw_command.cbinding;
}

dx_result dx_val_mesh_instance_update_cbinding(dx_val_mesh_instance* TYPE, dx_val_cbinding* cbinding) {
  dx_val_cbinding_set_rgba_f32(cbinding, "vs_mesh_ambient_rgba", &TYPE->mesh->material->ambient_color);
  dx_val_cbinding_set_mat4(cbinding, "vs_mesh_world_matrix", &TYPE->world_matrix);
  dx_val_cbinding_set_texture_index(cbinding, "ambient_texture_sampler", 0);
  return DX_SUCCESS;
}
