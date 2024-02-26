// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MOUSEBUTTONACTION_H_INCLUDED)
#define CORE_INPUT_MOUSEBUTTONACTION_H_INCLUDED

#include "dx/core/TypeSystem.h"
typedef struct Core_String Core_String;

/* http://localhost/core#core-mousebuttonaction */
Core_declareEnumerationType("Core.MouseButtonAction",
                            Core_MouseButtonAction);

enum Core_MouseButtonAction {

  Core_MouseButtonAction_Pressed = 1,
  
  Core_MouseButtonAction_Released,

};

#endif // CORE_INPUT_MOUSEBUTTONACTION_H_INCLUDED
