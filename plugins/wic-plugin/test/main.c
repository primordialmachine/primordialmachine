// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com)
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "wic-plugin.h"

#include "dx/core.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// LoadLibrary, GetProcAddress, FreeLibrary
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void* allocate(void* context, size_t n) {
  Core_Error old_error = Core_getError();
  void* p = NULL;
  if (Core_Memory_allocate(&p, n)) {
    Core_setError(old_error);
    return NULL;
  }
  return p;
}

static void deallocate(void* context, void* p) {
  if (p) {
    free(p);
  }
}

static Core_Result test_read_png() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == hModule) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  DX_WIC_PLUGIN_READ_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_READ_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  void* bytes;
  uint32_t stride;
  uint8_t pixel_format;
  uint32_t width;
  uint32_t height;
  if (procedure("hello.png", DX_WIC_PLUGIN_IMAGE_FORMAT_PNG, &bytes, &pixel_format, &stride, &width, &height, NULL, &allocate, &deallocate)) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  FreeLibrary(hModule);
  hModule = NULL;
  return Core_Success;
}

static Core_Result test_write_tiff() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == hModule) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  DX_WIC_PLUGIN_WRITE_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_WRITE_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  uint32_t source_stride = 256 * (24/8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t *buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.tiff", buffer, DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8, source_stride, source_width, source_height, DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return Core_Success;
}

static Core_Result test_write_png() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME);
  if (NULL == hModule) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  DX_WIC_PLUGIN_WRITE_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_WRITE_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  uint32_t source_stride = 256 * (24 / 8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t* buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.png", buffer, DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8, source_stride, source_width, source_height, DX_WIC_PLUGIN_IMAGE_FORMAT_PNG)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return Core_Success;
}

int main(int argc, char** argv) {
  if (test_write_tiff() || test_write_png()) {
    return EXIT_FAILURE;
  }
  if (test_read_png()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
