#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "dx/core.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>

DX_DECLARE_OBJECT_TYPE("Pixie_Scanner", Pixie_Scanner, Core_Object);

static inline Pixie_Scanner* PIXIE_SCANNER(void* p) {
  return (Pixie_Scanner*)p;
}

struct Pixie_Scanner {
  Core_Object _parent;
};

static inline Pixie_Scanner_dispatch* PIXIE_SCANNER_DISPATCH(void* p) {
  return (Pixie_Scanner_dispatch*)p;
}

struct Pixie_Scanner_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Pixie_Scanner_construct(Pixie_Scanner* SELF);

Core_Result Pixie_Scanner_create(Pixie_Scanner** RETURN);

DX_DEFINE_OBJECT_TYPE("PixieScanner", Pixie_Scanner, Core_Object);

static void Pixie_Scanner_destruct(Pixie_Scanner* SELF)
{/*Intentionally empty.*/}

static void Pixie_Scanner_constructDispatch(Pixie_Scanner_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Pixie_Scanner_construct(Pixie_Scanner* SELF) {
  Core_BeginConstructor(Pixie_Scanner);
  Core_EndConstructor(Pixie_Scanner);
#if 0
  DX_OBJECT(SELF)->type = TYPE;
  return Core_Success;
#endif
}

Core_Result Pixie_Scanner_create(Pixie_Scanner** RETURN) {
  DX_CREATE_PREFIX(Pixie_Scanner);
  if (Pixie_Scanner_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int draw_something_2(struct Canvas* canvas);

static int draw_something_1(struct Canvas* canvas);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct JumpTarget {
  struct JumpTarget* previous;
  jmp_buf env;
};

typedef int64_t Integer64;
typedef Integer64 Integer;

/// A pointer to a struct Canvas object.
#define Value_Tag_Canvas (0)

/// A 64-bit two's complement integer.
#define Value_Tag_Integer (1)
/// Alias for Value_Tag_Integer64
#define Value_Tag_Integer64 (1)

/// A pointer to a C function of the signature void(struct State*).
#define Value_Tag_NativeFunction (2)

/// Four consecutive Bytes.
/// The 2nd, 3rd, and 4th Byte contain the values of the red, green, and blue component, respectively.
/// The 1st Byte contains the value of the (ignored) alpha component.
#define Value_Tag_PackedColorA8R8G8B8 (3)

struct Value {
  uint8_t tag;
  union {
    struct Canvas* _canvas;
    union {
      Integer _integer64;
      Integer _integer;
    };
    void (*_nativeFunction)(struct State*);
    uint32_t _packedColor;
  };
};

static void Value_setCanvas(struct Value* value, struct Canvas* _canvas) {
  value->tag = Value_Tag_Canvas;
  value->_canvas = _canvas;
}

static void Value_setInteger(struct Value* value, Integer _integer) {
  value->tag = Value_Tag_Integer;
  value->_integer = _integer;
}

static void Value_setInteger64(struct Value* value, Integer64 _integer64) {
  value->tag = Value_Tag_Integer64;
  value->_integer64 = _integer64;
}

static void Value_setNativeFunction(struct Value* value, void (*_nativeFunction)(struct State*)) {
  value->tag = Value_Tag_NativeFunction;
  value->_nativeFunction = _nativeFunction;
}

static void Value_setPackedColorA8R8G8B8(struct Value* value, uint32_t _packedColor) {
  value->tag = Value_Tag_PackedColorA8R8G8B8;
  value->_packedColor = _packedColor;
}

struct State {
  int error;
  struct JumpTarget* jumpTarget;
  struct {
    struct Value* values;
    size_t size, capacity;
  } stack;
};

static int State_initialize(struct State* self) {
  self->stack.size = 0;
  self->stack.capacity = 8;
  self->stack.values = malloc(sizeof(struct Value) * 8);
  if (!self->stack.values) {
    fprintf(stderr, "%s:%d: failed to create stack\n", __FILE__, __LINE__);
    return 1;
  }
  self->error = 0;
  self->jumpTarget = NULL;
  return 0;
}

static void State_uninitialize(struct State* self) {
  free(self->stack.values);
  self->stack.values = NULL;
}

static void State_pushJumpTarget(struct State* self, struct JumpTarget* jumpTarget) {
  jumpTarget->previous = self->jumpTarget;
  self->jumpTarget = jumpTarget;
}

static void State_popJumpTarget(struct State* self) {
  self->jumpTarget = self->jumpTarget->previous;
}

static void State_growStack(struct State* self) {
  static const uint32_t MAX_CAPACITY = UINT32_MAX / sizeof(struct Value);
  uint8_t oldCapacity = self->stack.capacity;
  uint8_t addCapacity = oldCapacity / 2;
  if (MAX_CAPACITY - oldCapacity < addCapacity) {
    self->error = 1;
    longjmp(self->jumpTarget->env, -1);
  }
  uint8_t newCapacity = oldCapacity + addCapacity;
  struct Value* newValues = realloc(self->stack.values, sizeof(struct Value) * newCapacity);
  if (!newValues) {
    self->error = 1;
    longjmp(self->jumpTarget->env, -1);
  }
  self->stack.capacity = newCapacity;
  self->stack.values = newValues;
}

static void State_pushCanvas(struct State* self, struct Canvas* _canvas) {
  if (self->stack.size == self->stack.capacity) {
    State_growStack(self);
  }
  struct Value* value = &(self->stack.values[self->stack.size]);
  Value_setCanvas(value, _canvas);
  self->stack.size++;
}

static void State_pushInteger(struct State* self, Integer _integer) {
  if (self->stack.size == self->stack.capacity) {
    State_growStack(self);
  }
  struct Value* value = &(self->stack.values[self->stack.size]);
  Value_setInteger(value, _integer);
  self->stack.size++;
}

static void State_pushInteger64(struct State* self, Integer64 _integer64) {
  if (self->stack.size == self->stack.capacity) {
    State_growStack(self);
  }
  struct Value* value = &(self->stack.values[self->stack.size]);
  Value_setInteger64(value, _integer64);
  self->stack.size++;
}

static void State_pushNativeFunction(struct State* self, void (*_nativeFunction)(struct State*)) {
  if (self->stack.size == self->stack.capacity) {
    State_growStack(self);
  }
  struct Value* value = &(self->stack.values[self->stack.size]);
  Value_setNativeFunction(value, _nativeFunction);
  self->stack.size++;
}


static void State_pushPackedColorA8R8G8B8(struct State* self, uint32_t _packedColor) {
  if (self->stack.size == self->stack.capacity) {
    State_growStack(self);
  }
  struct Value* value = &(self->stack.values[self->stack.size]);
  Value_setPackedColorA8R8G8B8(value, _packedColor);
  self->stack.size++;
}

static struct State* g_state = NULL;

static struct Canvas* g_canvas = NULL;

static const int g_timerId = 101;

static HWND g_hWnd = NULL;

static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct GraphicsBuffer {
  HBITMAP hBitmap;
  uint32_t* pixels;
  int width;
  int height;
};

static int GraphicsBuffer_initialize(struct GraphicsBuffer* self, int width, int height);

static void GraphicsBuffer_uninitialize(struct GraphicsBuffer* self);

static int GraphicsBuffer_swap(struct GraphicsBuffer* self, struct GraphicsBuffer* other);

static int GraphicsBuffer_clear(struct GraphicsBuffer* self);

static int GraphicsBuffer_setPixel(struct GraphicsBuffer* self, uint32_t x, uint32_t y, uint32_t pixelValue);

static int GraphicsBuffer_initialize(struct GraphicsBuffer* self, int width, int height) {
  HDC hdcScreen = GetDC(NULL);

  self->width = width;
  self->height = height;

  BITMAPINFO bmi = { 0 };
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height; // top-down
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  self->hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)(&self->pixels), NULL, 0);
  if (!self->hBitmap) {
    ReleaseDC(NULL, hdcScreen);
    return 1;
  }

  ReleaseDC(NULL, hdcScreen);

  return 0;
}

static void GraphicsBuffer_uninitialize(struct GraphicsBuffer* self) {
  DeleteObject(self->hBitmap);
}

// UNDEFINED BEHAVIOR if SELF and OTHER are not compatible
static int GraphicsBuffer_swap(struct GraphicsBuffer* self, struct GraphicsBuffer* other) {
  if (self == other) {
    return 0;
  }
  void* pixels;
  pixels = self->pixels;
  self->pixels = other->pixels;
  other->pixels = pixels;

  HBITMAP hBitmap;
  hBitmap = self->hBitmap;
  self->hBitmap = other->hBitmap;
  other->hBitmap = hBitmap;

  return 0;
}

static int GraphicsBuffer_clear(struct GraphicsBuffer* self) {
  uint64_t size = (uint64_t)self->width * (uint64_t)self->height;
  memset(self->pixels, 0, size);
  return 0;
}

static int GraphicsBuffer_setPixel(struct GraphicsBuffer* self, uint32_t x, uint32_t y, uint32_t pixelValue) {
  self->pixels[y * self->width + x] = pixelValue;
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct Canvas {
  struct GraphicsBuffer front;
  struct GraphicsBuffer back;
  int width;
  int height;
};

static int Canvas_initialize(struct Canvas* self, int width, int height) {
  self->width = width;
  self->height = height;
  if (GraphicsBuffer_initialize(&self->front, width, height)) {
    return 1;
  }
  if (GraphicsBuffer_initialize(&self->back, width, height)) {
    GraphicsBuffer_uninitialize(&self->front);
    return 1;
  }
  return 0;
}

static void Canvas_uninitialize(struct Canvas* self) {
  GraphicsBuffer_uninitialize(&self->back);
  GraphicsBuffer_uninitialize(&self->front);
}

static int Canvas_clear(struct Canvas* self) {
  uint64_t size = (uint64_t)self->width * (uint64_t)self->height;
  memset(self->back.pixels, 0, size);
  return 0;
}

static int Canvas_setPixel(struct Canvas* self, uint32_t x, uint32_t y, uint32_t pixelValue) {
  self->back.pixels[y * self->width + x] = pixelValue;
  return 0;
}

static int Canvas_swap(struct Canvas* self) {
  return GraphicsBuffer_swap(&self->front, &self->back);
}

static int Canvas_getSize(struct Canvas* self, int* width, int* height) {
  *width = self->width;
  *height = self->height;
  return 0;
}

static int Canvas_resize(struct Canvas* self, int width, int height) {
  if (self->width != width || self->height != height) {
    struct GraphicsBuffer front;
    struct GraphicsBuffer back;
    if (GraphicsBuffer_initialize(&front, width, height)) {
      return 1;
    }
    if (GraphicsBuffer_initialize(&back, width, height)) {
      GraphicsBuffer_uninitialize(&front);
      return 1;
    }
    GraphicsBuffer_swap(&self->front, &front);
    GraphicsBuffer_swap(&self->back, &back);
    GraphicsBuffer_uninitialize(&back);
    GraphicsBuffer_uninitialize(&front);
    self->width = width;
    self->height = height;
  }
  return 0;
}

static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_SIZE: {
      UINT width = LOWORD(lParam);
      UINT height = HIWORD(lParam);
      if (Canvas_resize(g_canvas, width, height)) {
        g_state->error = 1;
      }
      return 0;
    } break;
    case WM_CREATE: {
      RECT r;
      GetClientRect(hWnd, &r);
      g_canvas = malloc(sizeof(struct Canvas));
      if (!g_canvas) {
        return -1;
      }
      if (Canvas_initialize(g_canvas, r.right - r.left, r.bottom - r.top)) {
        free(g_canvas);
        g_canvas = NULL;
        return -1;
      }
      SetTimer(hWnd, g_timerId, 1, NULL);
      return 0;
    } break;
    case WM_TIMER: {
      draw_something_2(g_canvas);
      draw_something_1(g_canvas);
      Canvas_swap(g_canvas);
      Canvas_clear(g_canvas);
      InvalidateRect(hWnd, NULL, FALSE);
      return 0;
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      HDC hdc_bmp = CreateCompatibleDC(hdc);
      HBITMAP old_bmp = SelectObject(hdc_bmp, g_canvas->front.hBitmap);
      BitBlt(hdc, 0, 0, g_canvas->width, g_canvas->height, hdc_bmp, 0, 0, SRCCOPY);
      SelectObject(hdc, old_bmp);
      DeleteDC(hdc_bmp);
      EndPaint(hWnd, &ps);
      return 0;
    } break;
    case WM_DESTROY: {
      Canvas_uninitialize(g_canvas);
      free(g_canvas);
      g_canvas = NULL;
      PostQuitMessage(0);
      return 0;
    } break;
    case WM_CLOSE: {
      DestroyWindow(hWnd);
      return 0;
    } break;
    default: {
      return DefWindowProc(hWnd, message, wParam, lParam);
    } break;
  };
}

static int packA8R8G8B8(uint8_t r, uint8_t g, uint8_t b, uint32_t *p) {
  uint8_t a = 255;
  *p = ((uint32_t)a << 24)
     | ((uint32_t)r << 16)
     | ((uint32_t)g <<  8)
     | ((uint32_t)b <<  0)
    ;
  return 0;
}

// Push the canvas on sthe stack
// [
//  ...,
//  numberOfArguments:Integer = 0
// ]
// =>
// [
//  ...,
//  canvas:Canvas
// ]
static void Cmd_getCanvas(struct State* state) {
  if (state->stack.size < 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  // numberOfArguments:Integer = 0
  if (state->stack.values[state->stack.size - 1].tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int numberOfArguments = state->stack.values[state->stack.size - 1]._integer;
  if (numberOfArguments != 0) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
}

// Push the canvas width on sthe stack
// [
//  ...,
//  canvas:Canvas,
//  numberOfArguments:Integer = 1
// ]
// =>
// [
//  ...,
//  canvasWidth:Integer
// ]
static void Cmd_getCanvasWidth(struct State* state) {
  if (state->stack.size < 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  // numberOfArguments:Integer = 1
  if (state->stack.values[state->stack.size - 1].tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int numberOfArguments = state->stack.values[state->stack.size - 1]._integer;
  if (numberOfArguments != 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }

  struct Value* arg = NULL;

  // canvas:Canvas
  arg = &(state->stack.values[state->stack.size - 1 - 6]);
  if (arg->tag != Value_Tag_Canvas) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  struct Canvas* canvas = NULL;
  canvas = arg->_canvas;
}

// Push the canvas height on sthe stack
// [
//  ...,
//  canvas:Canvas,
//  numberOfArguments:Integer = 1
// ]
// =>
// [
//  ...,
//  canvasWHeight:Integer
// ]
static void Cmd_getCanvasHeight(struct State* state) {
  if (state->stack.size < 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  // numberOfArguments:Integer = 1
  if (state->stack.values[state->stack.size - 1].tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int numberOfArguments = state->stack.values[state->stack.size - 1]._integer;
  if (numberOfArguments != 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }

  struct Value* arg = NULL;

  // canvas:Canvas
  arg = &(state->stack.values[state->stack.size - 1 - 6]);
  if (arg->tag != Value_Tag_Canvas) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  struct Canvas* canvas = NULL;
  canvas = arg->_canvas;
}

// Fill a rectangle
// [
//  ...,
//  canvas:Canvas,
//  x:Integer,
//  y:Integer,
//  w:Integer >= 0
//  h:Integer >= 0,
//  fillColor:PackedColor,
//  numberOfArguments:Integer = 6
// ]
// =>
// [
//  ...
// ]
static void Cmd_fillRectangle(struct State* state) {
  if (state->stack.size < 1) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  // numberOfArguments:Integer = 6
  if (state->stack.values[state->stack.size - 1].tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int numberOfArguments = state->stack.values[state->stack.size - 1]._integer;
  if (numberOfArguments != 6) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }

  struct Value* arg = NULL;

  // canvas:Canvas
  arg = &(state->stack.values[state->stack.size - 1 - 6]);
  if (arg->tag != Value_Tag_Canvas) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  struct Canvas* canvas = NULL;
  canvas = arg->_canvas;
  // x:Integer
  arg = &(state->stack.values[state->stack.size - 1 - 5]);
  if (arg->tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int x = arg->_integer;
  // y:Integer
  arg = &(state->stack.values[state->stack.size - 1 - 4]);
  if (arg->tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int y = arg->_integer;
  // w:Integer
  arg = &(state->stack.values[state->stack.size - 1 - 3]);
  if (arg->tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int w = arg->_integer;
  // h:Integer
  arg = &(state->stack.values[state->stack.size - 1 - 2]);
  if (arg->tag != Value_Tag_Integer) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  int h = arg->_integer;
  // fillColor:PackedColor
  arg = &(state->stack.values[state->stack.size - 1 - 1]);
  if (arg->tag != Value_Tag_PackedColorA8R8G8B8) {
    state->error = 1;
    longjmp(state->jumpTarget->env, -1);
  }
  uint32_t fillColor = arg->_packedColor;

  for (int xx = x; xx < x + w; ++xx) {
    for (int yy = y; yy < y + h; ++yy) {
      if (xx >= 0 && xx < canvas->width && yy >= 0 && yy < canvas->height) {
        Canvas_setPixel(canvas, xx, yy, fillColor);
      }
    }
  }
}

static int draw_something_2(struct Canvas* canvas) {
  int w, h;
  Canvas_getSize(canvas, &w, &h);

  struct recti_t {
    int x, y, w, h;
  };

  // the width of the rectangle to draw
  struct recti_t r;
  r.w = w / 2;
  r.h = h / 2;
  r.x = 0;
  r.y = 0;
  r.x += w / 2 - r.w / 2;
  r.y += h / 2 - r.h / 2;

  struct JumpTarget jumpTarget;
  jumpTarget.previous = g_state->jumpTarget;
  g_state->jumpTarget = &jumpTarget;
  uint32_t oldStackSize = g_state->stack.size;
  if (!setjmp(jumpTarget.env)) {
    State_pushCanvas(g_state, canvas);
    State_pushInteger(g_state, r.x);
    State_pushInteger(g_state, r.y);
    State_pushInteger(g_state, r.w);
    State_pushInteger(g_state, r.h);
    uint32_t p;
    packA8R8G8B8(0, 0, 255, &p);
    State_pushPackedColorA8R8G8B8(g_state, p);
    State_pushInteger(g_state, 6);

    Cmd_fillRectangle(g_state);
    g_state->jumpTarget = g_state->jumpTarget->previous;
    g_state->stack.size = oldStackSize;
  } else {
    g_state->jumpTarget = g_state->jumpTarget->previous;
    g_state->stack.size = oldStackSize;
    return 1;
  }

return 0;
}

static int draw_something_1(struct Canvas* canvas) {
  int w, h;
  Canvas_getSize(canvas, &w, &h);

  static int x = 0;
  static int y = 0;
  static int x_vel = 4;
  static int y_vel = 7;

  uint32_t p;
  packA8R8G8B8(255, 255, 255, &p);

  if (x >= 0 && x < w && y >= 0 && y < h) {
    Canvas_setPixel(canvas, x, y, p);
  }

  x += x_vel;
  y += y_vel;
  if (x < 0 || x > w) {
    x_vel *= -1;
  }
  if (y < 0 || y > h) {
    y_vel *= -1;
  }

  return 0;
}

static int startup() {
  HINSTANCE hInstance = GetModuleHandle(NULL);
  WNDCLASS wc = { 0 };
  wc.lpfnWndProc = &windowProcedure;
  wc.hInstance = hInstance;
  wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
  wc.lpszClassName = TEXT("window-windows-winapi");
  if (!RegisterClass(&wc)) {
    return 1;
  }
  g_hWnd = CreateWindow(wc.lpszClassName,
    TEXT("Windows WinAPI"),
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 640, 480, 0, 0, hInstance, NULL);
  if (!g_hWnd) {
    UnregisterClassA(wc.lpszClassName, hInstance);
    return 1;
  }

  ShowWindow(g_hWnd, SW_SHOW);

  return 0;
}

static void shutdown() {
  // should be done in WM_NCDESTROY.
  g_hWnd = NULL;
}

int main(int argc, char** argv) {
  if (Core_initialize()) {
    return EXIT_FAILURE;
  }
  g_state = malloc(sizeof(struct State));
  if (!g_state) {
    Core_uninitialize();
    return EXIT_FAILURE;
  }
  if (State_initialize(g_state)) {
    free(g_state);
    g_state = NULL;
    Core_uninitialize();
    return EXIT_FAILURE;
  }
  if (startup()) {
    State_uninitialize(g_state);
    free(g_state);
    g_state = NULL;
    Core_uninitialize();
    return 1;
  }
  MSG msg = { 0 };
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  shutdown();
  State_uninitialize(g_state);
  free(g_state);
  g_state = NULL;
  Core_uninitialize();
  return EXIT_SUCCESS;
}
