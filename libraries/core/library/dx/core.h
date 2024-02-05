/// @file dx/core.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @brief Main include file for core functionality.
#if !defined(DX_CORE_H_INCLUDED)
#define DX_CORE_H_INCLUDED

/// @defgroup Core_Annotations Core:Annotations
/// Functionality related to annotations.

/// @defgroup Core_FundamentalTypes Core:Fundamental Types
/// Functionality related to fundamental types.

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

#include "dx/core/context.h"
#include "dx/core/core.h"

#include "Core/Bits.h"
#include "Core/CountLeadingZeroes.h"
#include "Core/Convert.h"
#include "Core/Error.h"
#include "Core/FileSystem/ExistingFilePolicy.h"
#include "Core/FileSystem/FileAccessMode.h"
#include "Core/FileSystem/FileType.h"
#include "Core/FileSystem/getFileContents.h"
#include "Core/FileSystem/getFileType.h"
#include "Core/FileSystem/NonExistingFilePolicy.h"
#include "Core/Hash.h"
#include "Core/Input/KeyboardKey.h"
#include "Core/Input/KeyboardKeyMessage.h"
#include "Core/Input/MouseButton.h"
#include "Core/Input/MouseButtonMessage.h"
#include "Core/Input/MousePointerMessage.h"
#include "Core/Input/ModifierKeys.h"
#include "Core/Memory.h"
#include "Core/NextPowerOfTwo.h"
#include "Core/Result.h"
#include "Core/safeAddNx.h"
#include "Core/safeMulIx.h"
#include "Core/safeMulNx.h"
#include "Core/Time.h"
#include "Core/Visuals/CanvasMessage.h"

#include "dx/core/math.h"
#include "dx/core/msgs.h"
#include "dx/core/object.h"
#include "dx/core/object_array.h"
#include "dx/core/os.h"
#include "Core/Collections/InlineArrayListN8.h"
#include "Core/Collections/InlineArrayListP.h"
#include "dx/core/inline_object_array.h"
#include "dx/core/inline_pointer_hashmap.h"
#include "dx/core/scanner.h"
#include "dx/core/signals.h"
#include "dx/core/string.h"
#include "dx/core/string_buffer.h"
#include "dx/core/visuals.h"
#include "dx/core/system.h"
#include "dx/core/system_factory.h"

// begin: dx/core/input
#include "dx/core/keyboard_state.h"
#include "dx/core/mouse_state.h"
// end: dx/core/input

// begin: dx/core/text
#include "dx/core/text/anchor.h"
#include "dx/core/text/bounds_type.h"
// end: dx/core/text


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


/// @brief Initialize the "Core" module.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @undefined The "Core" module is already initialized.
/// @undefined This function is called from a thread other than the main thread.
/// @failure This function sets the by-thread error variable.
Core_Result Core_initialize();

/// @brief Uninitialize the "Core" module.
/// @return #Core_Success on success. #Core_Failure on failure.
/// @undefined The "Core" module is not yet initialized.
/// @undefined This function is called from a thread other than the main thread.
/// @failure This function sets the by-thread error variable.
Core_Result Core_uninitialize();

#endif // DX_CORE_H_INCLUDED
