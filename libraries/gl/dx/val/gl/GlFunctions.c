#include "dx/val/gl/GlFunctions.h"

#include <stdio.h>

static Core_Boolean g_initialized = false;
static _GlFunctions g_instance = { 0 };

static void _GlFunctions_keyAdded(Core_String** w) {
  CORE_REFERENCE(*w);
}

static void _GlFunctions_keyRemoved(Core_String** w) {
  CORE_UNREFERENCE(*w);
}

static Core_Result _GlFunctions_compareKeys(Core_Boolean* RETURN, Core_String** v, Core_String** w) {
  return Core_String_isEqualTo(RETURN, *v, *w);
}

static Core_Result _GlFunctions_hashKey(Core_Size* RETURN, Core_String** w) {
  return Core_String_getHashValue(RETURN, *w);
}

static Core_Result link(void** RETURN, char const* name) {
  void* p = (void*)wglGetProcAddress(name);
  if (!p) {
    // This technique is required for OpenGL 1.0 core functions.
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    p = (void*)GetProcAddress(module, name);
  }
  if (!p) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = p;
  return Core_Success;
}

static Core_Result _GlFunctions_addGlExtensionString(Core_String* w) {
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

static Core_Result _GlFunctions_dumpGlExtensionsString() {
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

static Core_Result _GlFunctions_updateGlExtensionsString() {
  if (Core_InlineHashMapPP_clear(&g_instance.extensions)) {
    return Core_Failure;
  }
  GLint n;
  g_instance._glGetIntegerv(GL_NUM_EXTENSIONS, &n);
  for (GLint i = 0; i < n; ++i) {
    GLubyte const* p = g_instance._glGetStringi(GL_EXTENSIONS, i);
    if (!p) {
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    Core_String *w;
    if (Core_String_create(&w, p, strlen(p))) {
      return Core_Failure;
    }
    if (_GlFunctions_addGlExtensionString(w)) {
      CORE_UNREFERENCE(w);
      w = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(w);
    w = NULL;
  }
  return Core_Success;
}


Core_Result _GlFunctions_startup() {
  if (g_initialized) {
    Core_setError(Core_Error_Initialized);
    return Core_Failure;
  }
  Core_InlineHashMapPP_Configuration configuration = {
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&_GlFunctions_keyAdded,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&_GlFunctions_keyRemoved,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&_GlFunctions_compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&_GlFunctions_hashKey,
  };
  if (Core_InlineHashMapPP_initialize(&g_instance.extensions, &configuration)) {
    return Core_Failure;
  }
#define Link(V, T, S) \
  if (link((void**)&g_instance.V, S)) { \
    fprintf(stderr, "%s:%d: unable to acquire a pointer to the %s function\n", __FILE__, __LINE__, S); \
    Core_InlineHashMapPP_uninitialize(&g_instance.extensions); \
    return Core_Failure; \
  }

  // Link the fundamental GL functions.
  Link(_glGetIntegerv, PFNGLGETINTEGERVPROC, "glGetIntegerv");
  Link(_glGetStringi, PFNGLGETSTRINIGPROC, "glGetStringi");
  // (2) Update WGL extension string.
  if (_GlFunctions_updateGlExtensionsString()) {
    Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
    return Core_Failure;
  }
#if 0
  _GlFunctions_dumpGlExtensionsString();
#endif
  g_initialized = Core_True;
  return Core_Success;
}

Core_Result _GlFunctions_shutdown() {
  if (!g_initialized) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  Core_InlineHashMapPP_uninitialize(&g_instance.extensions);
  g_initialized = Core_False;
  return Core_Success;
}

Core_Result _GlFunctions_getInstance(_GlFunctions** RETURN) {
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
