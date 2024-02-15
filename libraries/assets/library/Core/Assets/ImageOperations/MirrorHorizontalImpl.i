static Core_Result on_mirror_horizontal_operation(Core_Assets_Image* SELF) {
  return Core_SoftwarePixelBuffer_mirrorHorizontal(&SELF->backing);
}
