#include "scene2.h"



#include <stddef.h>
#include <malloc.h>

#include "UtilitiesGl.h"

#include <linmath.h>

#include "Fonts.h"
#include "_Images.h"
#include "_Video.h"
#include "GL/Buffer.h"
#include "GL/Texture.h"
#include "ShaderProgram.h"
#include "VertexDescriptor.h"
#include "Binding.h"
#include "Video.h"


static const struct {
  float x, y;
  float r, g, b;
}
vertices[] =
{
  { -0.6f, -0.4f, 1.f, 0.f, 0.f }, // left/bottom
  { +0.6f, -0.4f, 0.f, 1.f, 0.f }, // right/bottom
  { -0.6f, +0.6f, 0.f, 0.f, 1.f }, // left/top
  { +0.6f, +0.6f, 1.f, 1.f, 1.f }, // right/top
};

static const uint8_t indices[] = {
  0, 1, 2,
  2, 1, 3,
};

struct Scene2 {
  Scene parent;
  Machine_ShaderProgram* shaderProgram;
  GLint mvp_location;
  Machine_Binding* binding;
  Machine_VideoBuffer* vertices;
};

static void Scene2_destruct(Scene2* self);

static void Scene2_visit(Scene2* self) {
  if (self->binding) {
    Machine_visit(self->binding);
  }
  if (self->shaderProgram) {
    Machine_visit(self->shaderProgram);
  }
  if (self->vertices) {
    Machine_visit(self->vertices);
  }
}

MACHINE_DEFINE_CLASSTYPE_EX(Scene2, Scene, &Scene2_visit, &Scene2_construct, NULL)

static void Scene2_onStartup(Scene2* scene) {
  scene->vertices = (Machine_VideoBuffer*)Machine_GL_FloatBuffer_create();
  Machine_VideoBuffer_setData((Machine_VideoBuffer *)scene->vertices, sizeof(vertices), (void const*)vertices);

  scene->shaderProgram = Machine_ShaderProgram_generate(false, true, false, false);
  scene->mvp_location = glGetUniformLocation(scene->shaderProgram->programId, "modelToProjectionMatrix");

  Machine_VertexDescriptor* vd = Machine_VertexDescriptor_create();
  Machine_VertexDescriptor_append(vd, Machine_VertexElementSemantics_XfYf);
  Machine_VertexDescriptor_append(vd, Machine_VertexElementSemantics_RfGfBf);

  scene->binding = Machine_Binding_create(scene->shaderProgram, vd, (Machine_VideoBuffer*)scene->vertices);
  Machine_Binding_set(scene->binding, Machine_String_create_noraise("vertex_position", strlen("vertex_position") + 1), 0);
  Machine_Binding_set(scene->binding, Machine_String_create_noraise("vertex_color", strlen("vertex_color") + 1), 1);
}

static void Scene2_onCanvasSizeChanged(Scene2* self, Machine_CanvasSizeChangedEvent* event) {
}

static void Scene2_onUpdate(Scene2* self, float width, float height) {
  float ratio;
  mat4x4 m, p, mvp;

  ratio = width / height;

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);

  mat4x4_identity(m);
  mat4x4_rotate_Z(m, m, (float)glfwGetTime());
  mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  mat4x4_mul(mvp, p, m);

  Machine_Binding_activate(self->binding);
  Machine_Video_bindShaderProgram(self->shaderProgram);
  Machine_Binding_bindMatrix4x4(self->binding, Machine_String_create("modelToProjectionMatrix", strlen("modelToProjectionMatrix") + 1), mvp);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices);
}

static void Scene2_onShutdown(Scene2* scene) {
  scene->vertices = NULL;
  scene->shaderProgram = NULL;
  scene->binding = NULL;
}

void Scene2_construct(Scene2* self, size_t numberOfArguments, const Machine_Value* arguments) {
  Scene_construct((Scene*)self, numberOfArguments, arguments);
  ((Scene*)self)->onCanvasSizeChanged = (Scene_OnCanvaSizeChangedCallback*)&Scene2_onCanvasSizeChanged;
  ((Scene*)self)->onStartup = (Scene_OnStartupCallback*)&Scene2_onStartup;
  ((Scene*)self)->onUpdate = (Scene_OnUpdateCallback*)&Scene2_onUpdate;
  ((Scene*)self)->onShutdown = (Scene_OnShutdownCallback*)&Scene2_onShutdown;
  Machine_setClassType((Machine_Object*)self, Scene2_getClassType());
}

void Scene2_destruct(Scene2* self) {
  self->binding = NULL;
  self->shaderProgram = NULL;
  self->vertices = NULL;
}

Scene2* Scene2_create() {
  Machine_ClassType* ty = Scene2_getClassType();
  static const size_t NUMBER_OF_ARGUMENTS = 0;
  static const Machine_Value ARGUMENTS[] = { { Machine_ValueFlag_Void, Machine_Void_Void } };
  Scene2* scene = (Scene2*)Machine_allocateClassObject(ty, NUMBER_OF_ARGUMENTS, ARGUMENTS);
  if (!scene) {
    Machine_setStatus(Machine_Status_AllocationFailed);
    Machine_jump();
  }
  return scene;
}
