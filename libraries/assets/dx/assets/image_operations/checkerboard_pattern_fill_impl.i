
static Core_Result on_checkerboard_pattern_fill_image_operation(dx_assets_image* SELF, OFFSET2 offset, EXTEND2 extend, dx_assets_image_operations_checkerboard_pattern_fill* image_operation) {
  dx_assets_image_operations_color_fill* first = NULL;
  if (dx_assets_image_operations_color_fill_create(&first)) {
    return Core_Failure;
  }
  dx_assets_image_operations_color_fill* second = NULL;
  if (dx_assets_image_operations_color_fill_create(&second)) {
    CORE_UNREFERENCE(first);
    first = NULL;
    return Core_Failure;
  }

  if (dx_assets_image_operations_color_fill_set_color(first, DX_ASSETS_COLOR_RGB_N8(image_operation->first_checker_color->object)) ||
      dx_assets_image_operations_color_fill_set_color(second, DX_ASSETS_COLOR_RGB_N8(image_operation->second_checker_color->object))) {
    CORE_UNREFERENCE(first);
    first = NULL;
    return Core_Failure;
  }

  for (Core_Size y = 0; y < image_operation->number_of_checkers_vertical; ++y) {
    Core_Size t = offset.top + y * image_operation->checker_size_vertical,
            h = image_operation->checker_size_vertical;
    // Fast clip.
    if (t + h > offset.top + extend.height) {
      continue;
    }
    // Slow clip.
    if (t + h > offset.top + extend.height) {
      Core_Size delta = (t + h) - (offset.top + extend.height);
      h -= delta;
    }
    for (Core_Size x = 0; x < image_operation->number_of_checkers_horizontal; ++x) {
      Core_Size l = offset.left + x * image_operation->checker_size_horizontal,
              w = image_operation->checker_size_horizontal;
      // Fast clip.
      if (l > offset.left + extend.width) {
        continue;
      }
      // Slow clip.
      if (l + w > offset.left + extend.width) {
        Core_Size delta = (l + w) - (offset.left + extend.width);
        w -= delta;
      }
      int even_x = x % 2 == 0;
      int even_y = y % 2 == 0;
      if (even_x != even_y) {
        dx_assets_image_apply(SELF, l, t, w, h, DX_ASSETS_IMAGE_OPERATION(first));
      } else {
        dx_assets_image_apply(SELF, l, t, w, h, DX_ASSETS_IMAGE_OPERATION(second));
      }
    }
  }

  CORE_UNREFERENCE(second);
  second = NULL;

  CORE_UNREFERENCE(first);
  first = NULL;

  return Core_Success;
}
