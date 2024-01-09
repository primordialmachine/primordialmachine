// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "common.h"
#include "wic-plugin.h"

static int
read_image_1
  (
    wchar_t const* path,
    DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
    void** target_bytes, 
    DX_WIC_PLUGIN_PIXEL_FORMAT* target_pixel_format,
    uint32_t* target_stride,
    uint32_t* target_width,
    uint32_t* target_height,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate
  )
{
  int result = 0;

  IWICBitmapSource* pIWICBitmapSource = NULL;
  IWICBitmapFrameDecode* pIWICBitmapFrameDecode = NULL;
  IWICStream* pIWICStream = NULL;
  IWICBitmapDecoder* pIWICBitmapDecoder = NULL;
  IWICImagingFactory* pIWICFactory = NULL;
  HRESULT hr;
  //
  hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr =
    CoCreateInstance
      (
        &CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory,
        (LPVOID*)&pIWICFactory
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  GUID const* guidContainerFormat = NULL;
  switch (source_image_format) {
    case DX_WIC_PLUGIN_IMAGE_FORMAT_PNG: {
      guidContainerFormat = &GUID_ContainerFormatPng;
    } break;
    case DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF: {
      guidContainerFormat = &GUID_ContainerFormatTiff;
    } break;
    default: {
      result = 1;
      goto END;
    } break;
  }
  hr =
    IWICImagingFactory_CreateDecoder
      (
        pIWICFactory,
        guidContainerFormat,
        NULL,                  // Do not prefer a particular vendor
        &pIWICBitmapDecoder
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr =
    IWICImagingFactory_CreateStream
      (
        pIWICFactory,
        &pIWICStream
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  hr =
      IWICStream_InitializeFromFilename
        (
          pIWICStream,
          path,
          GENERIC_READ
        );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr =
    IWICBitmapDecoder_Initialize
      (
        pIWICBitmapDecoder,
        (IStream*)pIWICStream,
        WICDecodeMetadataCacheOnDemand
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr =
    IWICBitmapDecoder_GetFrame
      (
        pIWICBitmapDecoder,
        0,
        &pIWICBitmapFrameDecode
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  hr =
    IWICBitmapFrameDecode_QueryInterface
      (
        pIWICBitmapFrameDecode,
        &IID_IWICBitmapSource,
        (void**)&pIWICBitmapSource
      );
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  // Get the size (width and height) and the pixel format of the image.
  // Assert the image is BGR 24bpp.
  UINT uiWidth, uiHeight;
  hr = IWICBitmapSource_GetSize(pIWICBitmapSource, &uiWidth, &uiHeight);
  if (FAILED(hr) || uiWidth > UINT32_MAX || uiHeight > UINT32_MAX) {
    result = 1;
    goto END;
  }
  WICPixelFormatGUID wicPixelFormatGUID;
  hr = IWICBitmapSource_GetPixelFormat(pIWICBitmapSource, &wicPixelFormatGUID);
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
#if 0
  //
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat24bpp3Channels)) {
    result = 1;
    goto END;
  }
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat24bppBGR)) {
    result = 1;
    goto END;
  }
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat24bppRGB)) {
    result = 1;
    goto END;
  }
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat32bppRGBA)) {
    result = 1;
    goto END;
  }
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat32bppBGRA)) {
    result = 1;
    goto END;
  }
  if (IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat24bppRGB)) {
    result = 1;
    goto END;
  }
#endif
  hr = IsEqualGUID(&wicPixelFormatGUID, &GUID_WICPixelFormat24bppBGR) ? S_OK : E_FAIL;
  if (FAILED(hr)) {
    result = 1;
    goto END;
  }
  //
  UINT uiStride;
  hr = UIntMult(uiWidth, 3, &uiStride);
  if (FAILED(hr) || uiStride > UINT32_MAX) {
    result = 1;
    goto END;
  }
  UINT uiSize;
  hr = UIntMult(uiStride, uiHeight, &uiSize);
  if (FAILED(hr) || uiSize > UINT32_MAX) {
    result = 1;
    goto END;
  }
  void* target_bytes_ = allocate(context, (size_t)uiSize);
  if (!target_bytes_) {
    result = 1;
    goto END;
  }
  hr =
    IWICBitmapSource_CopyPixels
      (
        pIWICBitmapSource,
        NULL,
        uiStride, // stride
        uiSize, // size in bytes of the image
        (BYTE*)target_bytes_
      );
  if (FAILED(hr)) {
    deallocate(context, target_bytes_);
    target_bytes_ = NULL;
    result = 1;
    goto END;
  }
  *target_bytes = target_bytes_;
  *target_stride = (uint32_t)uiStride;
  *target_width = (uint32_t)uiWidth;
  *target_height = (uint32_t)uiHeight;
  *target_pixel_format = (uint32_t)DX_WIC_PLUGIN_PIXEL_FORMAT_BN8_GN8_RN8;
END:
  //
  if (pIWICBitmapSource) {
    IWICBitmapSource_Release(pIWICBitmapSource);
    pIWICBitmapSource = NULL;
  }
  //
  if (pIWICBitmapFrameDecode) {
    IWICBitmapFrameDecode_Release(pIWICBitmapFrameDecode);
    pIWICBitmapFrameDecode = NULL;
  }
  //
  if (pIWICStream) {
    IWICStream_Release(pIWICStream);
    pIWICStream = NULL;
  }
  //
  if (pIWICBitmapDecoder) {
    IWICBitmapDecoder_Release(pIWICBitmapDecoder);
    pIWICBitmapDecoder = NULL;
  }
  //
  if (pIWICFactory) {
    IWICImagingFactory_Release(pIWICFactory);
    pIWICFactory = NULL;
  }
  //
  return result;
}

__declspec(dllexport) int
read_image
  (
    char const* path,
    DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
    void** target_bytes, 
    DX_WIC_PLUGIN_PIXEL_FORMAT* target_pixel_format,
    uint32_t* target_stride,
    uint32_t* target_width,
    uint32_t* target_height,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate,
    DX_WIC_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate
  )
{
  if (!path) {
    return 1;
  }
  wchar_t* path_1 = mb_to_wc(path);
  if (!path_1) {
    return 1;
  }
  if (read_image_1(path_1,
                   source_image_format,
                   target_bytes,
                   target_pixel_format,
                   target_stride,
                   target_width,
                   target_height,
                   context,
                   allocate,
                   deallocate)) {
    free(path_1);
    path_1 = NULL;
    return 1;
  }
  free(path_1);
  return 0;
}
