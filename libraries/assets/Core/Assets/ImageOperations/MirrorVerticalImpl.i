static Core_Result on_mirror_vertical_operation(Core_Assets_Image *SELF) {
  for (Core_Size column = 0; column < SELF->width / 2; ++column) {
    _swap_columns(SELF, column, SELF->width - 1 - column);
  }
  return Core_getError() ? Core_Failure : Core_Success;
}
