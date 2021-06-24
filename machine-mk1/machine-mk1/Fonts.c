/**
 * @author Michael Heilmann (<michaelheilmann@primordialmachine.com>)
 * @copyright Copyright (c) 2021 Michael Heilmann. All rights reservied.
 */
#include "Fonts.h"



#include "Video.h"
#include "Machine.h"
#include "_Video.h"
#include "GL/Buffer.h"
#include "GL/Texture.h"
#include "ShaderProgram.h"

#include <linmath.h>
#include "VertexDescriptor.h"
#include "Binding.h"
#include <inttypes.h>

#include <ft2build.h>
#include FT_FREETYPE_H



static int referenceCount = 0;
static FT_Library library;


typedef struct Node Node;

struct Node {
  Node* next;
  /// @brief Unicode code point.
  uint32_t codepoint;
  /// @brief The texture.
  Machine_Texture* texture;
  /// @brief Add bearingx to the cursor position before rendering.
  float bearingx;
  /// @brief Add h - bearingy to the cursor position before rendering.
  float bearingy;
  /// @brief left, top, width, height in pixels.
  /// @remark Left and top are usually 0.
  float w, h;
  /// @brief Advance in pixels.
  float advancex;
  /// @brief Advance in pixels.
  float advancey;
};

typedef struct Map Map;

struct Map {
  Node** buckets;
  size_t size, capacity;
};

static void Map_visit(Map* self) {
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    Node* node = self->buckets[i];
    while (node) {
      Machine_visit(node->texture);
      node = node->next;
    }
  }
}

static void Map_finalize(Map* self) {
  for (size_t i = 0, n = self->capacity; i < n; ++i) {
    while (self->buckets[i]) {
      Node* node = self->buckets[i];
      self->buckets[i] = node->next;
      free(node);
      self->size--;
    }
  }
  if (self->buckets) {
    free(self->buckets);
    self->buckets = NULL;
  }
}

static Map* Map_create() {
  Map* self = Machine_allocate(sizeof(Map), (Machine_VisitCallback*)&Map_visit, (Machine_FinalizeCallback*)&Map_finalize);
  if (!self) {
    Machine_setStatus(Machine_Status_AllocationFailed);
    Machine_jump();
  }
  self->size = 0;
  self->capacity = 8;
  self->buckets = malloc(sizeof(Node*) * 8);
  if (!self->buckets) {
    self->capacity = 0;
    Machine_setStatus(Machine_Status_AllocationFailed);
    Machine_jump();
  }
  for (size_t i = 0; i < 8; ++i) {
    self->buckets[i] = NULL;
  }
  return self;
}

static Node* Map_get(Map* self, uint32_t codepoint) {
  size_t hashIndex = codepoint % self->capacity;
  for (Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->codepoint == codepoint) {
      return node;
    }
  }
  return NULL;
}

static void Map_set(Map* self, uint32_t codepoint, float bearingx, float bearingy, float w, float h, float advancex, float advancey, Machine_Texture* texture) {
  size_t hashIndex = codepoint % self->capacity;
  for (Node* node = self->buckets[hashIndex]; NULL != node; node = node->next) {
    if (node->codepoint == codepoint) {
      Machine_setStatus(Machine_Status_InvalidOperation);
      Machine_jump();
    }
  }

  Node* node = malloc(sizeof(Node));
  if (!node) {
    Machine_setStatus(Machine_Status_InvalidOperation);
    Machine_jump();
  }

  node->codepoint = codepoint;
  node->texture = texture;
  node->w = w;
  node->h = h;
  node->bearingx = bearingx;
  node->bearingy = bearingy;
  node->advancex = advancex;
  node->advancey = advancey;

  node->next = self->buckets[hashIndex];
  self->buckets[hashIndex] = node;
  self->size++;
}

struct Machine_Fonts_Font {
  int referenceCount;
  FT_Face face;
  Map* map;
  float baselineDistance;

  Machine_ShaderProgram* shader;
  Machine_FloatBuffer* vertices;
  Machine_Binding* binding;
};


int Machine_Fonts_startup() {
  if (referenceCount == INT_MAX) {
    return 1;
  }
  else if (referenceCount == 0) {
    FT_Error error;
    error = FT_Init_FreeType(&library);
    if (error) {
      Machine_log(Machine_LogFlags_ToErrors, __FILE__, __LINE__, "FT_Init_FreeType failed\n");
      return 1;
    }
    referenceCount++;
    return 0;
  }
  else {
    referenceCount++;
    return 0;
  }
}

void Machine_Fonts_shutdown() {
  if (0 == --referenceCount) {
    FT_Done_FreeType(library);
  }
}

void Machine_Fonts_Font_visit(Machine_Fonts_Font* self) {
  if (self->map) {
    Machine_visit(self->map);
  }
  if (self->vertices) {
    Machine_visit(self->vertices);
  }
  if (self->shader) {
    Machine_visit(self->shader);
  }
  if (self->binding) {
    Machine_visit(self->binding);
  }
}

void Machine_Fonts_Font_finalize(Machine_Fonts_Font* self) {
  if (self->face) {
    FT_Done_Face(self->face);
    self->face = NULL;
  }
  Machine_Fonts_shutdown();
}

Machine_Fonts_Font* Machine_Fonts_createFont(const char* path, int pointSize) {
  if (Machine_Fonts_startup()) {
    return NULL;
  }
  Machine_Fonts_Font* font = Machine_allocate(sizeof(Machine_Fonts_Font), (Machine_VisitCallback*)&Machine_Fonts_Font_visit, (Machine_FinalizeCallback*)&Machine_Fonts_Font_finalize);
  if (!font) {
    Machine_Fonts_shutdown();
    return NULL;
  }
  FT_Error error;
  // (1)
  error = FT_New_Face(library, path, 0, &font->face);
  if (error) {
    return NULL;
  }
  // (2)
  error = FT_Set_Pixel_Sizes(font->face, 0, pointSize);
  if (error) {
    FT_Done_Face(font->face);
    font->face = NULL;
    return NULL;
  }

  font->baselineDistance = font->face->size->metrics.height >> 6;

  Machine_JumpTarget jumpTarget;
  Machine_pushJumpTarget(&jumpTarget);
  if (!setjmp(jumpTarget.environment)) {
    font->map = Map_create();
    font->vertices = (Machine_FloatBuffer*)Machine_GL_FloatBuffer_create();
    font->shader = Machine_ShaderProgram_generateTextShader(true);

    Machine_VertexDescriptor* vertexDescriptor = Machine_VertexDescriptor_create();
    Machine_VertexDescriptor_append(vertexDescriptor, Machine_VertexElementSemantics_XfYf);
    Machine_VertexDescriptor_append(vertexDescriptor, Machine_VertexElementSemantics_UfVf);

    font->binding = Machine_Binding_create(font->shader, vertexDescriptor, (Machine_VideoBuffer*)font->vertices);
    Machine_Binding_set(font->binding, Machine_String_create("vertex_position", strlen("vertex_position") + 1), 0);
    Machine_Binding_set(font->binding, Machine_String_create("vertex_texture_coordinate_1", strlen("vertex_texture_coordinate_1") + 1), 1);

    Machine_popJumpTarget();
  }
  else {
    Machine_popJumpTarget();
    FT_Done_Face(font->face);
    font->face = NULL;
    return NULL;
  }

  const uint32_t codepoints[] = {
    'a', 'A',
    'b', 'B',
    'c', 'C',
    'd', 'D',
    'e', 'E',
    'f', 'F',
    'g', 'G',
    'h', 'H',
    'i', 'I',
    'j', 'J',
    'k', 'K',
    'l', 'L',
    'm', 'M',
    'n', 'N',
    'o', 'O',
    'p', 'P',
    'q', 'Q',
    'r', 'R',
    's', 'S',
    't', 'T',
    'u', 'U',
    'v', 'V',
    'w', 'W',
    'x', 'X',
    'y', 'Y',
    'z', 'Z',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '.',
    ',',
    '!',
    ' ',
    '+',
    '-',
  };
  Machine_ByteBuffer* temporary = Machine_ByteBuffer_create();
  for (size_t i = 0, n = sizeof(codepoints) / sizeof(uint32_t); i < n; ++i) {
    const uint32_t codepoint = codepoints[i];
    if (FT_Load_Char(font->face, codepoint, FT_LOAD_RENDER)) {
      Machine_setStatus(Machine_Status_InvalidOperation);
      Machine_jump();
    }
    Machine_ByteBuffer_clear(temporary);
    Machine_ByteBuffer_appendBytes(temporary, font->face->glyph->bitmap.buffer, (size_t)(font->face->glyph->bitmap.width * font->face->glyph->bitmap.rows));
    Machine_Images_Image* image = Machine_Images_createImageDirect(Machine_PixelFormat_GRAYSCALE, font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows, temporary);
    Machine_Texture* texture = (Machine_Texture *)Machine_GL_Texture_create(image);
    Map_set(font->map, codepoint,
            font->face->glyph->bitmap_left, font->face->glyph->bitmap_top,
            font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows,
            font->face->glyph->advance.x >> 6, font->face->glyph->advance.y >> 6, texture);
  }
  return font;
}

static bool isWhitespace(uint32_t codepoint) {
  return codepoint == ' '
      || codepoint == '\t';
}

static bool isNewline(uint32_t codepoint) {
  return codepoint == '\n';
}

float Machine_Font_getBaselineDistance(Machine_Fonts_Font* self) {
  return self->baselineDistance;
}

bool Machine_Font_getCodePointInfo(Machine_Fonts_Font* self, uint32_t codepoint, Machine_Math_Rectangle2* bounds, vec2 advance, Machine_Texture** texture) {
  if (isWhitespace(codepoint)) {
    codepoint = ' ';
  } else if (isNewline(codepoint)) {
    return false;
  }
  Node* node = Map_get(self->map, codepoint);
  if (node == NULL) {
    Machine_log(Machine_LogFlags_ToWarnings, __FILE__, __LINE__, "%"PRIu32" not found\n", codepoint);
    return false;
  }
  Machine_Math_Vector2* v = Machine_Math_Vector2_create();
  Machine_Math_Vector2_set(v, node->bearingx, node->bearingy);
  Machine_Math_Rectangle2_setPosition(bounds, v);
  Machine_Math_Vector2_set(v, node->w, node->h);
  Machine_Math_Rectangle2_setSize(bounds, v);
  *texture = node->texture;
  advance[0] = node->advancex;
  advance[1] = node->advancey;
  return true;
}

Machine_Binding* Machine_Font_getBinding(Machine_Fonts_Font* self) {
  return self->binding;
}

Machine_ShaderProgram* Machine_Font_getShaderProgram(Machine_Fonts_Font* self) {
  return self->shader;
}

Machine_FloatBuffer* Machine_Font_getFloatBuffer(Machine_Fonts_Font* self) {
  return self->vertices;
}
