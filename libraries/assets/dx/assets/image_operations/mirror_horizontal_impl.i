static dx_result on_mirror_horizontal_operation(dx_assets_image* SELF) {
  for (dx_size row = 0; row < SELF->height / 2; ++row) {
	_swap_rows(SELF, row, SELF->height - 1 - row);
  }
  return dx_get_error() ? DX_FAILURE : DX_SUCCESS;
}
