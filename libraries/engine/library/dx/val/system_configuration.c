// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "dx/val/system_configuration.h"

Core_defineObjectType("dx.val.system_configuration",
                      dx_val_system_configuration,
                      Core_Object);

static void dx_val_system_configuration_destruct(dx_val_system_configuration* SELF)
{/*Intentionally empty.*/}

static void dx_val_system_configuration_constructDispatch(dx_val_system_configuration_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_system_configuration_construct(dx_val_system_configuration* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_system_configuration);
  SELF->sample_buffers = 0;
  SELF->samples = 0;
  SELF->red_bits = 8;
  SELF->green_bits = 8;
  SELF->blue_bits = 8;
  SELF->alpha_bits = 8;
  SELF->depth_bits = 24;
  SELF->stencil_bits = 8;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_system_configuration_create(dx_val_system_configuration** RETURN) {
  DX_CREATE_PREFIX(dx_val_system_configuration);
  if (dx_val_system_configuration_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_sample_buffers(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->sample_buffers;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_sample_buffers(dx_val_system_configuration* SELF, Core_Natural8 sample_buffers) {
  SELF->sample_buffers = sample_buffers;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_samples(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->samples;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_samples(dx_val_system_configuration* SELF, Core_Natural8 samples) {
  SELF->samples = samples;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_red_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->red_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_red_bits(dx_val_system_configuration* SELF, Core_Natural8 red_bits) {
  SELF->red_bits = red_bits;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configurarion_get_green_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->green_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_green_bits(dx_val_system_configuration* SELF, Core_Natural8 green_bits) {
  SELF->green_bits = green_bits;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_blue_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->blue_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_blue_bits(dx_val_system_configuration* SELF, Core_Natural8 blue_bits) {
  SELF->blue_bits = blue_bits;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_alpha_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->alpha_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_alpha_bits(dx_val_system_configuration* SELF, Core_Natural8 alpha_bits) {
  SELF->alpha_bits = alpha_bits;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_depth_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->depth_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_depth_bits(dx_val_system_configuration* SELF, Core_Natural8 depth_bits) {
  SELF->depth_bits = depth_bits;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_stencil_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF) {
  *RETURN = SELF->stencil_bits;
  return Core_Success;
}

Core_Result dx_val_system_configuration_set_stencil_bits(dx_val_system_configuration* SELF, Core_Natural8 stencil_bits) {
  SELF->stencil_bits = stencil_bits;
  return Core_Success;
}
