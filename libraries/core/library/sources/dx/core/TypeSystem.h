#if !defined(CORE_TYPESYSTEM_H_INCLUDED)
#define CORE_TYPESYSTEM_H_INCLUDED

#include "Core/FundamentalTypes.h"
/*Forward declaration.*/
typedef struct Core_Object Core_Object;
/*Forward declaration.*/
typedef struct Core_Object_Dispatch Core_Object_Dispatch;


/// @brief
/// The opaque C representation of runtime information on a type.
typedef struct Core_Type Core_Type;

/// @brief Get if a type is a lower than or equal to another type.
/// @param x The first type.
/// @param y The second type.
/// @return
/// @a true if the first type is lower than or equal to the second type.
/// @a false otherwise.
/// 
/// @undefined @a x does not pointed to a dx_type object.
/// @undefined The runtime type system is not initialized.
Core_Result Core_Type_isLowerThanOrEqualTo(Core_Boolean* RETURN, Core_Type* x, Core_Type* y);

/// @brief Get a pointer to the dispatch of an object type.
/// @param x The type.
/// @return A pointer to the dispatch.
/// 
/// @undefined @a x does not pointed to a dx_type object.
/// @undefined The runtime type system is not initialized.
Core_Result Core_Type_getDispatch(void** RETURN, Core_Type* x);

/// @brief Get the parent type of a type.
/// @param RETURN A pointer to a <code>Core_Type*</code> variable.
/// @param SELF A pointer to this type.
/// 
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
/// 
/// @success
/// <code>*RETURN</code>
/// - was assigned a pointer to the Core_Type object of the parent type (if such a parent type exists) or
/// - was assigned the null pointer (if no such parent type exists)
///
/// @undefined @a SELF does not point to a <code>Core_Type</code> object.
/// @undefined @a RETURN does not point to a <code>Core_Type*</code> variable.
/// @undefined The runtime type system is not initialized.
Core_Result Core_Type_getParent(Core_Type** RETURN, Core_Type* SELF);

/// @brief Get the name of the type.
/// @param SELF A pointer to the type.
/// @param p A pointer to a <code>char const*</code> variable.
/// @param n A pointer to a <code>Core_Size</code> variable.
/// 
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable on failure.
/// 
/// @success
/// <code>*p</code> was assigned a pointer to an UTF-8 string of <code>*n</code> Bytes.
/// The Bytes' lifetime is the lifetime of the passed type object.
/// <code>*n</code> was assigned the number of Bytes in the UTF-8 string assigned to <code>*p</code>.
/// 
/// @undefined @a SELF does not pointed to a <code>Core_Type</code> object.
/// @undefined @a p does not point to a <code>char const*</code> variable.
/// @undefined @a n does not point to a <code>Core_Size</code> variable.
Core_Result Core_Type_getName(Core_Type* SELF, char const** p, Core_Size* n);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Get if a type is an enumeration type.
/// @param RETURN
/// A pointer to a <code>Core_Boolean</code> variable.
/// @param type
/// A pointer to the type.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @success
/// <code>*RETURN</code> was assigned @a Core_Boolean if the type pointed to by @a type is an enumeration type.
/// Otherwise it was assigned @a Core_False.
/// @error Core_Error_ArgumentInvalid
/// @a type is null pointer.
/// @undefined
/// The runtime type system is not initialized.
Core_Result Core_TypeSystem_isEnumerationType(Core_Boolean* RETURN, Core_Type* type);

/// @brief Used to register enumeration types.
/// 
/// @param [out] RETURN A pointer to a <code>Core_Type*</code> variable.
/// 
/// @param p, n An UTF-8 string. Must be a valid type name.
/// 
/// @return #Core_Success on success. #Core_Failure on failure.
/// @remarks This function sets the by-thread error variable.
/// @success <code>*RETURN</code> was assigned a pointer to the type.
/// 
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// 
/// @error #Core_Error_AllocationFailed An allocation failed.
/// @error #Core_Error_Exists A type of the same name already exists.
Core_Result Core_TypeSystem_defineEnumerationType(Core_Type** RETURN, char const* p, Core_Size n, void(*onTypeDestroyed)());

#define Core_declareEnumerationType(NAME, C_NAME) \
  Core_Result C_NAME##_getType(Core_Type** RETURN); \
  typedef enum C_NAME C_NAME;

#define Core_defineEnumerationType(NAME, CNAME) \
  static Core_Type* _##CNAME##_type = NULL; \
\
  static void CNAME##_onTypeDestroyed() { \
    _##CNAME##_type = NULL; \
  } \
\
  Core_Result CNAME##_getType(Core_Type** RETURN) { \
    if (!RETURN) { \
      Core_setError(Core_Error_ArgumentInvalid); \
      return Core_Failure; \
    } \
    if (!_##CNAME##_type) { \
      if (Core_TypeSystem_defineEnumerationType(&_##CNAME##_type, NAME, sizeof(NAME) - 1, &CNAME##_onTypeDestroyed)) { \
        return Core_Failure; \
      } \
    } \
    *RETURN = _##CNAME##_type; \
    return Core_Success; \
  }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Get if a type is an fundamental type.
/// @param RETURN
/// A pointer to a <code>Core_Boolean</code> variable.
/// @param type
/// A pointer to the type.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @success
/// <code>*RETURN</code> was assigned @a Core_Boolean if the type pointed to by @a type is an fundamental type.
/// Otherwise it was assigned @a Core_False.
/// @error Core_Error_ArgumentInvalid
/// @a type is null pointer.
/// @undefined
/// The runtime type system is not initialized.
Core_Result Core_TypeSystem_isFundamentalType(Core_Boolean* RETURN, Core_Type* type);

/// @brief Used to register types like Core_(Integer|Natural)(8|16|32|64), Core_Real(32|64), Core_Size, and Core_Boolean.
/// 
/// @param [out] RETURN A pointer to a <code>Core_Type*</code> variable.
/// 
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param value_size The size of a value of that type.
/// 
/// @return #Core_Success on success. #Core_Failure on failure.
/// @success <code>*RETURN</code> was assigned a poiinter to the type.
/// 
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// 
/// @error #Core_Error_AllocationFailed an allocation failed
/// @error #Core_Error_Exists a type of the same name already exists
Core_Result Core_TypeSystem_defineFundamentalType(Core_Type** RETURN, char const* p, Core_Size n, void(*onTypeDestroyed)(), Core_Size value_size);

#define Core_declareFundamentalType(NAME, CNAME) \
  Core_Result CNAME##_getType(Core_Type** RETURN); \

#define Core_defineFundamentalType(NAME, CNAME) \
  static Core_Type* _##CNAME##_type = NULL; \
\
  static void CNAME##_onTypeDestroyed() { \
    _##CNAME##_type = NULL; \
  } \
\
  Core_Result CNAME##_getType(Core_Type** RETURN) { \
    if (!RETURN) { \
      Core_setError(Core_Error_ArgumentInvalid); \
      return Core_Failure; \
    } \
    if (!_##CNAME##_type) { \
      if (Core_TypeSystem_defineFundamentalType(&_##CNAME##_type, NAME, sizeof(NAME) - 1, &CNAME##_onTypeDestroyed, sizeof(CNAME) - 1)) { \
        return Core_Failure; \
      } \
    } \
    *RETURN = _##CNAME##_type; \
    return Core_Success; \
  }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// Get if a type is an object type.
/// @param RETURN
/// A pointer to a <code>Core_Boolean</code> variable.
/// @param type
/// A pointer to the type.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @success
/// <code>*RETURN</code> was assigned @a Core_Boolean if the type pointed to by @a type is an object type.
/// Otherwise it was assigned @a Core_False.
/// @error Core_Error_ArgumentInvalid
/// @a type is null pointer.
/// @undefined
/// The runtime type system is not initialized.
Core_Result Core_TypeSystem_isObjectType(Core_Boolean* RETURN, Core_Type* type);

/// @brief Used to register object types.
/// 
/// @param [out] RETURN A pointer to a <code>Core_Type*</code> variable.
/// 
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param parent A pointer to the parent type or a null pointer.
/// @param object_size The object size.
/// @param object_destruct A pointer to the object destructor or a null pointer.
/// @param dispatch_size The dispatch size.
/// @param dispatch_construct A pointer to the dispatch constructor or a null pointer.
/// 
/// @return #Core_Success on success. #Core_Failure on failure.
/// @success <code>*RETURN</code> was assigned a poiinter to the type.
///
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// 
/// @error #Core_Error_AllocationFailed An allocation failed.
/// @error #Core_Error_Exists A type of the same name already exists.
Core_Result Core_TypeSystem_defineObjectType(Core_Type** RETURN, char const* p, Core_Size n, void (*onTypeDestroyed)(), Core_Type* parent,
                                             Core_Size object_size, void (*object_destruct)(Core_Object*), \
                                             Core_Size dispatch_size, void (*dispatch_construct)(Core_Object_Dispatch*));

#define Core_declareObjectType(NAME, C_NAME, C_PARENT_NAME) \
  Core_Result C_NAME##_getType(Core_Type** RETURN); \
  typedef struct C_NAME##_Dispatch C_NAME##_Dispatch; \
  typedef struct C_NAME C_NAME;

#define Core_defineObjectType(NAME, C_NAME, C_PARENT_NAME) \
  static Core_Type* _##C_NAME##_type = NULL; \
\
  static void C_NAME##_destruct(C_NAME* SELF); \
\
  static void C_NAME##_constructDispatch(C_NAME##_Dispatch* SELF); \
\
  static void C_NAME##_onTypeDestroyed() { \
    _##C_NAME##_type = NULL; \
  } \
\
  Core_Result C_NAME##_getType(Core_Type** RETURN) { \
    if (!_##C_NAME##_type) { \
      Core_Type* parent = NULL; \
      if (C_PARENT_NAME##_getType(&parent)) { \
        return Core_Failure; \
      } \
      if (Core_TypeSystem_defineObjectType(&_##C_NAME##_type, \
                                           NAME, sizeof(NAME) - 1, \
                                           &C_NAME##_onTypeDestroyed, \
                                           parent, \
                                           sizeof(C_NAME), (void(*)(Core_Object*))&C_NAME##_destruct, \
                                           sizeof(C_NAME##_Dispatch), (void(*)(Core_Object_Dispatch*))&C_NAME##_constructDispatch)) { \
        return Core_Failure; \
      } \
    } \
    *RETURN = _##C_NAME##_type; \
    return Core_Success; \
  }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareFundamentalType
  (
    "Core.Natural8",
    Core_Natural8
  );

Core_declareFundamentalType
  (
    "Core.Natural16",
    Core_Natural16
  );

Core_declareFundamentalType
  (
    "Core.Natural32",
    Core_Natural32
  );

Core_declareFundamentalType
  (
    "Core.Natural64",
    Core_Natural64
  );



Core_declareFundamentalType
  (
    "Core.Integer8",
    Core_Integer8
  );

Core_declareFundamentalType
  (
    "Core.Integer16",
    Core_Integer16
  );

Core_declareFundamentalType
  (
    "Core.Integer32",
    Core_Integer32
  );

Core_declareFundamentalType
  (
    "Core.Integer64",
    Core_Integer64
  );



Core_declareFundamentalType
  (
    "Core.Real32",
    Core_Real32
  );

Core_declareFundamentalType
  (
    "Core.Real64",
    Core_Real64
  );



Core_declareFundamentalType
  (
    "Core.Boolean",
    Core_Boolean
  );



Core_declareFundamentalType
  (
    "Core.Size",
    Core_Size
  );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_TYPESYSTEM_H_INCLUDED
