#if !defined(DX_ASSET_MESH_H_INCLUDED)
#define DX_ASSET_MESH_H_INCLUDED

#include "dx/assets/material.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// If the number of indices is three, then a face is called a triangle.
/// If the number of indices is greater than three, then the face is called a polygon.
/// A face of four indices is also called a quadriliteral.
DX_DECLARE_OBJECT_TYPE("dx.asset.face",
                       dx_asset_face,
                       dx_object)

static inline dx_asset_face* DX_ASSET_FACE(void* p) {
  return (dx_asset_face*)p;
}

struct dx_asset_face {
  dx_object _parent;
  dx_size number_of_indices;
  uint32_t *indices;
};

struct dx_asset_face_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Construct this face.
/// @param SELF A pointer to this face.
/// @param a The first index.
/// @param b The second index.
/// @param c The third index.
/// @default-return
/// @default-failure
int dx_asset_face_construct_triangle(dx_asset_face* SELF, uint32_t a, uint32_t b, uint32_t c);

dx_asset_face* dx_asset_face_create_triangle(uint32_t a, uint32_t b, uint32_t c);

/// @brief Construct this face.
/// @param SELF A pointer to this face.
/// @param a The first index.
/// @param b The second index.
/// @param c The third index.
/// @param c The fourth index.
/// @default-return
/// @default-failure
int dx_asset_face_construct_quadriliteral(dx_asset_face* SELF, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

dx_asset_face* dx_asset_face_create_quadriliteral(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

/// @brief Construct this face.
/// @param SELF A pointer to this face.
/// @param other A pointer of the face to copy.
/// @default-return
/// @default-failure
int dx_asset_face_construct_copy(dx_asset_face* SELF, dx_asset_face* other);

dx_asset_face* dx_asset_face_create_copy(dx_asset_face* other);

/// @brief Convert this face into triangles. Add those triangles to the specified list.
/// @param SELF 
/// @param faces 
/// @return 
/// @remarks If the face is a triangle, then its copy is added to the list.
/// If the face is a quadriliteral (with four indices i0, i1, i2, i3) then it is split at indices i0 and i2.
int dx_asset_face_to_triangles(dx_asset_face* SELF, dx_inline_object_array* faces);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A mesh asset.
/// @detail
/// A mesh consists of vertices. Each vertex always provides the following information:
/// xyz, rgba ambient, uv ambient.
/// xyz denotes the position coordinates of the vertex in model space.
/// rgba ambient denotes the color coordinates and the alpha of the vertex in the non-pbr lighting model.
/// uv ambient denotes the texture coordinates for the rgba ambient texture in the non-pbr ligthting model.
/// A mesh always has a dx_asset_material associated.
DX_DECLARE_OBJECT_TYPE("dx.asset.mesh",
                       dx_asset_mesh,
                       dx_object)

static inline dx_asset_mesh* DX_ASSET_MESH(void* p) {
  return (dx_asset_mesh*)p;
}

struct dx_asset_mesh {
  dx_object _parent;

  /// @brief A pointer to the ADL name of this image.
  dx_string* name;

  /// @brief Reference to the material of this mesh.
  dx_asset_reference* material_reference;

  /// @brief The number of vertices of this mesh.
  uint32_t number_of_vertices;

  /// @brief The format of the vertices of this mesh.
  DX_VERTEX_FORMAT vertex_format;

  struct {
    /// The mesh ambient "rgba" value.
    DX_VEC4 ambient_rgba;
  } mesh;

  struct {
    /// Pointer to an array of number_of_vertices DX_VEC3 objects.
    /// These objects are the per-vertex "xyz" values.
    DX_VEC3* xyz;
    /// Pointer to an array of number_of_vertices DX_VEC4 objects.
    /// These objects are the per-vertex ambient "rgba" values.
    DX_VEC4* ambient_rgba;
    /// Pointer to an array of number_of_vertices DX_VEC2 objects.
    /// These objects are the per-vertex ambient "uv" values.
    DX_VEC2* ambient_uv;
  } vertices;

  /// @brief The operations that eventually create the mesh.
  dx_inline_object_array operations;
};

static inline dx_asset_mesh_dispatch* DX_ASSET_MESH_DISPATCH(void* p) {
  return (dx_asset_mesh_dispatch*)p;
}

struct dx_asset_mesh_dispatch {
  dx_object_dispatch _parent;
};

/// @brief Generate a mesh.
/// @param RETURN A pointer to a <code>dx_asset_mesh*</code> variable.
/// @param name A pointer to the ADL name of this mesh.
/// @param specifier The specifier.
/// @param material_reference The material reference of this mesh.
/// @success <code>*RETURN</code> was assigned a pointer to the mesh.
/// @remarks
/// A "specifier" specifies what mesh this function shall return.
/// The following specifiers are currently supported:
/// - "triangle" a triangle mesh
/// - "quadriliteral" a quadriliteral mesh
/// - "cube" a cube mesh
/// - "empty" an empty mesh
/// @method-call
dx_result dx_asset_mesh_create(dx_asset_mesh** RETURN, dx_string* name, dx_string* specifier, DX_VERTEX_FORMAT vertex_format, dx_asset_reference* material_reference);

/// @brief Pack the mesh data into a single stream of the specified format.
/// @param SELF A pointer to this mesh.
/// @param vertex_format The vertex format to pack the mesh data in.
/// @param bytes A pointer to a <code>void*</code> variable.
/// @param number_of_bytes A pointer to a <code>dx_size</code> variable.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the packed mesh data.
/// <code>*bytes</code> was assigned a pointer to an array of length <code>*number_of_bytes</code>.
int dx_asset_mesh_format(dx_asset_mesh* SELF, DX_VERTEX_FORMAT vertex_format, void **bytes, dx_size*number_of_bytes);

/// @brief Transform a range of vertices.
/// @param SELF A pointer to this mesh.
/// @param a A pointer to the transformation matrix.
/// @param i The index of the vertex at which the range starts.
/// @param n The number of vertices in the range.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_transform_range(dx_asset_mesh* SELF, DX_MAT4 const* a, dx_size i, dx_size n);

/// @brief Append a vertex.
/// @param SELF A pointer to this mesh.
/// @param xyz, ambient_rgba, ambient_uv The mesh data.
/// @return The zero value on success. A non-zero value on failre.
int dx_asset_mesh_append_vertex(dx_asset_mesh* SELF,
                                DX_VEC3 const* xyz,
                                DX_VEC4 const* ambient_rgba,
                                DX_VEC2 const* ambient_uv);

/// @brief Remove all vertices.
/// @param SELF A pointer to this mesh.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_clear(dx_asset_mesh* SELF);

/// @brief Set the "mesh ambient rgba" value.
/// @param SELF A pointer to this mesh.
/// @param color A pointer to "mesh ambient rgba" value.
void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* SELF, DX_VEC4 const* value);

/// @brief Append the specified range to the end of this mesh.
/// @param SELF A pointer to this mesh.
/// @param i The index of the vertex at which the range starts.
/// @param n The length, in vertices, of the range.
/// @return The zero value on success. A non-zero value on failure.
int dx_asset_mesh_append_range(dx_asset_mesh* SELF, dx_size i, dx_size n);

#endif // DX_ASSET_MESH_H_INCLUDED
