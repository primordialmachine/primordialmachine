static Core_Result on_mirror_horizontal_operation(dx_assets_image* SELF) {
  for (Core_Size row = 0; row < SELF->height / 2; ++row) {
	_swap_rows(SELF, row, SELF->height - 1 - row);
  }
  return Core_getError() ? Core_Failure : Core_Success;
}
