/// @file dx/core.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @brief Main include file for core functionality.
#if !defined(DX_CORE_H_INCLUDED)
#define DX_CORE_H_INCLUDED

/// @defgroup Core_Annotations Core:Annotations
/// Functionality related to annotations.

/// @defgroup Core_BuiltinTypes Core:Builtin Types
/// Functionality related to builtin types.

/// @defgroup Core_Strings Core:Strings
/// Functionality related to Strings.

/// @defgroup Core_Aggregates Core:Aggregates
/// Functionality related to Aggregates.

/// @defgroup Core_Visuals Core:Visuals
/// Functionality related to visuals.

/// @defgroup Core_Audials Core:Audials
/// Functionality related to audials.

/// @defgroup Core_Hapticals Core:Hapticals
/// Functionality related to haptics.

/// @defgroup Core_FileSystem Core:FileSystem
/// Functionality related to the file system.

#include "dx/core/byte_array.h"
#include "dx/core/canvas_msgs.h"
#include "dx/core/convert.h"
#include "dx/core/core.h"
#include "dx/core/count_leading_zeroes.h"
#include "dx/core/error.h"
#include "dx/core/file_system.h"
#include "dx/core/math.h"
#include "dx/core/memory.h"
#include "dx/core/msgs.h"
#include "dx/core/next_power_of_two.h"
#include "dx/core/object.h"
#include "dx/core/object_array.h"
#include "dx/core/os.h"
#include "dx/core/inline_object_array.h"
#include "dx/core/inline_pointer_array.h"
#include "dx/core/inline_pointer_hashmap.h"
#include "dx/core/safe_add_nx.h"
#include "dx/core/safe_mul_ix.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/scanner.h"
#include "dx/core/string.h"
#include "dx/core/string_buffer.h"
#include "dx/core/visuals.h"

#include "dx/core/system.h"
#include "dx/core/system_factory.h"

// begin: dx/core/input
#include "dx/core/hapticals.h"
#include "dx/core/keyboard_state.h"
#include "dx/core/mouse_state.h"
// end: dx/core/input


/// @brief The name of this library.
#define DX_CORE_LIBRARY_NAME "Core"

/// @brief The major version of this library.
#define DX_CORE_LIBRARY_MAJOR_VERSION (0)

/// @brief The minor version of this library.
#define DX_CORE_LIBRARY_MINOR_VERSION (1)


/// @brief Macro evaluating to <code>__FILE__</code>.
#define DX_C_FILE_NAME __FILE__

/// @brief Macro evaluating to <code>__LINE__</code>.
#define DX_C_FILE_LINE __LINE__

/// @brief Macro evaluating to <code>__func__</code>.
#define DX_C_FUNCTION_NAME __func__


#endif // DX_CORE_H_INCLUDED
