/// @file Runtime/Map.h
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#if !defined(MACHINE_RUNTIME_MAP_H_INCLUDED)
#define MACHINE_RUNTIME_MAP_H_INCLUDED

#if !defined(MACHINE_RUNTIME_PRIVATE)
#error("Do not include this file directly, include `_Runtime.h` instead.")
#endif

#include "./Runtime/Collection.h"
typedef struct Machine_List Machine_List;

/// @brief A map.
MACHINE_DECLARE_CLASSTYPE(Machine_Map)

struct Machine_Map {
  Machine_Collection parent;
  void* pimpl;
  void (*set)(Machine_Map* self, Machine_Value key, Machine_Value value);
  Machine_Value(*get)(const Machine_Map* self, Machine_Value key);
};

/// @brief Construct this map.
/// @param self This map.
/// @param numberOfArguments The number of arguments.
/// @param arguments The arguments.
void Machine_Map_construct(Machine_Map* self, size_t numberOfArguments, const Machine_Value* arguments);

/// @brief Create an empty map.
/// @return The map.
Machine_Map* Machine_Map_create();

/// @brief Insert this map.
/// Performs <code>o[k] = v</code> where <code>o</code> is this map, <code>k</code> is the key, and <code>v</code> is the value.
/// If v = nil, then o[k] = nil.
/// @param self This map.
/// @param key The key.
/// @param value The value.
/// - If a pair with an equivalent key exists in this map, that existing pair is removed.
/// - The specified pair is added to this map.
/// - A pair with a value <code>void</code> is not considered as part of the map.
void Machine_Map_set(Machine_Map* self, Machine_Value key, Machine_Value value);

/// @brief Get the value for a key.
/// @param self This map.
/// @param key The key.
/// @return The value for the key if any. If no value for that key exists, the void value is returned.
Machine_Value Machine_Map_get(const Machine_Map* self, Machine_Value key);

/// @brief Convert the specified map to a list of key value pairs.
/// @param self This map.
/// @return A Machine_List of Machine_Pair values. 
Machine_List* Machine_Map_toList(const Machine_Map* self);

#endif // MACHINE_RUNTIME_MAP_H_INCLUDED
