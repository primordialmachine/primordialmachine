#if !defined(DX_ASSET_MESH_GENERATORS_H_INCLUDED)
#define DX_ASSET_MESH_GENERATORS_H_INCLUDED

#include "dx/assets/mesh.h"

/// @brief Append a triangle to a mesh.
/// @param mesh A pointer to the mesh.
/// @remarks
/// The vertex color is opaque white.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
Core_Result Core_Assets_Mesh_AppendTriangle(Core_Assets_Mesh* mesh);

/// @brief Append a quadriliteral.
/// @param mesh A pointer to the mesh.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
/// The vertex color is opaque white.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
Core_Result Core_Assets_Mesh_appendQuadriliteral(Core_Assets_Mesh* mesh);

/// @brief Create a triangle mesh.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
Core_Result Core_Assets_Mesh_onTriangle(Core_Assets_Mesh* mesh);

/// @brief Create a cube mesh.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
/// @remarks
/// Each side has width and height of @a 1.
/// The cube is centered around the origin.
Core_Result Core_Assets_Mesh_onCube(Core_Assets_Mesh* mesh);

/// @brief Create an empty mesh.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
Core_Result Core_Assets_Mesh_onEmpty(Core_Assets_Mesh* mesh);

/// @brief Create a quadriliteral mesh.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
/// @remarks
/// The width and height of each side is @a 1.
/// The center is <code>(0,0,0)</code>.
/// The normal is <code>(0,0,+1)</code>.
Core_Result Core_Assets_Mesh_onQuadriliteral(Core_Assets_Mesh* mesh);

/// @brief Create an octahedron mesh.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
Core_Result Core_Assets_Mesh_onOctahedron(Core_Assets_Mesh* mesh);

#endif // DX_ASSETS_MESH_GENERATORS_H_INCLUDED
