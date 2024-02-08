#include "dx/val/gl/wgl/wglfunctions.h"

#include <stdio.h>

static Core_Boolean g_initialized = false;
static _WglFunctions g_instance = { 0 };

static void _WglFunctions_keyAdded(Core_String** w) {
  CORE_REFERENCE(*w);
}

static void _WglFunctions_keyRemoved(Core_String** w) {
  CORE_UNREFERENCE(*w);
}

static Core_Result _WglFunctions_compareKeys(Core_Boolean* RETURN, Core_String** v, Core_String** w) {
  return Core_String_isEqualTo(RETURN, *v, *w);
}

static Core_Result _WglFunctions_hashKey(Core_Size* RETURN, Core_String** w) {
  return Core_String_getHashValue(RETURN, *w);
}

static Core_Result link(void** RETURN, char const* name) {
  void* p = wglGetProcAddress(name);
  if (!p) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = p;
  return Core_Success;
}

static Core_Result _WglFunctions_dumpWglExtensionsString() {
  Core_InlineHashMapPPIterator iterator;
  if (Core_InlineHashMapPPIterator_initialize(&iterator, &g_instance.extensions)) {
    return Core_Failure;
  }
  Core_Boolean hasEntry;
  if (Core_InlineHashMapPPIterator_hasEntry(&hasEntry, &iterator)) {
    Core_InlineHashMapPPIterator_uninitialize(&iterator);
    return Core_Failure;
  }
  while (hasEntry) {
    Core_String* key;
    if (Core_InlineHashMapPPIterator_getKey(&key, &iterator)) {
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    fwrite(" ", 1, strlen(" "), stdout);
    fwrite(key->bytes, 1, key->numberOfBytes, stdout);
    fwrite("\n", 1, strlen("\n"), stdout);
    if (Core_InlineHashMapPPIterator_next(&iterator)) {
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    if (Core_InlineHashMapPPIterator_hasEntry(&hasEntry, &iterator)) {
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
  }
  Core_InlineHashMapPPIterator_uninitialize(&iterator);
  return Core_Success;
}

static Core_Result _WglFunctions_addWglExtensionString(Core_String* w) {
  Core_String* v = NULL;
  if (Core_InlineHashMapPP_get(&v, &g_instance.extensions, w)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  }
  if (v) {
    fprintf(stderr, "%s:%d: extension %*.s already added\n", __FILE__, __LINE__, (int)w->numberOfBytes, w->bytes);
  } else {
    if (Core_InlineHashMapPP_set(&g_instance.extensions, w, w)) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static Core_Result _WglFunctions_updateWglExtensionsString() {
  if (Core_InlineHashMapPP_clear(&g_instance.extensions)) {
    return Core_Failure;
  }
  char const* start = g_instance._wglGetExtensionsStringARB(g_instance.dc);
  if (!start) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  char const* end = start + strlen(start);
  char const* current = start;
  while (current != end) {
    while (*current != ' ' && current != end) {
      current++;
    }
    Core_String* s;
    if (current != start) {
      if (Core_String_create(&s, start, current - start)) {
        return Core_Failure;
      }
      if (_WglFunctions_addWglExtensionString(s)) {
        CORE_UNREFERENCE(s);
        s = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(s);
      s = NULL;
    }
    start = current;
    if (current != end) {
      current++;
    }
    start = current;
  }
  if (current != start) {
    Core_String* s;
    if (Core_String_create(&s, start, current - start)) {
      return Core_Failure;
    }
    if (_WglFunctions_addWglExtensionString(s)) {
      CORE_UNREFERENCE(s);
      s = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(s);
    s = NULL;
  }
  return Core_Success;
}

static Core_Result _WglFunctions_isWglExtensionSupported(Core_Boolean* RETURN, const char* expected) {
  Core_String* receivedString;
  Core_String* expectedString;
  if (Core_String_create(&expectedString, expected, strlen(expected))) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPP_get(&receivedString, &g_instance.extensions, expectedString)) {
    if (Core_Error_NotFound == Core_getError()) {
      *RETURN = Core_False;
      CORE_UNREFERENCE(expectedString);
      expectedString = NULL;
      Core_setError(Core_Error_NoError);
      return Core_Success;
    } else {
      CORE_UNREFERENCE(expectedString);
      expectedString = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(expectedString);
  expectedString = NULL;
  *RETURN = Core_True;
  return Core_Success;
}

Core_Result _WglFunctions_startup(HDC dc) {
  if (g_initialized) {
    Core_setError(Core_Error_Initialized);
    return Core_Failure;
  }
  g_instance.dc = dc;
  Core_InlineHashMapPP_Configuration configuration = {
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&_WglFunctions_keyAdded,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&_WglFunctions_keyRemoved,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&_WglFunctions_compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&_WglFunctions_hashKey,
  };
  if (Core_InlineHashMapPP_initialize(&g_instance.extensions, &configuration)) {
    g_instance.dc = NULL;
    return Core_Failure;
  }
#define Link(V, T, S) \
  if (link((void**)&g_instance.V, S)) { \
    fprintf(stderr, "%s:%d: unable to acquire a pointer to the %s function\n", __FILE__, __LINE__, S); \
    Core_InlineHashMapPP_uninitialize(&g_instance.extensions); \
    g_instance.dc = NULL; \
    return Core_Failure; \
  }

  // Link the fundamental WGL extension.
  Link(_wglGetExtensionsStringARB, PFNWGLGETEXTENSIONSSTRINGARBPROC, "wglGetExtensionsStringARB");
  // (2) Update WGL extension string.
  if (_WglFunctions_updateWglExtensionsString()) {
    Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
    g_instance.dc = NULL;
    return Core_Failure;
  }
#if 0
  _WglFunctions_dumpWglExtensionsString();
#endif

  // Link "WGL_ARB_pixel_format"/"WGL_EXT_pixel_format" WGL extensions.
  {
    Core_Boolean isSupported;
    if (_WglFunctions_isWglExtensionSupported(&isSupported, "WGL_ARB_pixel_format")) {
      Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
      g_instance.dc = NULL;
      return Core_Failure;
    }
    if (!isSupported) {
      fprintf(stderr, "%s:%d: extension %s not supported\n", __FILE__, __LINE__, "WGL_ARB_pixel_format");
      return Core_Failure;
    }
    Link(_wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC, "wglChoosePixelFormatARB");
    Link(_wglGetPixelFormatAttribivARB, PFNWGLGETPIXELFORMATATTRIBIVARBPROC, "wglGetPixelFormatAttribivARB");
    Link(_wglGetPixelFormatAttribfvARB, PFNWGLGETPIXELFORMATATTRIBFVARBPROC, "wglGetPixelFormatAttribfvARB");
  }

  // Link "WGL_ARB_create_context" WGL extension.
  {
    Core_Boolean isSupported;
    if (_WglFunctions_isWglExtensionSupported(&isSupported, "WGL_ARB_create_context")) {
      Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
      g_instance.dc = NULL;
      return Core_Failure;
    }
    if (!isSupported) {
      fprintf(stderr, "%s:%d: extension %s not supported\n", __FILE__, __LINE__, "WGL_ARB_create_context");
      return Core_Failure;
    }
    Link(_wglCreateContextAttribsARB, PFNWGLCREATECONTEXTATTRIBSARBPROC, "wglCreateContextAttribsARB");
  }
  g_initialized = Core_True;
  return Core_Success;
}

Core_Result _WglFunctions_shutdown() {
  if (!g_initialized) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
  g_instance.dc = NULL;
  g_initialized = Core_False;
  return Core_Success;
}

Core_Result _WglFunctions_getInstance(_WglFunctions** RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!g_initialized) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  *RETURN = &g_instance;
  return Core_Success;
}
