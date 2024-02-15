#if !defined(DX_ASSET_MESH_GENERATORS_H_INCLUDED)
#define DX_ASSET_MESH_GENERATORS_H_INCLUDED

#include "dx/assets/mesh.h"

/// @brief Append a triangle to a mesh.
/// @param mesh A pointer to the mesh.
/// @remarks
/// The vertex color is opaque white.
/// @default-return
/// @default-failure.
Core_Result dx_assets_mesh_append_triangle(dx_assets_mesh* mesh);

/// @brief Append a quadriliteral.
/// @param mesh A pointer to the mesh.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
/// The vertex color is opaque white.
/// @default-return
/// @default-failure
Core_Result dx_assets_mesh_append_quadriliteral(dx_assets_mesh* mesh);

/// @brief Create a triangle mesh.
/// @return
/// The zero value on success. A non-zero value on failure.
Core_Result dx_assets_mesh_on_triangle(dx_assets_mesh* mesh);

/// @brief Create a cube mesh.
/// @return
/// The zero value on success. A non-zero value on failure.
/// @remarks
/// Each side has width and height of @a 1.
/// The cube is centered around the origin.
Core_Result dx_assets_mesh_on_cube(dx_assets_mesh* mesh);

/// @brief Create an empty mesh.
/// @return The zero value on success. A non-zero value on failure.
Core_Result dx_assets_mesh_on_empty(dx_assets_mesh* mesh);

/// @brief Create a quadriliteral mesh.
/// @return The zero value on success. A non-zero value on failure.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
Core_Result dx_assets_mesh_on_quadriliteral(dx_assets_mesh* mesh);

Core_Result dx_assets_mesh_on_octahedron(dx_assets_mesh* mesh);

#endif // DX_ASSETS_MESH_GENERATORS_H_INCLUDED
