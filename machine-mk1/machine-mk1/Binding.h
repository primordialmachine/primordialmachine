#if !defined(MACHINE_BINDING_H_INCLUDED)
#define MACHINE_BINDING_H_INCLUDED



#include "_Video.h"
#include "ShaderProgram.h"
#include "VertexDescriptor.h"
#include "_Math.h"
#include <linmath.h>



/// @brief Binding between shader program inputs and vertex elements.
/// Not EVERY vertex element needs to have a corresponding shader program input.
/// However EVERY shader program input needs to have a corresponding vertex element.
MACHINE_DECLARE_CLASSTYPE(Machine_Binding)

Machine_Binding* Machine_Binding_create(Machine_ShaderProgram* program, Machine_VertexDescriptor* vertexDescriptor, Machine_VideoBuffer* buffer);

bool Machine_Binding_set(Machine_Binding* self, Machine_String* name, size_t index);

size_t Machine_Binding_get(Machine_Binding* self, Machine_String* name);

void Machine_Binding_activate(Machine_Binding* self);

/**
 * @brief Bind a matrix to a constant.
 * @param self This binding.
 * @param name The name of the constant.
 * @param value The value.
 */
void Machine_Binding_bindMatrix4x4(Machine_Binding* self, const Machine_String* name, const mat4x4 value);

/**
 * @brief Bind a vector to a constant.
 * @param self This binding.
 * @param name The name of the constant.
 * @param value The value.
 */
void Machine_Binding_bindVector2(Machine_Binding* self, const Machine_String* name, const Machine_Math_Vector2* value);

/**
 * @brief Bind a vector to a constant.
 * @param self This binding.
 * @param name The name of the constant.
 * @param value The value.
 */
void Machine_Binding_bindVector3(Machine_Binding* self, const Machine_String* name, const Machine_Math_Vector3* value);

/**
 * @brief Bind a vector to a constant.
 * @param self This binding.
 * @param name The name of the constant.
 * @param value The value.
 */
void Machine_Binding_bindVector4(Machine_Binding* self, const Machine_String* name, const Machine_Math_Vector4* value);

/**
 * @todo Remove this.
 */
DEPRECATED void Machine_Binding_bindVector4f(Machine_Binding* self, const Machine_String* name, const vec4 value);

/**
 * @brief Bind a texture unit index to a texture sampler.
 * @param self This binding.
 * @param name The name of the constant.
 * @param value The value.
 */
void Machine_Binding_bindSampler(Machine_Binding* self, const Machine_String* name, const size_t value);

#endif // MACHINE_BINDING_H_INCLUDED
