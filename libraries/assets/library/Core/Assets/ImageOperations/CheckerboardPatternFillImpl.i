
static Core_Result on_checkerboard_pattern_fill_image_operation(Core_Assets_Image* SELF, OFFSET2 offset, EXTEND2 extend, Core_Assets_ImageOperations_CheckerboardPatternFill* image_operation) {
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

  for (Core_Size y = 0; y < image_operation->numberOfCheckersVertical; ++y) {
    Core_Size t = offset.top + y * image_operation->checkerSizeVertical,
              h = image_operation->checkerSizeVertical;
    // Fast clip.
    if (t + h > offset.top + extend.height) {
      continue;
    }
    // Slow clip.
    if (t + h > offset.top + extend.height) {
      Core_Size delta = (t + h) - (offset.top + extend.height);
      h -= delta;
    }
    for (Core_Size x = 0; x < image_operation->numberOfCheckersHorizontal; ++x) {
      Core_Size l = offset.left + x * image_operation->checkerSizeHorizontal,
                w = image_operation->checkerSizeHorizontal;
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
        Core_Assets_Image_apply(SELF, l, t, w, h, CORE_ASSETS_IMAGEOPERATION(first));
      } else {
        Core_Assets_Image_apply(SELF, l, t, w, h, CORE_ASSETS_IMAGEOPERATION(second));
      }
    }
  }

  CORE_UNREFERENCE(second);
  second = NULL;

  CORE_UNREFERENCE(first);
  first = NULL;

  return Core_Success;
}
