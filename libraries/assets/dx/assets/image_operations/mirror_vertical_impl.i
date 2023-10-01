static dx_result on_mirror_vertical_operation(dx_assets_image *SELF) {
  for (dx_size column = 0; column < SELF->width / 2; ++column) {
    _swap_columns(SELF, column, SELF->width - 1 - column);
  }
  return dx_get_error() ? DX_FAILURE : DX_SUCCESS;
}
