// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(DX_VAL_SYSTEM_CONFIGURATION_H_INCLUDED)
#define DX_VAL_SYSTEM_CONFIGURATION_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("dx.val.system_configuration",
                       dx_val_system_configuration,
                       Core_Object);
                      
static inline dx_val_system_configuration* DX_VAL_SYSTEM_CONFIGURATION(void* p) {
  return (dx_val_system_configuration*)p;
}
                      
struct dx_val_system_configuration {
  Core_Object _parent;
  Core_Natural8 sample_buffers; /* Number of sample buffers. Default is 0. */
  Core_Natural8 samples; /* Number of samples. Default is 0. */
  Core_Natural8 red_bits, green_bits, blue_bits, alpha_bits; /* Red, green, blue, and alpha bits. Default is 8. */
  Core_Natural8 depth_bits; /* Depth bits. Default is 24. */
  Core_Natural8 stencil_bits; /* Stencil bits. Default is 8. */
};

static inline dx_val_system_configuration_Dispatch* DX_VAL_SYSTEM_CONFIGURATION_DISPATCH(void* p) {
  return (dx_val_system_configuration_Dispatch*)p;
}

struct dx_val_system_configuration_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_val_system_configuration_construct(dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_create(dx_val_system_configuration** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_sample_buffers(Core_Natural8* RETURN, dx_val_system_configuration *SELF);

Core_Result dx_val_system_configuration_set_sample_buffers(dx_val_system_configuration* SELF, Core_Natural8 sample_buffers);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_samples(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_samples(dx_val_system_configuration* SELF, Core_Natural8 samples);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_red_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_red_bits(dx_val_system_configuration* SELF, Core_Natural8 red_bits);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configurarion_get_green_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_green_bits(dx_val_system_configuration* SELF, Core_Natural8 green_bits);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_blue_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_blue_bits(dx_val_system_configuration* SELF, Core_Natural8 blue_bits);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_alpha_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_alpha_bits(dx_val_system_configuration* SELF, Core_Natural8 alpha_bits);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_depth_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_depth_bits(dx_val_system_configuration* SELF, Core_Natural8 depth_bits);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result dx_val_system_configuration_get_stencil_bits(Core_Natural8* RETURN, dx_val_system_configuration* SELF);

Core_Result dx_val_system_configuration_set_stencil_bits(dx_val_system_configuration* SELF, Core_Natural8 stencil_bits);

#endif // DX_VAL_SYSTEM_CONFIGURATION_H_INCLUDED
