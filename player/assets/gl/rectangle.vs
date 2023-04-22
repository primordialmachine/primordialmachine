struct dx_matrices {
  mat4 projection_matrix;
  mat4 view_matrix;
  mat4 world_matrix;
};

// vs.matrices
uniform dx_matrices vs_matrices = dx_matrices(mat4(1.), mat4(1.), mat4(1.));

// vs.vertex.xyz
layout (location = 0) in vec3 vs_vertex_xyz;


// vs.rgba
uniform vec4 vs_rgba = vec4(0.f, 255.f/255.f, 64.f/255.f, 1.f); // color called "Malachite" (0, 255, 64) from "Capri"'s tetradic palette

// fs.rgba
out vec4 fs_rgba;

void main() {
  mat4 pvw = vs_matrices.projection_matrix
           * vs_matrices.view_matrix
           * vs_matrices.world_matrix;

  gl_Position = pvw * vec4(vs_vertex_xyz, 1.0);
  fs_rgba = vs_rgba;
}
