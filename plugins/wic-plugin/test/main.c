// Copyright (c) 2023 Michael Heilmann. All rights reserved.
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
  int old_error = dx_get_error();
  void* p = NULL;
  if (dx_memory_allocate(&p, n)) {
    dx_set_error(old_error);
    return NULL;
  }
  return p;
}

static void deallocate(void* context, void* p) {
  if (p) {
    free(p);
  }
}

static dx_result test_read_png() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == hModule) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  DX_WIC_PLUGIN_READ_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_READ_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  void* bytes;
  uint32_t stride;
  uint8_t pixel_format;
  uint32_t width;
  uint32_t height;
  if (procedure("hello.png", DX_WIC_PLUGIN_IMAGE_FORMAT_PNG, &bytes, &pixel_format, &stride, &width, &height, NULL, &allocate, &deallocate)) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  dx_memory_deallocate(bytes);
  FreeLibrary(hModule);
  hModule = NULL;
  return DX_SUCCESS;
}

static dx_result test_write_tiff() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME ".dll");
  if (NULL == hModule) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  DX_WIC_PLUGIN_WRITE_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_WRITE_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  uint32_t source_stride = 256 * (24/8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t *buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.tiff", buffer, DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8, source_stride, source_width, source_height, DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return DX_SUCCESS;
}

static dx_result test_write_png() {
  HMODULE hModule = LoadLibraryA("./" DX_WIC_PLUGIN_LIBRARY_NAME);
  if (NULL == hModule) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  DX_WIC_PLUGIN_WRITE_IMAGE_PROC* procedure = (DX_WIC_PLUGIN_WRITE_IMAGE_PROC*)GetProcAddress(hModule, DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME);
  if (!procedure) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  uint32_t source_stride = 256 * (24 / 8);
  uint32_t source_width = 256;
  uint32_t source_height = 256;
  uint8_t* buffer = malloc(sizeof(uint8_t) * source_stride * source_height);
  if (!buffer) {
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  for (size_t i = 0; i < sizeof(uint8_t) * source_stride * source_height; i++) {
    buffer[i] = (uint8_t)rand();
  }
  if (procedure("hello.png", buffer, DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8, source_stride, source_width, source_height, DX_WIC_PLUGIN_IMAGE_FORMAT_PNG)) {
    free(buffer);
    buffer = NULL;
    FreeLibrary(hModule);
    hModule = NULL;
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  free(buffer);
  buffer = NULL;
  FreeLibrary(hModule);
  hModule = NULL;
  return DX_SUCCESS;
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
