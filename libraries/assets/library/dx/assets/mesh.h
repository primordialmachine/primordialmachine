#if !defined(DX_ASSETS_MESH_H_INCLUDED)
#define DX_ASSETS_MESH_H_INCLUDED

#include "Core/Assets/Material.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A mesh asset.
/// @detail
/// A mesh consists of vertices. Each vertex always provides the following information:
/// xyz, rgba ambient, uv ambient.
/// xyz denotes the position coordinates of the vertex in model space.
/// rgba ambient denotes the color coordinates and the alpha of the vertex in the non-pbr lighting model.
/// uv ambient denotes the texture coordinates for the rgba ambient texture in the non-pbr ligthting model.
/// A mesh always has a dx_asset_material associated.
Core_declareObjectType("Core.Assets.Mesh",
                       dx_assets_mesh,
                       Core_Object);

static inline dx_assets_mesh* DX_ASSETS_MESH(void* p) {
  return (dx_assets_mesh*)p;
}

struct dx_assets_mesh {
  Core_Object _parent;

  /// @brief A pointer to the ADL name of this image.
  Core_String* name;

  /// @brief Reference to the material of this mesh.
  Core_Assets_Ref* material_reference;

  /// @brief The number of vertices of this mesh.
  uint32_t number_of_vertices;

  /// @brief The format of the vertices of this mesh.
  Core_VertexFormat vertex_format;

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
    Core_InlineVector2R32* ambient_uv;
  } vertices;

  /// @brief The operations that eventually create the mesh.
  dx_inline_object_array operations;
};

static inline dx_assets_mesh_Dispatch* DX_ASSETS_MESH_DISPATCH(void* p) {
  return (dx_assets_mesh_Dispatch*)p;
}

struct dx_assets_mesh_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Generate a mesh.
/// @param RETURN A pointer to a <code>dx_assets_mesh*</code> variable.
/// @param name A pointer to the *Asset Definition Language* name of this mesh.
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
/// @create-operator{dx_assets_mesh}
Core_Result dx_assets_mesh_create(dx_assets_mesh** RETURN, Core_String* name, Core_String* specifier, Core_VertexFormat vertex_format, Core_Assets_Ref* material_reference);

/// @brief Pack the mesh data into a single stream of the specified format.
/// @param vertex_format The vertex format to pack the mesh data in.
/// @param bytes A pointer to a <code>void*</code> variable.
/// @param number_of_bytes A pointer to a <code>Core_Size</code> variable.
/// @success
/// <code>*number_of_bytes</code> was assigned the length, in Bytes, of the packed mesh data.
/// <code>*bytes</code> was assigned a pointer to an array of length <code>*number_of_bytes</code>.
/// @method{dx_assets_mesh}
Core_Result dx_assets_mesh_format(dx_assets_mesh* SELF, Core_VertexFormat vertex_format, void **bytes, Core_Size*number_of_bytes);

/// @brief Transform a range of vertices.
/// @param a A pointer to the transformation matrix.
/// @param i The index of the vertex at which the range starts.
/// @param n The number of vertices in the range.
/// @method{dx_assets_mesh}
Core_Result dx_assets_mesh_transform_range(dx_assets_mesh* SELF, DX_MAT4 const* a, Core_Size i, Core_Size n);

/// @brief Append a vertex.
/// @param xyz, ambient_rgba, ambient_uv The mesh data.
/// @method{dx_assets_mesh}
Core_Result dx_assets_mesh_append_vertex(dx_assets_mesh* SELF,
                                         DX_VEC3 const* xyz,
                                         DX_VEC4 const* ambient_rgba,
                                         Core_InlineVector2R32 const* ambient_uv);

/// @brief Remove all vertices.
/// @method{dx_assets_mesh}
Core_Result dx_assets_mesh_clear(dx_assets_mesh* SELF);

/// @brief Set the "mesh ambient rgba" value.
/// @param SELF A pointer to this mesh.
/// @param color A pointer to "mesh ambient rgba" value.
void dx_assets_mesh_set_mesh_ambient_rgba(dx_assets_mesh* SELF, DX_VEC4 const* value);

/// @brief Append the specified range to the end of this mesh.
/// @param i The index of the vertex at which the range starts.
/// @param n The length, in vertices, of the range.
/// @method{dx_assets_mesh}
Core_Result dx_assets_mesh_append_range(dx_assets_mesh* SELF, Core_Size i, Core_Size n);

#endif // DX_ASSETS_MESH_H_INCLUDED