#include "dx/aal/al/context.h"

#include "dx/aal/al/system.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>

DX_DEFINE_OBJECT_TYPE("dx.aal.al.context",
                      dx_aal_al_context,
                      dx_aal_context);

static dx_result start(dx_aal_al_context* SELF);

static dx_result stop(dx_aal_al_context* SELF);

static void dx_aal_al_context_destruct(dx_aal_al_context* SELF) {
  alDeleteBuffers(1, &SELF->buffer);
  SELF->buffer = 0;
  alcDestroyContext(SELF->context);
  SELF->context = NULL;
}

static void dx_aal_al_context_dispatch_construct(dx_aal_al_context_dispatch* SELF) {
  DX_AAL_CONTEXT_DISPATCH(SELF)->start = (dx_result(*)(dx_aal_context*)) & start;
  DX_AAL_CONTEXT_DISPATCH(SELF)->stop = (dx_result(*)(dx_aal_context*)) & stop;
}

dx_result dx_aal_al_context_construct(dx_aal_al_context * SELF, dx_aal_al_system* system) {
  dx_rti_type* TYPE = dx_aal_al_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_aal_context_construct(DX_AAL_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
  ALCint attributes[] = {
    ALC_MAJOR_VERSION, 1,
    ALC_MINOR_VERSION, 1,
    0, 0,
  };
  SELF->context = alcCreateContext(system->device, attributes);
  if (!SELF->context) {
    return DX_FAILURE;
  }
  if (!alcMakeContextCurrent(SELF->context)) {
    alcDestroyContext(SELF->context);
    SELF->context = NULL;
    return DX_FAILURE;
  }
  //
  ALfloat listener_position[] = { 0.0,0.0,4.0 };
  ALfloat listener_Velocity[] = { 0.0,0.0,0.0 };
  ALfloat listener_orientation[] = { 0.0,0.0,1.0, 0.0,1.0,0.0 };
  alListenerfv(AL_POSITION, listener_position);
  alListenerfv(AL_VELOCITY, listener_Velocity);
  alListenerfv(AL_ORIENTATION, listener_orientation);
  if (AL_NO_ERROR != alGetError()) {
    alcDestroyContext(SELF->context);
    SELF->context = NULL;
    return DX_FAILURE;
  }
  //
  ALfloat source_0_position[] = { -2.0, 0.0, 0.0 };
  ALfloat source_0_velocity[] = { 0.0, 0.0, 0.0 };

  // a 4 second sine wave with a sample rate of 22050.
  // That is, we have 22050 samples per second.
  // That is, we have 4*22050 samples.
  ALenum format = AL_FORMAT_MONO8;
  ALuint duration = 4;
  ALuint sample_rate = 22050;
  float frequency = 220.f; // 220Hz.
  alGenBuffers(1, & SELF->buffer);
  if (AL_NO_ERROR != alGetError()) {
    alcDestroyContext(SELF->context);
    return DX_FAILURE;
  }
  // the number of samples
  // the sample rate (aka samples per second) multiplied by the duration in seconds
  ALuint number_of_samples = sample_rate * duration;

  uint8_t* data = NULL;
  if (dx_memory_allocate(&data, sizeof(uint8_t) * number_of_samples)) {
    alDeleteBuffers(1, &SELF->buffer);
    SELF->buffer = 0;
    alcDestroyContext(SELF->context);
    SELF->context = NULL;
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < number_of_samples; ++i) {
    data[i] = 0;
  }
  alBufferData(SELF->buffer, AL_FORMAT_MONO8, data, sizeof(uint8_t) * sample_rate * duration, sample_rate);
  dx_memory_deallocate(data);
  //
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_aal_al_context_create(dx_aal_al_context** RETURN, dx_aal_al_system* system) {
  DX_CREATE_PREFIX(dx_aal_al_context)
  if (dx_aal_al_context_construct(SELF, system)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result start(dx_aal_al_context* SELF) {
  return DX_SUCCESS;
}

static dx_result stop(dx_aal_al_context* SELF) {
  return DX_SUCCESS;
}
