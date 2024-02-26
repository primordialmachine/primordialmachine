// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_APPLICATIONMESSAGEKIND_H_INCLUDED)
#define CORE_APPLICATIONMESSAGEKIND_H_INCLUDED

#include "dx/core/TypeSystem.h"

Core_declareEnumerationType("Core.ApplicationMessageKind",
                            Core_ApplicationMessageKind);

enum Core_ApplicationMessageKind {
  Core_ApplicationMessageKind_QuitRequested = 1,
};

#endif // CORE_APPLICATIONMESSAGEKIND_H_INCLUDED
