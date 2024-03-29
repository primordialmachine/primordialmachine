#if !defined(DX_ASSETS_H_INCLUDED)
#define DX_ASSETS_H_INCLUDED


#include "Core/Assets/ColorRgbN8.h"
#include "Core/Assets/Context.h"
#include "Core/Assets/Image.h"
#include "Core/Assets/ImageOperation.h"
#include "Core/Assets/ImageOperations/ColorFill.h"
#include "Core/Assets/ImageOperations/CheckerboardPatternFill.h"
#include "Core/Assets/Material.h"
#include "Core/Assets/MaterialController.h"
#include "Core/Assets/MaterialControllers/AmbientColor.h"
#include "Core/Assets/Matrix4x4R32.h"
#include "dx/assets/mesh.h"
#include "dx/assets/mesh_instance.h"
#include "dx/assets/mesh_operation.h"
#include "dx/assets/mesh_operations/set_vertex_colors.h"
#include "Core/Assets/Optics.h"
#include "Core/Assets/OrthographicOptics.h"
#include "Core/Assets/PerspectiveOptics.h"
#include "Core/Assets/Ref.h"
#include "Core/Assets/Def.h"
#include "dx/assets/scene.h"
#include "Core/Assets/System.h"
#include "Core/Assets/SystemFactory.h"
#include "dx/assets/texture.h"
#include "Core/Assets/Vector3R32.h"
#include "Core/Assets/Viewer.h"
#include "Core/Assets/ViewerController.h"
#include "Core/Assets/ViewerControllers/RotateY.h"
#include "dx/assets/viewer_instance.h"


/// @brief The name of this library.
#define DX_ASSETS_LIBRARY_NAME "Assets"

/// @brief The major version of this library.
#define DX_ASSETS_LIBRARY_MAJOR_VERSION (0)

/// @brief The minor version of this library.
#define DX_ASSETS_LIBRARY_MINOR_VERSION (1)


#endif // DX_ASSETS_H_INCLUDED
