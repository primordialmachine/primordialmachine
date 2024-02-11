
static Core_Result on_checkerboard_pattern_fill_image_operation(dx_assets_image* SELF, OFFSET2 offset, EXTEND2 extend, Core_Assets_ImageOperations_CheckerboardPatternFill* image_operation) {
  Core_Assets_ImageOperations_ColorFill* first = NULL;
  if (Core_Assets_ImageOperations_ColorFill_create(&first)) {
    return Core_Failure;
  }
  Core_Assets_ImageOperations_ColorFill* second = NULL;
  if (Core_Assets_ImageOperations_ColorFill_create(&second)) {
    CORE_UNREFERENCE(first);
    first = NULL;
    return Core_Failure;
  }

  if (Core_Assets_ImageOperations_ColorFill_set_color(first, CORE_ASSETS_COLORRGBN8(image_operation->first_checker_color->object)) ||
      Core_Assets_ImageOperations_ColorFill_set_color(second, CORE_ASSETS_COLORRGBN8(image_operation->second_checker_color->object))) {
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
        dx_assets_image_apply(SELF, l, t, w, h, CORE_ASSETS_IMAGEOPERATION(first));
      } else {
        dx_assets_image_apply(SELF, l, t, w, h, CORE_ASSETS_IMAGEOPERATION(second));
      }
    }
  }

  CORE_UNREFERENCE(second);
  second = NULL;

  CORE_UNREFERENCE(first);
  first = NULL;

  return Core_Success;
}
