#include "./../Text/Layout.h"

#include "./../Fonts.h"
#include "./../Video.h"
#include "./../Text/LayoutLine.h"

static const float OFFSET_X = 0.f;

static const float OFFSET_Y = 0.f;

#define WITH_SNAPTOGRID (1)

#define Y_UP (1)

static void visit(Machine_Text_Layout* self) {
  if (self->color) {
    Machine_visit(self->color);
  }
  if (self->position) {
    Machine_visit(self->position);
  }
  if (self->font) {
    Machine_visit(self->font);
  }
  if (self->text) {
    Machine_visit(self->text);
  }
  if (self->lines) {
    Machine_visit(self->lines);
  }
  if (self->visualBounds) {
    Machine_visit(self->visualBounds);
  }
  if (self->clipRectangle) {
    Machine_visit(self->clipRectangle);
  }
}

static void parse(Machine_String* text, Machine_PointerArray* lines) {
  const char* start = Machine_String_getBytes(text),
    * end = Machine_String_getBytes(text) + Machine_String_getNumberOfBytes(text);

  const char* lineStart = start,
    * lineEnd = start;

  while (lineEnd != end) {
    // terminates a line
    if (*lineEnd == '\n' || *lineEnd == '\r') {
      int old = *lineEnd;
      lineEnd++;
      if (old != *lineEnd && (*lineEnd == '\n' || *lineEnd == '\r')) {
        lineEnd++;
      }
      Machine_Text_LayoutLine* line = Machine_Text_LayoutLine_create(lineStart - start, lineEnd - lineStart);
      Machine_PointerArray_append(lines, line);
      lineStart = lineEnd;
    }
    lineEnd++;
  }
  if (lineEnd != lineStart) {
    Machine_Text_LayoutLine* line = Machine_Text_LayoutLine_create(lineStart - start, lineEnd - lineStart);
    Machine_PointerArray_append(lines, line);
  }
}

static void updateLinesBounds(Machine_Math_Vector2 *position, Machine_Fonts_Font* font, Machine_String* text, Machine_PointerArray* lines, bool yup) {
#if defined(WITH_SNAPTOGRID)
  // Snap to pixel (ensure there are no artifacts).
  vec2 position0 = { floorf(Machine_Math_Vector2_getX(position)),
                     floorf(Machine_Math_Vector2_getY(position)) };
#else
  vec2 position0 = { (Machine_Math_Vector2_getX(position)),
                     (Machine_Math_Vector2_getY(position)) };
#endif

  vec2 cursorPosition = { 0, 0 };

  Machine_Math_Rectangle2* lineBounds = Machine_Math_Rectangle2_create();

  for (size_t i = 0, n = Machine_PointerArray_getSize(lines); i < n; ++i) {
    Machine_Text_LayoutLine* layoutLine = (Machine_Text_LayoutLine*)Machine_PointerArray_getAt(lines, i);
    const char* bytes = Machine_String_getBytes(text);

    Machine_Math_Vector2* position = Machine_Math_Vector2_create();
    Machine_Math_Vector2_set(position, cursorPosition[0], cursorPosition[1]);
    Machine_Math_Vector2* size = Machine_Math_Vector2_create();
    Machine_Math_Vector2_set(size, 0.f, 0.f);

    Machine_Math_Rectangle2_setPosition(lineBounds, position);
    Machine_Math_Rectangle2_setSize(lineBounds, size);

    Machine_Math_Rectangle2* symbolBounds = Machine_Math_Rectangle2_create();
    for (size_t j = layoutLine->start, m = layoutLine->start + layoutLine->length; j < m; ++j) {
      uint32_t codepoint = bytes[j];
      vec2 symbolAdvance;
      Machine_Texture* symbolTexture;
      bool skip = !Machine_Font_getCodePointInfo(font, codepoint, symbolBounds, symbolAdvance, &symbolTexture);
      if (skip) {
        continue;
      }

      float l = cursorPosition[0] + Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getPosition(symbolBounds));
      float r = l + Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(symbolBounds));
      float t, b;
      if (yup) {
        t = cursorPosition[1] + (Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds)) - Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getPosition(symbolBounds)));
        b = t - Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds));
      } else {
        t = cursorPosition[1] - (Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds)) - Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getPosition(symbolBounds)));
        b = t + Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds));
      }

      Machine_Math_Vector2* min = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(min, l, t);
      Machine_Math_Vector2* max = Machine_Math_Vector2_create();
      Machine_Math_Vector2_set(max, r, b);
      Machine_Math_Rectangle2_addPoint(lineBounds, min);
      Machine_Math_Rectangle2_addPoint(lineBounds, max);

      vec2_add(cursorPosition, cursorPosition, symbolAdvance);
    }
    layoutLine->left = Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getPosition(lineBounds));
    layoutLine->width = Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(lineBounds));
    layoutLine->top = Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getPosition(lineBounds));
    layoutLine->height = Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(lineBounds));

    cursorPosition[0] = 0;
    cursorPosition[1] += Machine_Font_getBaselineDistance(font);
  }

  for (size_t i = 0, n = Machine_PointerArray_getSize(lines); i < n; ++i) {
    Machine_Text_LayoutLine* layoutLine = (Machine_Text_LayoutLine*)Machine_PointerArray_getAt(lines, i);
    layoutLine->top += position0[1];
    layoutLine->left += position0[0];
  }
}

static void updateBounds(Machine_Text_Layout* self) {
#if defined(WITH_SNAPTOGRID)
  // Snap to pixel (ensure there are no artifacts).
  vec2 position0 = { floorf(Machine_Math_Vector2_getX(self->position)),
                     floorf(Machine_Math_Vector2_getY(self->position)) };
#else
  vec2 position0 = { (Machine_Math_Vector2_getX(self->position)),
                     (Machine_Math_Vector2_getY(self->position)) };
#endif

  vec2 cursorPosition = { position0[0], position0[1] };

  Machine_Math_Rectangle2* bounds = Machine_Math_Rectangle2_create();
  Machine_Math_Vector2* p = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(p, cursorPosition[0], cursorPosition[1]);
  Machine_Math_Rectangle2_setPosition(bounds, p);

  const char* bytes = Machine_String_getBytes(self->text);

  for (size_t i = 0, n = Machine_PointerArray_getSize(self->lines); i < n; ++i) {
    Machine_Text_LayoutLine* layoutLine = (Machine_Text_LayoutLine*)Machine_PointerArray_getAt(self->lines, i);
    Machine_Math_Vector2_set(p, layoutLine->left, layoutLine->top);
    Machine_Math_Rectangle2_addPoint(bounds, p);
    Machine_Math_Vector2_set(p, layoutLine->left + layoutLine->width, layoutLine->top + layoutLine->height);
    Machine_Math_Rectangle2_addPoint(bounds, p);

    cursorPosition[0] = Machine_Math_Vector2_getX(self->position);
    cursorPosition[1] += Machine_Font_getBaselineDistance(self->font);
  }

  if (!self->visualBounds) {
    self->visualBounds = Machine_Rectangle2_create();
  }
  Machine_Rectangle2_setRectangle(self->visualBounds, bounds);
}

Machine_Text_Layout* Machine_Text_Layout_create(Machine_String* text, Machine_Fonts_Font* font) {
  if (text == NULL) {
    Machine_setStatus(Machine_Status_InvalidArgument);
    Machine_jump();
  }
  Machine_Text_Layout* self = Machine_allocate(sizeof(Machine_Text_Layout), (Machine_VisitCallback*)&visit, NULL);
  if (!self) {
    Machine_setStatus(Machine_Status_AllocationFailed);
    Machine_jump();
  }
  self->color = Machine_Math_Vector3_create();
  Machine_Math_Vector3_set(self->color, 0.f, 0.f, 0.f);
  self->position = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(self->position, 0.f, 0.f);
  self->font = font;
  self->text = text;
  self->lines = Machine_PointerArray_create();
  self->yup = true;
  self->flags |= LINES_DIRTY;
  return self;
}

const Machine_Math_Rectangle2* Machine_Text_Layout_getBounds(Machine_Text_Layout* self) {
  if ((self->flags & LINES_DIRTY) == LINES_DIRTY) {
    Machine_PointerArray_clear(self->lines);
    parse(self->text, self->lines);
    self->flags |= LINE_BOUNDS_DIRTY;
    self->flags &= ~LINES_DIRTY;
  }
  if ((self->flags & LINE_BOUNDS_DIRTY) == LINE_BOUNDS_DIRTY) {
    updateLinesBounds(self->position, self->font, self->text, self->lines, self->yup);
    self->flags |= BOUNDS_DIRTY;
    self->flags &= ~LINE_BOUNDS_DIRTY;
  }
  if ((self->flags & BOUNDS_DIRTY) == BOUNDS_DIRTY) {
    updateBounds(self);
    self->flags &= ~BOUNDS_DIRTY;
  }
  return Machine_Rectangle2_getRectangle(self->visualBounds);
}

void Machine_Text_Layout_render(Machine_Text_Layout* self, float width, float height) {
  if ((self->flags & LINES_DIRTY) == LINES_DIRTY) {
    Machine_PointerArray_clear(self->lines);
    parse(self->text, self->lines);
    self->flags |= LINE_BOUNDS_DIRTY;
    self->flags &= ~LINES_DIRTY;
  }
  if ((self->flags & LINE_BOUNDS_DIRTY) == LINE_BOUNDS_DIRTY) {
    updateLinesBounds(self->position, self->font, self->text, self->lines, self->yup);
    self->flags |= BOUNDS_DIRTY;
    self->flags &= ~LINE_BOUNDS_DIRTY;
  }
  if ((self->flags & BOUNDS_DIRTY) == BOUNDS_DIRTY) {
    updateBounds(self);
    self->flags &= ~BOUNDS_DIRTY;
  }

  if (self->clipRectangle) {
    glEnable(GL_CLIP_DISTANCE0);
    glEnable(GL_CLIP_DISTANCE1);
    glEnable(GL_CLIP_DISTANCE2);
    glEnable(GL_CLIP_DISTANCE3);
  }
  if (self->renderVisualBounds) {
    if (!self->visualBounds) {
      updateBounds(self);
    }
    Machine_Math_Vector4* color = Machine_Math_Vector4_create();
    Machine_Math_Vector4_set(color, .3f, .6f, .3f, 1.f);
    Machine_Rectangle2_setColor(self->visualBounds, color);
    Machine_Shape2_render((Machine_Shape2*)self->visualBounds, width, height);
  }

#if defined(WITH_SNAPTOGRID)
  // Snap to pixel (ensure there are no artifacts).
  vec2 position0 = { floorf(Machine_Math_Vector2_getX(self->position)),
                     floorf(Machine_Math_Vector2_getY(self->position)) };
#else
  vec2 position0 = { Machine_Math_Vector2_getX(self->position),
                     Machine_Math_Vector2_getY(self->position) };
#endif

  // Set the world matrix, view matrix, and projection matrix.
  mat4x4 world, view, projection, wvp;
  mat4x4_identity(world); // world matrix is identity.
  mat4x4_identity(view); // view matrix is identity.
#if defined(Y_UP)
  mat4x4_ortho(projection, 0.f, width, height, 0.f, 1.f, -1.f); // projection matrix
#else
  mat4x4_ortho(projection, 0.f, width, 0.f, height, 1.f, -1.f); // projection matrix
#endif
  // Compute combined world view projection matrix.
  mat4x4_mul(wvp, projection, view);
  mat4x4_mul(wvp, wvp, world);

  Machine_ShaderProgram* shaderProgram = Machine_Font_getShaderProgram(self->font);
  Machine_Binding* binding = Machine_Font_getBinding(self->font);

  Machine_Binding_activate(binding);
  Machine_Video_bindShaderProgram(shaderProgram);
  if (self->clipRectangle) {
    Machine_Math_Vector2* position = Machine_Math_Rectangle2_getPosition(self->clipRectangle);
    const Machine_Math_Vector2* size = Machine_Math_Rectangle2_getSize(self->clipRectangle);
    {
      vec3 n = { -1, 0, 0 };
      vec3 p = { Machine_Math_Vector2_getX(position), 0, 0 };
      float d = -vec3_mul_inner(n, p);

      vec4 x = { -1, 0, 0, d };
      Machine_Binding_bindVector4f(binding, Machine_String_create("clipPlane0", strlen("clipPlane0") + 1), x);
    }
    {
      vec3 n = { +1, 0, 0 };
      vec3 p = { Machine_Math_Vector2_getX(position) + Machine_Math_Vector2_getX(size), 0, 0 };
      float d = -vec3_mul_inner(n, p);

      vec4 x = { +1, 0, 0, d };
      Machine_Binding_bindVector4f(binding, Machine_String_create("clipPlane1", strlen("clipPlane1") + 1), x);
    }
  }
  Machine_Binding_bindVector3(binding, Machine_String_create("mesh_color", strlen("mesh_color") + 1), self->color);
  Machine_Binding_bindMatrix4x4(binding, Machine_String_create("modelToWorldMatrix", strlen("modelToWorldMatrix") + 1), world);
  Machine_Binding_bindMatrix4x4(binding, Machine_String_create("modelToProjectionMatrix", strlen("modelToProjectionMatrix") + 1), wvp);

  Machine_Video_setDepthTestFunction(Machine_DepthTestFunction_Always);
  Machine_Video_setDepthWriteEnabled(false);
  Machine_Video_setIncomingBlendFunction(Machine_BlendFunction_IncomingAlpha);
  Machine_Video_setExistingBlendFunction(Machine_BlendFunction_OneMinusIncomingAlpha);
  
  vec2 cursorPosition = { position0[0], position0[1] };

  const char* bytes = Machine_String_getBytes(self->text);

  Machine_Math_Rectangle2* symbolBounds = Machine_Math_Rectangle2_create();
  for (size_t i = 0, n = Machine_PointerArray_getSize(self->lines); i < n; ++i) {
    Machine_Text_LayoutLine* layoutLine = (Machine_Text_LayoutLine*)Machine_PointerArray_getAt(self->lines, i);
    for (size_t j = layoutLine->start, m = layoutLine->start + layoutLine->length; j < m; ++j) {
      uint32_t codepoint = bytes[j];
      vec2 symbolAdvance;
      Machine_Texture* symbolTexture;
      bool skip = !Machine_Font_getCodePointInfo(self->font, codepoint, symbolBounds, symbolAdvance, &symbolTexture);
      if (skip) continue;

      float l = OFFSET_X + cursorPosition[0] + Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getPosition(symbolBounds));
      float r = l + Machine_Math_Vector2_getX(Machine_Math_Rectangle2_getSize(symbolBounds));
      float t, b;
    #if defined(Y_UP)
      t = OFFSET_Y + cursorPosition[1] + (Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds)) - Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getPosition(symbolBounds)));
      b = t - Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds));
    #else
      t = OFFSET_Y + cursorPosition[1] - (Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds)) - symbolBounds.b);
      b = t + Machine_Math_Vector2_getY(Machine_Math_Rectangle2_getSize(symbolBounds));
    #endif

      const struct {
        float x, y;
        float u, v;
      } vertices[] = {
        { l,  t,  0.f, 1.f }, // left/bottom
        { r,  t,  1.f, 1.f }, // right/bottom
        { l,  b,  0.f, 0.f }, // left/top
        { r,  b,  1.f, 0.f }, // right/top
      };

      Machine_VideoBuffer_setData((Machine_VideoBuffer *)Machine_Font_getFloatBuffer(self->font), sizeof(vertices), (void const*)vertices);

      static const uint8_t indices[] = {
        0, 1, 2,
        2, 1, 3,
      };
      static const size_t UNIT = 0;
      Machine_Video_bindTexture(0, symbolTexture);
      Machine_Binding_bindSampler(binding, Machine_String_create("texture_1", strlen("texture_1")), UNIT);
      Machine_UtilitiesGl_call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices));

      vec2_add(cursorPosition, cursorPosition, symbolAdvance);
    }
    cursorPosition[0] = position0[0];
    cursorPosition[1] += Machine_Font_getBaselineDistance(self->font);
  }

  glDisable(GL_CLIP_DISTANCE3);
  glDisable(GL_CLIP_DISTANCE2);
  glDisable(GL_CLIP_DISTANCE1);
  glDisable(GL_CLIP_DISTANCE0);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_Text_Layout_setText(Machine_Text_Layout* self, Machine_String* text) {
  if (!self || !text) {
    Machine_setStatus(Machine_Status_InvalidArgument);
    Machine_jump();
  }
  if (!Machine_String_isEqualTo(self->text, text)) {
    self->text = text;
    self->flags |= LINES_DIRTY | LINE_BOUNDS_DIRTY | BOUNDS_DIRTY;
  }
}

Machine_String* Machine_Text_Layout_getText(Machine_Text_Layout* self) {
  return self->text;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_Text_Layout_setPosition(Machine_Text_Layout* self, Machine_Math_Vector2* position) {
  if (position == NULL) {
    Machine_setStatus(Machine_Status_InvalidArgument);
    Machine_jump();
  }
  Machine_Math_Vector2_copy(self->position, position);
  self->flags |= LINE_BOUNDS_DIRTY | BOUNDS_DIRTY;
}

const Machine_Math_Vector2* Machine_Text_Layout_getPosition(Machine_Text_Layout* self) {
  return self->position;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_Text_Layout_setColor(Machine_Text_Layout* self, const Machine_Math_Vector3* color) {
  if (color == NULL) {
    Machine_setStatus(Machine_Status_InvalidArgument);
    Machine_jump();
  }
  Machine_Math_Vector3_copy(self->color, color);
}

const Machine_Math_Vector3* Machine_Text_Layout_getColor(Machine_Text_Layout* self) {
  return self->color;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_Text_Layout_setRenderVisualBoundsEnabled(Machine_Text_Layout* self, bool renderVisualBounds) {
  self->renderVisualBounds = renderVisualBounds;
}

bool Machine_Text_Layout_getRenderVisualBoundsEnabled(Machine_Text_Layout* self) {
  return self->renderVisualBounds;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Machine_Text_Layout_setClipRectangle(Machine_Text_Layout* self, Machine_Math_Rectangle2* clipRectangle) {
  if (clipRectangle) {
    if (!self->clipRectangle) {
      Machine_Math_Rectangle2 * temporary = Machine_Math_Rectangle2_create();
      Machine_Math_Rectangle2_copy(temporary, clipRectangle);
      self->clipRectangle = temporary;
    } else {
      Machine_Math_Rectangle2_copy(self->clipRectangle, clipRectangle);
    }
  } else {
    self->clipRectangle = NULL;
  }
}

Machine_Math_Rectangle2* Machine_Text_Layout_getClipRectangle(Machine_Text_Layout* self) {
  return self->clipRectangle;
}
