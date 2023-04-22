struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
  mat4 world_matrix;
};

// vs.matrices
uniform dx_matrices vs_matrices = dx_matrices(mat4(1.), mat4(1.), mat4(1.));

// vs.vertex.xyz
layout (location = 0) in vec3 vs_vertex_xyz;
out vec4 fs_vertex_xyz;

// vs.vertex.uv
layout (location = 1) in vec2 vs_vertex_uv;
out vec2 fs_vertex_uv;

// vs.rgba
uniform vec4 vs_text_color = vec4(0.f, 255.f/255.f, 64.f/255.f, 1.f); // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette
out vec4 fs_text_color;

void main() {
  mat4 pvw = vs_matrices.projection_matrix
           * vs_matrices.view_matrix
           * vs_matrices.world_matrix;
  fs_vertex_xyz = pvw * vec4(vs_vertex_xyz, 1.0);
  gl_Position = fs_vertex_xyz;

  fs_text_color = vs_text_color;
  fs_vertex_uv = vs_vertex_uv;
}
