/// @file dx/core/math.h
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/configuration.h"
#include "dx/core/core.h"
#include "Core/Numerics.h"
#include "idlib.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;
typedef struct Core_InlineRgbR32 Core_InlineRgbR32;
typedef struct Core_InlineRgbaR32 Core_InlineRgbaR32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineRgbR32 {
  Core_Real32 r;
  Core_Real32 g;
  Core_Real32 b;
} Core_InlineRgbR32;

static inline void Core_InlineRgbR32_set(Core_InlineRgbR32* c, Core_Real32 r, Core_Real32 g, Core_Real32 b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

static inline Core_Boolean Core_InlineRgbR32_areEqual(Core_InlineRgbR32 const* v, Core_InlineRgbR32 const* w) {
  return v->r == w->r
      && v->g == w->g
      && v->b == w->b;
}

void dx_rgb_f32_lerp(Core_InlineRgbR32 const* a, Core_InlineRgbR32 const* b, Core_Real32 t, Core_InlineRgbR32* c);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineRgbaR32 {
  Core_Real32 r;
  Core_Real32 g;
  Core_Real32 b;
  Core_Real32 a;
} Core_InlineRgbaR32;

static inline void Core_InlineRgbaR32_set(Core_InlineRgbaR32* c, Core_Real32 r, Core_Real32 g, Core_Real32 b, Core_Real32 a) {
  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;
}

static inline Core_Boolean Core_InlineRgbaR32_areEqual(Core_InlineRgbaR32 const* v, Core_InlineRgbaR32 const* w) {
  return v->r == w->r
      && v->g == w->g
      && v->b == w->b
      && v->a == w->a;
}

void dx_rgba_f32_lerp(Core_InlineRgbaR32 const* a, Core_InlineRgbaR32 const* b, Core_Real32 t, Core_InlineRgbaR32* c);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineAbgrN8 {
  Core_Natural8 a;
  Core_Natural8 b;
  Core_Natural8 g;
  Core_Natural8 r;
} Core_InlineAbgrN8;

static inline void Core_InlineAbgrN8_set(Core_InlineAbgrN8* c, Core_Natural8 a, Core_Natural8 b, Core_Natural8 g, Core_Natural8 r) {
  c->a = a;
  c->b = b;
  c->g = g;
  c->r = a;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineAlN8 {
  Core_Natural8 a;
  Core_Natural8 l;
} Core_InlineAlN8;

static inline void Core_InlineAlN8_set(Core_InlineAlN8* c, Core_Natural8 a, Core_Natural8 l) {
  c->a = a;
  c->l = l;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineArgbN8 {
  Core_Natural8 a;
  Core_Natural8 r;
  Core_Natural8 g;
  Core_Natural8 b;
} Core_InlineArgbN8;

static inline void Core_InlineArgbN8_set(Core_InlineArgbN8* c, Core_Natural8 a, Core_Natural8 r, Core_Natural8 g, Core_Natural8 b) {
  c->a = a;
  c->r = r;
  c->g = g;
  c->b = b;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineBgrN8 {
  Core_Natural8 b;
  Core_Natural8 g;
  Core_Natural8 r;
} Core_InlineBgrN8;

static inline void Core_InlineBgrN8_set(Core_InlineBgrN8* c, Core_Natural8 b, Core_Natural8 g, Core_Natural8 r) {
  c->b = b;
  c->g = g;
  c->r = r;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineBgraN8 {
  Core_Natural8 b;
  Core_Natural8 g;
  Core_Natural8 r;
  Core_Natural8 a;
} Core_InlineBgraN8;

static inline void Core_InlineBgraN8_set(Core_InlineBgraN8* c, Core_Natural8 b, Core_Natural8 g, Core_Natural8 r, Core_Natural8 a) {
  c->b = b;
  c->g = g;
  c->r = r;
  c->a = a;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineLN8 {
  Core_Natural8 l;
} Core_InlineLN8;

static inline void Core_InlineLN8_set(Core_InlineLN8* c, Core_Natural8 l) {
  c->l = l;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineLaN8 {
  Core_Natural8 l;
  Core_Natural8 a;
} Core_InlineLaN8;

static inline void Core_InlineLaN8_set(Core_InlineLaN8* c, Core_Natural8 l, Core_Natural8 a) {
  c->l = l;
  c->a = a;
}

static inline Core_Boolean Core_InlineLaN8_areEqual(Core_InlineLaN8 const* a, Core_InlineLaN8 const* b) {
  return a->l == b->l
      && a->a == b->a;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineRgbN8 {
  Core_Natural8 r;
  Core_Natural8 g;
  Core_Natural8 b;
} Core_InlineRgbN8;

static inline void Core_InlineRgbN8_set(Core_InlineRgbN8* c, Core_Natural8 r, Core_Natural8 g, Core_Natural8 b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

static inline Core_Boolean Core_InlineRgbN8_areEqual(Core_InlineRgbN8 const* a, Core_InlineRgbN8 const* b) {
  return a->r == b->r
      && a->g == b->g
      && a->b == b->b;
}

static inline void dx_rgb_n8_to_rgba_f32(Core_InlineRgbN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = a;
}

void dx_rgb_u8_lerp(Core_InlineRgbN8 const* a, Core_InlineRgbN8 const* b, Core_Real32 t, Core_InlineRgbN8* c);

// The color "Capri"  (0, 191, 255).
extern Core_InlineRgbN8 const dx_colors_capri;

// The color "Malachite" (0, 255, 64) from "Capri"'s tetradic palette.
extern Core_InlineRgbN8 const dx_colors_malachite;

// The color "Amber" (255, 192, 0) from "Capri"'s Split Complementary palette.
extern Core_InlineRgbN8 const dx_colors_amber;

// The color "White" (255, 255, 255).
extern Core_InlineRgbN8 const dx_colors_white;

// The color "Gray" (128, 128, 128).
extern Core_InlineRgbN8 const dx_colors_gray;

// The color "Black" (0, 0, 0).
extern Core_InlineRgbN8 const dx_colors_black;

// The color "Red" (255, 0, 0).
extern Core_InlineRgbN8 const dx_colors_red;

// The color "Green" (0, 255, 0).
extern Core_InlineRgbN8 const dx_colors_green;

// The color "Blue" (0, 0, 255).
extern Core_InlineRgbN8 const dx_colors_blue;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineRgbaN8 {
  Core_Natural8 r;
  Core_Natural8 g;
  Core_Natural8 b;
  Core_Natural8 a;
} Core_InlineRgbaN8;

static inline void Core_InlineRgbaN8_set(Core_InlineRgbaN8* c, Core_Natural8 r, Core_Natural8 g, Core_Natural8 b, Core_Natural8 a) {
  c->r = r;
  c->g = g;
  c->b = b;
  c->a = a;
}

static inline Core_Boolean Core_InlineRgbaN8_areEqual(Core_InlineRgbaN8 const* a, Core_InlineRgbaN8 const* b) {
  return a->r == b->r
      && a->g == b->g
      && a->b == b->b
      && a->a == b->a; 
}

static inline void Core_InlineRgbaN8_to_rgba_f32(Core_InlineRgbaN8 const* source, Core_Real32 a, Core_InlineRgbaR32* target) {
  target->r = ((Core_Real32)source->r) / 255.f;
  target->g = ((Core_Real32)source->g) / 255.f;
  target->b = ((Core_Real32)source->b) / 255.f;
  target->a = ((Core_Real32)source->a) / 255.f;;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// @todo Use idlib.
// Symbolic constant for the Core_Real32 representation of PI.
#define DX_PI_F32 3.1415926f

// @todo Use idlib.
// @brief Convert an angle in degrees into the corresponding angle in radians.
// @param a The angle in degrees.
// @return The corresponding angle in radians.
static inline Core_Real32 dx_deg_to_rad(Core_Real32 x) {
  return idlib_deg_to_rad_f32(x);
}

// @brief Clamp a value to the range [0,1].
// @param x The value to be clamped.
// @return x clamped.
static inline Core_Real32 dx_clamp(Core_Real32 x) {
  return idlib_clamp_f32(x);
}

static inline void dx_lerp(Core_Real32 *result, Core_Real32 start, Core_Real32 end, Core_Real32 t) {
  t = dx_clamp(t);
  if (t == 0.f) {
    *result = start;
  } else if (t == 1.f) {
    *result = end;
  } else {
    *result = (1.f - t) * start + t * end;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Core_InlineVector2R32 {
  idlib_vector_2_f32 v;
} Core_InlineVector2R32;

/// @todo Use idlib.
static inline void dx_vec2_f32_set(Core_InlineVector2R32* v, Core_Real32 x, Core_Real32 y) {
  idlib_vector_2_f32_set(&v->v, x, y);
}

/// @todo Use idlib.
/// @ingroup math
/// @brief Compute the sum of two vectors.
/// @param w Pointer to a DX_VEC2 object.
/// @param u Pointer to a DX_VEC2 object.
/// The object's values represent the augend (aka the 1st operand).
/// @param v Pointer toa DX_VEC2 object.
/// The object's values represent the addend (aka the 2nd operand).
/// @remarks @a RETURN, @a operand1, and @a operand2 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the product.
static inline void Core_InlineVector2R32_add_vv(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_InlineVector2R32 const* operand2) {
  idlib_vector_2_f32_add(&RETURN->v, &operand1->v, &operand2->v);
}

/// @ingroup math
/// @brief Compute the product of a vector and a scalar.
/// @param RETURN A pointer to a <code>Core_InlineVector2R32</code> variable.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The first operand aka the multiplier.
/// @param operand2 A <code>Core_Real32</code> value.
/// The second operand aka the multiplicand.
/// @remarks @a RETURN and @a operand1 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the product.
void Core_InlineVector2R32_mul_vs(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_Real32 operand2);

/// @ingroup math
/// @brief Compute the component-wise product of two vector.
/// @param RETURN A pointer to a <code>Core_InlineVector2R32</code> variable.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The first operand aka the multiplier.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The second operand aka the multiplicand.
/// @remarks @a RETURN, @a operand1, and @a operand2 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the product.
void Core_InlineVector2R32_mulc_vv(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_InlineVector2R32 const* operand2);

/// @todo Use idlib.
/// @ingroup math
/// @brief Compute the difference of two vectors.
/// @param RETURN A pointer to a <code>Core_InlineVector2R32</code> variable.
/// @param operand1 A pointer to a <code>Core_InlineVector2R32</code> variable.
/// The first operand aka the minuend.
/// @param operand2 Pointer to a <code>Core_InlineVector2R32</code> variable.
/// The second operand aka the subtrahend.
/// @remarks @a RETURN, @a operand1, and @a operand2 all may refer to the same object.
/// @post <code>*RETURN/<code> was assigned the difference.
static inline void Core_InlineVector2R32_sub_vv(Core_InlineVector2R32* RETURN, Core_InlineVector2R32 const* operand1, Core_InlineVector2R32 const* operand2) {
  idlib_vector_2_f32_subtract(&RETURN->v, &operand1->v, &operand2->v);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC3 {
  idlib_vector_3_f32 v;
} DX_VEC3;

static inline void dx_vec3_set(DX_VEC3* v, Core_Real32 x, Core_Real32 y, Core_Real32 z) {
  idlib_vector_3_f32_set(&v->v, x, y, z);
}

static inline Core_Boolean dx_vec3_are_equal(DX_VEC3 const* u, DX_VEC3 const* v) {
  return idlib_vector_3_f32_are_equal(&u->v, &v->v);
}

/// @ingroup math
/// Get the squared length of a vector
/// @param v the vector
/// @return the squared length of the vector
static inline Core_Real32 dx_vec3_sql(DX_VEC3 const* v) {
  return idlib_vector_3_f32_get_squared_length(&v->v);
}

// Compute a normalized vector.
// @param v the vector receiving the normalized vector
// @param u the vector to normalize 
// @remarks The result is the normalized vector of the vector to be normalized if the vector to be normalized is a non-zero vector.
// Otherwise the result is the zero vector.
// @a v and @a u may refer to the same object.
void dx_vec3_norm(DX_VEC3* v, DX_VEC3 const* u);

/// @ingroup math
/// @brief Compute the sum of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the augend (aka the 1st operand).
/// @param v Pointer toa DX_VEC3 object.
/// The object's values represent the addend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the sum vector.
static inline void dx_vec3_add3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  idlib_vector_3_f32_add(&w->v, &u->v, &v->v);
}

/// @ingroup math
/// @brief Compute the difference of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector that is the minuend (aka the 1st operand).
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent hte vector that is the the subtrahend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the difference vector.
static inline void dx_vec3_sub3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v) {
  idlib_vector_3_f32_subtract(&w->v, &u->v, &v->v);
}

/// @ingroup math
/// @brief Compute the cross product of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 1st operand.
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent the vector, that is the 2nd operand.
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the cross product vector.
void dx_vec3_cross(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

static inline void dx_vec3_lerp(DX_VEC3 *w, DX_VEC3 const* u, DX_VEC3 const* v, Core_Real32 t) {
  idlib_vector_3_f32_lerp(&w->v, &u->v, &v->v, t);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC4 {
  Core_Real32 e[4];
} DX_VEC4;

static inline void dx_vec4_set(DX_VEC4* v, Core_Real32 x, Core_Real32 y, Core_Real32 z, Core_Real32 w) {
  v->e[0] = x;
  v->e[1] = y;
  v->e[2] = z;
  v->e[3] = w;
}

static inline Core_Boolean dx_vec4_are_equal(DX_VEC4 const* u, DX_VEC4 const* v) {
  return u->e[0] == v->e[0]
      && u->e[1] == v->e[1]
      && u->e[2] == v->e[2]
      && u->e[3] == v->e[3];
}

/// @ingroup math
/// @brief Compute the sum of two vectors.
/// @param w Pointer to a DX_VEC4 object.
/// @param u Pointer to a DX_VEC4 object.
/// The object's values represent the augend (aka the 1st operand).
/// @param v Pointer toa DX_VEC4 object.
/// The object's values represent the addend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the sum vector.
void dx_vec4_add4(DX_VEC4* w, DX_VEC4 const* u, DX_VEC4 const* v);

/// @ingroup math
/// @brief Compute the difference of two vectors.
/// @param w Pointer to a DX_VEC4 object.
/// @param u Pointer to a DX_VEC4 object.
/// The object's values represent the vector that is the minuend (aka the 1st operand).
/// @param v Pointer to a DX_VEC4 object.
/// The object's values represent hte vector that is the the subtrahend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the difference vector.
void dx_vec4_sub4(DX_VEC4* w, DX_VEC4 const* u, DX_VEC4 const* v);

static inline void dx_vec4_lerp(DX_VEC4 const* u, DX_VEC4 const* v, Core_Real32 t, DX_VEC4* w) {
  t = dx_clamp(t);
  if (t == 0.f) {
    *w = *u;
  } else if (t == 1.f) {
    *w = *v;
  } else {
    w->e[0] = (1.f - t) * u->e[0] + t * v->e[0];
    w->e[1] = (1.f - t) * u->e[1] + t * v->e[1];
    w->e[2] = (1.f - t) * u->e[2] + t * v->e[2];
    w->e[3] = (1.f - t) * u->e[3] + t * v->e[3];
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// A 4x4 Core_Real32 matrix POD.
typedef struct DX_MAT4 {
  // index i,j denotes i-th row and j-th colmn
  Core_Real32 e[4][4];
} DX_MAT4;

// assign this matrix the values of an identity matrix
void dx_mat4_set_identity(DX_MAT4* m);

// assign this matrix the values of a translation matrix
void dx_mat4_set_translate(DX_MAT4* m, Core_Real32 x, Core_Real32 y, Core_Real32 z);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the x-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_x(DX_MAT4* a, Core_Real32 x);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the y-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_y(DX_MAT4* a, Core_Real32 x);

/// Assign a DX_MAT4 object the values of an orthographic projection matrix such that
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
void dx_mat4_set_ortho(DX_MAT4* m, Core_Real32 left, Core_Real32 right, Core_Real32 bottom, Core_Real32 top, Core_Real32 near, Core_Real32 far);

/// @brief Assignt a DX_MAT4 object the values of a perspective project matrix such that
// - the positive z-axis points out of the screen (negative z-axis points into the screen)
// - the positive x-axis points to the right
// - the positive y-axis points to the top
/// @param field_of_view_y
/// The field of vision angle, in degrees, along the y-axis.
/// In other terms: The angle, in degrees, in between a plane passing through the camera position as well as the top of your screen and another plane passing through the camera position and the bottom of your screen. 
/// The bigger this angle is, the more you can see of the world - but at the same time, the objects you can see will become smaller.
/// @param aspect_ratio The aspect ratio, that is, the ratio of the width to the height.
/// An aspect ratio of x means that the width is x times the height.
/// @param near The distance of the near clipping plane.
/// @param far The distance of the far clipping plane.
/// @remarks
/// This function creates the following matrix
/// \f[
/// \begin{matrix}
/// \frac{f}{aspectRatio} & 0 &                          0 &                                       0 \\
/// 0                     & f &                          0 &                                       0 \\
/// 0                     & 0 &  \frac{far+near}{near-far} & \frac{2 \cdot far \cdot near}{near-far} \\
/// 0                     & 0 &                         -1 &                                       0 \\
/// \end{matrix}
/// \f]
/// where
/// \f[
/// f = \cot\left(\frac{fieldOfVision}{2}\right)
/// \f]
void dx_mat4_set_perspective(DX_MAT4* m, Core_Real32 field_of_view_y, Core_Real32 aspect_ratio, Core_Real32 near, Core_Real32 far);

/// Compute the product of two matrices.
/// @param c Pointer to a DX_MAT4 object.
/// @param a Pointer to a DX_MAT4 object.
/// The object's values represent the matrix that is the the multiplier (aka the 1st factor aka the 1st operand)
/// @param b Pointer to a DX_MAT4 object.
/// The object's values represent the matrix that is the multiplicand (aka the 2nd factor aka the 2nd operand).
/// @remarks @a c, @a b, and @a b all may refer to the same object.
/// @post <code>*c/<code> was assigned the values of the product matrix.
void dx_mat4_mul3(DX_MAT4* c, DX_MAT4 const* a, DX_MAT4 const* b);

/// Assign this matrix the value a of a view matrix.
/// @param source the position vector of the point at which the viewer is located
/// @param target the position vector of the point at which the viewer is looking at
/// @param uo the direction vector indicating the upward direction of the viewer
/// @remarks
/// This function constructs a view matrix <code>V</code>given
/// - the position the viewer is located at <code>source</code>,
/// - the position the viewer is looking at <code>target</code>, and
/// - the vector indicating the up direction of the viewer <code>up</code>.
/// The view matrix <code>V</code> is constructed as follows
/// Let
/// @code
/// forward := norm(target - source)
/// right := forward x norm(up)
/// up' := right x forward
/// @endcode
/// Then the view matrix <code>V</code> is given by
/// @code
/// V :=
/// | right.x    | right.y    | right.z    | 0
/// | up'.x      | up'.y      | u'.z       | 0
/// | -forward.x | -forward.y | -forward.z | 0
/// | 0          | 0          | 0          | 1
/// @endcode
void dx_mat4_set_look_at(DX_MAT4* a, DX_VEC3 const* source, DX_VEC3 const* target, DX_VEC3 const* up);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup math
/// Transform a position vector.
/// @param u Pointer to a DX_VEC3 object. The object receives the transformed position vector.
/// @param v Pointer to a DX_VEC3 object. The object represents the position to transform.
/// @param m Pointer to a DX_MAT4 object. The object represents the transformation matrix.
void dx_transform_point(DX_VEC3* u, DX_VEC3 const* v, DX_MAT4 const* m);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An offset in 2D space.
typedef struct DX_OFFSET2_F32 {
  Core_Real32 x;
  Core_Real32 y;
} DX_OFFSET2_F32;

static inline void dx_offset2_f32_set(DX_OFFSET2_F32* a, Core_Real32 x, Core_Real32 y) {
  a->x = x;
  a->y = y;
}

/// @brief An extend in 2D space.
typedef struct DX_EXTEND2_F32 {
  Core_Real32 x;
  Core_Real32 y;
} DX_EXTEND2_F32;

static inline void dx_extend2_f32_set(DX_EXTEND2_F32* a, Core_Real32 x, Core_Real32 y) {
  a->x = x;
  a->y = y;
}

typedef struct DX_RECT2_F32 {
  /// @brief An offset in 2D.
  DX_OFFSET2_F32 offset;
  /// @brief An extend in 2D.
  DX_EXTEND2_F32 extend;
} DX_RECT2_F32;

static inline void dx_rect2_f32_set(DX_RECT2_F32* a, Core_Real32 left, Core_Real32 bottom, Core_Real32 right, Core_Real32 top) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, right - left, top - bottom);
}

static inline void dx_rect2_f32_set2(DX_RECT2_F32* a, Core_Real32 left, Core_Real32 bottom, Core_Real32 width, Core_Real32 height) {
  dx_offset2_f32_set(&a->offset, left, bottom);
  dx_extend2_f32_set(&a->extend, width, height);
}

static inline Core_Real32 dx_rect2_f32_get_left(DX_RECT2_F32 const* a) {
  if (a->extend.x < 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline Core_Real32 dx_rect2_f32_get_right(DX_RECT2_F32 const* a) {
  if (a->extend.x > 0.f) {
    return a->offset.x + a->extend.x;
  } else {
    return a->offset.x;
  }
}

static inline Core_Real32 dx_rect2_f32_get_bottom(DX_RECT2_F32 const* a) {
  if (a->extend.y < 0.f) {
    return a->offset.y + a->extend.y;
  } else {
    return a->offset.y;
  }
}

static inline Core_Real32 dx_rect2_f32_get_top(DX_RECT2_F32 const* a) {
  if (a->extend.y > 0.f) {
    return a->offset.y + a->extend.y;
  } else {
    return a->offset.y;
  }
}

static inline void dx_rect2_f32_union(DX_RECT2_F32* a, DX_RECT2_F32 const* x, DX_RECT2_F32 const* y) {
#pragma push_macro("MIN")
#pragma push_macro("MAX")
#define MIN(x,y) (x) < (y) ? (x) : (y)
#define MAX(x,y) (x) > (y) ? (x) : (y)

  Core_Real32 l = MIN(dx_rect2_f32_get_left(x), dx_rect2_f32_get_left(y));
  Core_Real32 b = MIN(dx_rect2_f32_get_bottom(x), dx_rect2_f32_get_bottom(y));
  Core_Real32 r = MAX(dx_rect2_f32_get_right(x), dx_rect2_f32_get_right(y));
  Core_Real32 t = MAX(dx_rect2_f32_get_top(x), dx_rect2_f32_get_top(y));

  a->offset.x = l;
  a->offset.y = b;
  a->extend.x = r - l;
  a->extend.y = t - b;

#undef MAX
#undef MIN
#pragma pop_macro("MAX")
#pragma pop_macro("MIN")
}

static inline void dx_rect2_f32_translate(DX_RECT2_F32* r, Core_InlineVector2R32 const* t) {
  r->offset.x += t->v.e[0];
  r->offset.y += t->v.e[1];
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
Core_Result dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
