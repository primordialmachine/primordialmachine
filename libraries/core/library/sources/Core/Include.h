// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_INCLUDE_H_INCLUDED)
#define CORE_INCLUDE_H_INCLUDED

#include "Core/ApplicationMessage.h"
#include "Core/Bits.h"
#include "Core/Collections/InlineArrayListN8.h"
#include "Core/Collections/InlineArrayListP.h"
#include "Core/CountLeadingZeroes.h"
#include "Core/Convert.h"
#include "Core/EmitMessage.h"
#include "Core/Error.h"
#include "Core/Numerics.h"
#include "Core/FileSystem/deleteRegularFile.h"
#include "Core/FileSystem/ExistingFilePolicy.h"
#include "Core/FileSystem/FileAccessMode.h"
#include "Core/FileSystem/FileType.h"
#include "Core/FileSystem/getFileContents.h"
#include "Core/FileSystem/getFileType.h"
#include "Core/FileSystem/NonExistingFilePolicy.h"
#include "Core/FileSystem/setFileContents.h"
#include "Core/Hash.h"
#include "Core/InlineByteIterator.h"
#include "Core/Utf8.h"
#include "Core/Input/InputMessageKind.h"
#include "Core/Input/InputMessage.h"
#include "Core/Input/KeyboardKey.h"
#include "Core/Input/KeyboardKeyAction.h"
#include "Core/Input/KeyboardKeyMessage.h"
#include "Core/Input/ModifierKeys.h"
#include "Core/Input/MouseButton.h"
#include "Core/Input/MouseButtonAction.h"
#include "Core/Input/MouseButtonMessage.h"
#include "Core/Input/MousePointerAction.h"
#include "Core/Input/MousePointerMessage.h"
#include "Core/Memory.h"
#include "Core/NextPowerOfTwo.h"
#include "Core/Result.h"
#include "Core/safeAddNx.h"
#include "Core/safeMulIx.h"
#include "Core/safeMulNx.h"
#include "Core/Scanner.h"
#include "Core/String.h"
#include "Core/Time.h"
#include "Core/Visuals/CanvasMessage.h"

#endif // CORE_INCLUDE_H_INCLUDED
