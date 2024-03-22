// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#if !defined(IDLIB_VECTOR_2_H_INCLUDED)
#define IDLIB_VECTOR_2_H_INCLUDED

#include "scalar.h"
// NULL
#include <stddef.h>

/// @since 1.0
/// @brief A two component vector with elements of type idlib_f32.
typedef struct idlib_vector_2_f23 {
  idlib_f32 e[2];
} idlib_vector_2_f32;

/// @since 1.0
/// @brief Get the squared length of a idlib_vector_2_f32 object.
/// @param operand A pointer to the idlib_vector_2_f32 object of which the squared length is computed.
/// @return The squared length of the idlib_Vector_2_f32 object pointed to by @a operand.
static inline idlib_f32
idlib_vector_2_f32_get_squared_length
  (
    idlib_vector_2_f32 const* operand
  );

/// @since 1.0
/// @brief Get the length of a idlib_vector_2_f32 object.
/// @param operand Pointer to the idlib_vector_2_f32 object of which the length is computed.
/// @return The length of the idlib_Vector_2_f32 object pointed to by @a operand.
static inline idlib_f32
idlib_vector_2_f32_get_length
  (
    idlib_vector_2_f32 const* operand
  );

/// @since 1.0
/// @brief Get the normalized vector for a vector.
/// @param target Pointer to the idlib_vector_2_f32 object to assign the result to.
/// @param operand Pointer to the idlib_vector_2_f32 object of which the normalized vector is computed.
/// @return @a false if the vector represented by @a operand, @a true otherwise.
/// If @a false is returned, then *target was assigned a copy of @a operand.
/// @remarks
/// "represented by @a operand" actually means "represented by the object pointed to by @a operand".
static inline bool
idlib_vector_2_f32_normalize
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand
  );

/// @since 1.0
/// @brief Negate a vector.
/// @param target Pointer to the idlib_vector_2_f32 object to assign the result to.
/// @param operand Pointer to the idlib_vector_2_f32 object to negate.
/// @remarks @a target and @a operand all may refer to the same idlib_vector_2_f32 object.
static inline void
idlib_vector_2_f32_negate
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand
  );

/// @since 1.0
/// Assign an idlib_vector_2_f32 object the specified scalar values.
/// @param target A pointer to the idlib_vector_2_f32 object to assign the vector <code>(x,y)</code> to.
/// @param x, y The scalar values.
static inline void
idlib_vector_2_f32_set
  (
    idlib_vector_2_f32* target,
    idlib_f32 x,
    idlib_f32 y
  );

/// @since 1.0
/// Assign an idlib_vector_2_f32 object the values of an zero vector.
/// @param target A pointer to the idlib_vector_2_f32 object to assign the vector <code>(x,y)</code> to.
static inline void
idlib_vector_2_f32_set_zero
  (
    idlib_vector_2_f32* target
  );

/// @since 1.0
/// Compute the sum of two idlib_vector_2_f32 objects and assign the result to a idlib_vector_2_f32 object.
/// @param target Pointer to the idlib_vector_2_f32 object to assign the result to.
/// @param operand1 The idlib_vector_2_f32 object that is the augend (aka first term).
/// @param operand2 The idlib_vector_2_f32 object that is the addend (aka second term).
/// @remarks @a target, @a operand1, and @a operand2 all may refer to the same idlib_vector_2_f32 object.
static inline void
idlib_vector_2_f32_add
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  );

/// @since 1.0
/// Compute the difference of two idlib_vector_2_f32 objects and assign the result to a idlib_vector_2_f32 object.
/// @param target Pointer to the idlib_vector_2_f32 object to assign the result to.
/// @param operand1 The idlib_vector_2_f32 object that is the minuend (aka first term).
/// @param operand2 The idlib_vector_2_f32 object that is the subtrahend (aka second term).
/// @remarks @a target, @a operand1, and @a operand2 all may refer to the same idlib_vector_2_f32 object.
static inline void
idlib_vector_2_f32_subtract
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  );

/// @since 1.0
/// Get if two idlib_vector_2_f32 objects are equal.
/// @param operand1 The first operand.
/// @param operand2 The second operand.
/// @return @a true if the idlib_f32 objects are equal. @a false otherwise.
static inline bool
idlib_vector_2_f32_are_equal
  (
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  );

/// @since 1.0
/// Linear interpolation between two vectors.
/// @param target Pointer to the idlib_vector_2_f32 object to assign the result to.
/// @param operand1 Pointer to an idlib_vector_2_f32 object that is the start of the interpolation.
/// @param operand2 Pointer to an idlib_vector_2_f32 object that is the end of the interpolation.
/// @param operand3 idlib_f32 value, the interpolation factor.
/// @remarks
/// The interpolation factor is clamped to [0,1].
/// Then the result is computed by operand1 * (1 - t) + operand2 * t.
static inline void
idlib_vector_2_f32_lerp
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2,
    idlib_f32 operand3
  );

static inline idlib_f32
idlib_vector_2_f32_get_squared_length
  (
    idlib_vector_2_f32 const* operand
  )
{
  IDLIB_DEBUG_ASSERT(NULL != operand);
  idlib_f32 length_squared = operand->e[0] * operand->e[0]
                           + operand->e[1] * operand->e[1];
  return length_squared;
}

static inline idlib_f32
idlib_vector_2_f32_get_length
  (
    idlib_vector_2_f32 const* operand
  )
{ return idlib_sqrt_f32(idlib_vector_2_f32_get_squared_length(operand)); }

static inline bool
idlib_vector_2_f32_normalize
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand
  )
{
  idlib_f32 sql = idlib_vector_2_f32_get_squared_length(operand);
  if (sql == 0.f) {
    target->e[0] = 0.f;
    target->e[1] = 0.f;
    return false;
  } else {
    idlib_f32 l = idlib_sqrt_f32(sql);
    target->e[0] = operand->e[0] / l;
    target->e[1] = operand->e[1] / l;
    return true;
  }
}

static inline void
idlib_vector_2_f32_negate
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand
  )
{
  idlib_vector_2_f32_set(target, -operand->e[0], -operand->e[1]);
}

static inline void
idlib_vector_2_f32_set
  (
    idlib_vector_2_f32* target,
    idlib_f32 x,
    idlib_f32 y
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);

  target->e[0] = x;
  target->e[1] = y;
}

static inline void
idlib_vector_2_f32_set_zero
  (
    idlib_vector_2_f32* target
  )
{ idlib_vector_2_f32_set(target, 0.f, 0.f); }

static inline void
idlib_vector_2_f32_add
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);
  target->e[0] = operand1->e[0] + operand2->e[0];
  target->e[1] = operand1->e[1] + operand2->e[1];
}

static inline void
idlib_vector_2_f32_subtract
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);
  target->e[0] = operand1->e[0] - operand2->e[0];
  target->e[1] = operand1->e[1] - operand2->e[1];
}

static inline bool
idlib_vector_2_f32_are_equal
  (
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2
  )
{
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);
  if (operand1 == operand2) {
    return true;
  }
  return operand1->e[0] == operand2->e[0]
      && operand1->e[1] == operand2->e[1];
}

static inline void
idlib_vector_2_f32_lerp
  (
    idlib_vector_2_f32* target,
    idlib_vector_2_f32 const* operand1,
    idlib_vector_2_f32 const* operand2,
    idlib_f32 operand3
  )
{
  IDLIB_DEBUG_ASSERT(NULL != target);
  IDLIB_DEBUG_ASSERT(NULL != operand1);
  IDLIB_DEBUG_ASSERT(NULL != operand2);
  idlib_f32 t = idlib_clamp_f32(operand3);
  if (t == 0.f) {
    *target = *operand1;
  } else if (t == 1.f) {
    *target = *operand2;
  } else {
    target->e[0] = (1.f - t) * operand1->e[0] + t * operand2->e[0];
    target->e[1] = (1.f - t) * operand1->e[1] + t * operand2->e[1];
  }
}

#endif // IDLIB_VECTOR_2_H_INCLUDED
