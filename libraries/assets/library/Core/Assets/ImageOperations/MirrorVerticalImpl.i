static Core_Result on_mirror_vertical_operation(Core_Assets_Image *SELF) {
  return Core_SoftwarePixelBuffer_mirrorVertical(&SELF->backing);
}
