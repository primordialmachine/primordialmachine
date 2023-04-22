in vec4 fs_text_color;
in vec4 fs_vertex_xyz;
in vec2 fs_vertex_uv;

// fs_rgba -> out_rgba
out vec4 out_rgba;

// texture sampler.
uniform sampler2D texture_sampler;

void main() {
  // we are using a LN8 texture.
  // the "red" component contains the LN8 value.
  // Furthermore, invert the texture coordinates to mirror the image.
  float t = texture(texture_sampler, vec2(fs_vertex_uv.x, 1 - fs_vertex_uv.y)).r;
  out_rgba = fs_text_color * vec4(1, 1, 1, t);
}
