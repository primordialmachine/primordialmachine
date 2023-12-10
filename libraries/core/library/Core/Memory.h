#if !defined(CORE_MEMORY_H_INCLUDED)
#define CORE_MEMORY_H_INCLUDED

#include "Core/FundamentalTypes.h"

// http://localhost/core#core-memory-copy
Core_Result Core_Memory_copy(void* p, void const* q, Core_Size n);

// http://localhost/core#core-memory-move
Core_Result Core_Memory_move(void* p, void const* q, Core_Size n);

// http://localhost/core#core-memory-zero
Core_Result Core_Memory_zero(void* p, Core_Size n);

// http://localhost/core#core-memory-fill
Core_Result Core_Memory_fill(void* p, Core_Size n, Core_Natural8 v);

// http://localhost/core#core-memory-allocate
Core_Result Core_Memory_allocate(void** RETURN, Core_Size n);

// http://localhost/core#core-memory-reallocate
Core_Result Core_Memory_reallocate(void** RETURN, void* p, Core_Size n);

// http://localhost/core#core-memory-deallocate
void Core_Memory_deallocate(void* p);

// http://localhost/core#core-memory-compare
Core_Result Core_Memory_compare(Core_Integer8* RETURN, void const* p, void const* q, Core_Size n);

#endif // CORE_MEMORY_H_INCLUDED
