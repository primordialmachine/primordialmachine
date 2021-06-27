#include "./Shape2.h"

#include "_Math.h"
#include "./Binding.h"
#include "./Video.h"
#include "./UtilitiesGL.h"
#include <linmath.h>
#include <stdio.h>
#include "GL/ShaderProgram.h"

struct Machine_Shape2 {
  Machine_Object parent;
  void (*render)(Machine_Shape2* self, float width, float height);
};

static void Machine_Shape2_visit(Machine_Shape2* self)
{/*Intentionally empty.*/}

void Machine_Shape2_construct(Machine_Shape2* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Object_construct((Machine_Object*)self, numberOfArguments, arguments);
  Machine_setClassType((Machine_Object*)self, Machine_Shape2_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_Shape2, Machine_Object, &Machine_Shape2_visit, Machine_Shape2_construct, NULL)

void Machine_Shape2_render(Machine_Shape2* self, float width, float height) {
  if (!self || !self->render) {
    Machine_setStatus(Machine_Status_InvalidArgument);
    Machine_jump();
  }
  self->render(self, width, height);
}

struct Machine_Rectangle2 {
  Machine_Shape2 __parent;
  Machine_Math_Vector2* position;
  Machine_Math_Vector2* size;
  Machine_ShaderProgram* shader;
  Machine_VideoBuffer* vertices;
  Machine_Binding* binding;
  Machine_Math_Vector4* color;
};

void Machine_Rectangle2_visit(Machine_Rectangle2* self) {
  if (self->position) {
    Machine_visit(self->position);
  }
  if (self->size) {
    Machine_visit(self->size);
  }
  if (self->shader) {
    Machine_visit(self->shader);
  }
  if (self->vertices) {
    Machine_visit(self->vertices);
  }
  if (self->binding) {
    Machine_visit(self->binding);
  }
  if (self->color) {
    Machine_visit(self->color);
  }
}

void Machine_Rectangle2_render(Machine_Rectangle2* self, float width, float height) {
  float ratio = width / height;

  // Set the world matrix, view matrix, and projection matrix.
  mat4x4 world, view, projection, wvp;
  mat4x4_identity(world); // world matrix is identity.
  mat4x4_identity(view); // view matrix is identity.
#if defined(true)
  mat4x4_ortho(projection, 0.f, width, height, 0.f, 1.f, -1.f); // projection matrix
#else
  mat4x4_ortho(projection, 0.f, width, 0.f, height, 1.f, -1.f); // projection matrix
#endif
  // Compute combined world view projection matrix.
  mat4x4_mul(wvp, projection, view);
  mat4x4_mul(wvp, wvp, world);

  float l = Machine_Math_Vector2_getX(self->position);
  float r = l + Machine_Math_Vector2_getX(self->size);
  float b = Machine_Math_Vector2_getY(self->position);
  float t = b + Machine_Math_Vector2_getY(self->size);

  struct {
    float x, y;
  }
  VERTICES[] =
  {
    { l, b, }, // left/bottom
    { r, b, }, // right/bottom
    { l, t, }, // left/top
    { r, t, }, // right/top
  };

  Machine_VideoBuffer_setData(self->vertices, sizeof(VERTICES), (void const *)VERTICES);

  Machine_Binding_activate(self->binding);
  Machine_Video_bindShaderProgram(self->shader);
  {
    Machine_Binding_bindMatrix4x4(self->binding, Machine_String_create("modelToProjectionMatrix", strlen("modelToProjectionMatrix") + 1), wvp);
  }
  {
    Machine_Binding_bindVector4(self->binding, Machine_String_create("mesh_color", strlen("mesh_color") + 1), self->color);
  }

  static const uint8_t indices[] = {
    0, 1, 2,
    2, 1, 3,
  };

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices);
}

void Machine_Rectangle2_construct(Machine_Rectangle2* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Machine_Shape2_construct((Machine_Shape2*)self, numberOfArguments, arguments);
  self->position = Machine_Math_Vector2_create();
  self->size = Machine_Math_Vector2_create();
  ((Machine_Shape2*)self)->render = (void(*)(Machine_Shape2*, float, float))&Machine_Rectangle2_render;

  self->vertices = Machine_Video_createBuffer();
  self->shader = Machine_GL_ShaderProgram_generateShape2Shader();

  Machine_VertexDescriptor* vertexDescriptor = Machine_VertexDescriptor_create();
  Machine_VertexDescriptor_append(vertexDescriptor, Machine_VertexElementSemantics_XfYf);

  self->binding = Machine_Binding_create(self->shader, vertexDescriptor, self->vertices);
  Machine_Binding_setVariableBinding(self->binding, Machine_String_create("vertex_position", strlen("vertex_position") + 1), 0);

  self->color = Machine_Math_Vector4_create();
  Machine_Math_Vector4_set(self->color, 1.f, 1.f, 1.f, 1.f);
  Machine_setClassType((Machine_Object*)self, Machine_Rectangle2_getClassType());
}

MACHINE_DEFINE_CLASSTYPE_EX(Machine_Rectangle2, Machine_Shape2, &Machine_Rectangle2_visit, &Machine_Rectangle2_construct, NULL)

Machine_Rectangle2* Machine_Rectangle2_create() {
  Machine_ClassType* ty = Machine_Rectangle2_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Machine_Rectangle2* self = (Machine_Rectangle2*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  return self;
}

void Machine_Rectangle2_setRectangle(Machine_Rectangle2* self, const Machine_Math_Rectangle2* rectangle) {
  Machine_Math_Vector2_copy(self->position, Machine_Math_Rectangle2_getPosition(rectangle));
  Machine_Math_Vector2_copy(self->size, Machine_Math_Rectangle2_getSize(rectangle));
}

Machine_Math_Rectangle2* Machine_Rectangle2_getRectangle(const Machine_Rectangle2* self) {
  Machine_Math_Rectangle2* rectangle =  Machine_Math_Rectangle2_create();
  Machine_Math_Rectangle2_setPosition(rectangle, self->position);
  Machine_Math_Rectangle2_setSize(rectangle, self->size);
  return rectangle;
}

void Machine_Rectangle2_setColor(Machine_Rectangle2* self, const Machine_Math_Vector4* color) {
  Machine_Math_Vector4_copy(self->color, color);
}

const Machine_Math_Vector4* Machine_Rectangle2_getColor(const Machine_Rectangle2* self) {
  return self->color;
}

void Machine_Rectangle2_setSize(Machine_Rectangle2* self, const Machine_Math_Vector2* size) {
  Machine_Math_Vector2_copy(self->size, size);
}

const Machine_Math_Vector2* Machine_Rectangle2_getSize(const Machine_Rectangle2* self) {
  return self->size;
}

void Machine_Rectangle2_setPosition(Machine_Rectangle2* self, const Machine_Math_Vector2* position) {
  Machine_Math_Vector2_copy(self->position, position);
}

const Machine_Math_Vector2* Machine_Rectangle2_getPosition(const Machine_Rectangle2* self) {
  return self->position;
}
