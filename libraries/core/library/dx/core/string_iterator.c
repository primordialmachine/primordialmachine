#include "dx/core/string_iterator.h"

Core_defineObjectType("dx.string_iterator",
                       dx_string_iterator,
                       Core_Object);

static void dx_string_iterator_destruct(dx_string_iterator* SELF)
{/*Intentionally empty.*/}

static void dx_string_iterator_constructDispatch(dx_string_iterator_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_string_iterator_construct(dx_string_iterator* SELF) {
  DX_CONSTRUCT_PREFIX(dx_string_iterator);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
