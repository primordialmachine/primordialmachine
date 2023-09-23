/// @file dx/core/math.h
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_MATH_H_INCLUDED)
#define DX_MATH_H_INCLUDED

#include "dx/core/core.h"

typedef struct DX_VEC3 DX_VEC3;
typedef struct DX_VEC4 DX_VEC4;
typedef struct DX_RGBA_F32 DX_RGBA_F32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_RGBA_F32 {
  dx_f32 r;
  dx_f32 g;
  dx_f32 b;
  dx_f32 a;
} DX_RGBA_F32;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_BGR_N8 {
  dx_n8 b;
  dx_n8 g;
  dx_n8 r;
} DX_BGR_N8;

static inline void dx_bgr_n8_set(DX_BGR_N8* c, dx_n8 b, dx_n8 g, dx_n8 r) {
  c->b = b;
  c->g = g;
  c->r = r;
}

typedef struct DX_RGB_N8 {
  dx_n8 r;
  dx_n8 g;
  dx_n8 b;
} DX_RGB_N8;

static inline void dx_rgb_n8_to_rgba_f32(DX_RGB_N8 const* source, dx_f32 a, DX_RGBA_F32* target) {
  target->r = ((dx_f32)source->r) / 255.f;
  target->g = ((dx_f32)source->g) / 255.f;
  target->b = ((dx_f32)source->b) / 255.f;
  target->a = a;
}

static inline void dx_rgb_u8_set(DX_RGB_N8* c, dx_n8 r, dx_n8 g, dx_n8 b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

void dx_rgb_u8_lerp(DX_RGB_N8 const* a, DX_RGB_N8 const* b, dx_f32 t, DX_RGB_N8* c);

// The color "Capri"  (0, 191, 255).
extern DX_RGB_N8 const dx_colors_capri;

// The color "Malachite" (0, 255, 64) from "Capri"'s tetradic palette.
extern DX_RGB_N8 const dx_colors_malachite;

// The color "Amber" (255, 192, 0) from "Capri"'s Split Complementary palette.
extern DX_RGB_N8 const dx_colors_amber;

// The color "White" (255, 255, 255).
extern DX_RGB_N8 const dx_colors_white;

// The color "Gray" (128, 128, 128).
extern DX_RGB_N8 const dx_colors_gray;

// The color "Black" (0, 0, 0).
extern DX_RGB_N8 const dx_colors_black;

// The color "Red" (255, 0, 0).
extern DX_RGB_N8 const dx_colors_red;

// The color "Green" (0, 255, 0).
extern DX_RGB_N8 const dx_colors_green;

// The color "Blue" (0, 0, 255).
extern DX_RGB_N8 const dx_colors_blue;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// Symbolic constant for the dx_f32 representation of PI.
#define DX_PI_F32 3.1415926f

// @brief Convert an angle in degrees into the corresponding angle in radians.
// @param a The angle in degrees.
// @return The corresponding angle in radians.
static inline dx_f32 dx_deg_to_rad(dx_f32 x) {
  return (x / 180.f) * DX_PI_F32;
}

// @brief Clamp a value to the range [0,1].
// @param x The value to be clamped.
// @return x clamped.
static inline dx_f32 dx_clamp(dx_f32 x) {
  if (x < 0.f) return 0.f;
  else if (x > 1.f) return 1.f;
  else return x;
}

static inline void dx_lerp(dx_f32 *result, dx_f32 start, dx_f32 end, dx_f32 t) {
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

typedef struct DX_VEC2 {
  dx_f32 e[2];
} DX_VEC2;

static inline void dx_vec2_set(DX_VEC2* v, dx_f32 x, dx_f32 y) {
  v->e[0] = x;
  v->e[1] = y;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC3 {
  dx_f32 e[3];
} DX_VEC3;

static inline void dx_vec3_set(DX_VEC3* v, dx_f32 x, dx_f32 y, dx_f32 z) {
  v->e[0] = x;
  v->e[1] = y;
  v->e[2] = z;
}

static inline dx_bool dx_vec3_are_equal(DX_VEC3 const* u, DX_VEC3 const* v) {
  return u->e[0] == v->e[0]
      && u->e[1] == v->e[1]
      && u->e[2] == v->e[2];
}

// get the squared length of a vector
// @param v the vector
// @return the squared length of the vector
dx_f32 dx_vec3_sql(DX_VEC3 const* v);

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
void dx_vec3_add3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

/// @ingroup math
/// @brief Compute the difference of two vectors.
/// @param w Pointer to a DX_VEC3 object.
/// @param u Pointer to a DX_VEC3 object.
/// The object's values represent the vector that is the minuend (aka the 1st operand).
/// @param v Pointer to a DX_VEC3 object.
/// The object's values represent hte vector that is the the subtrahend (aka the 2nd operand).
/// @remarks @a w, @a u, and @a v all may refer to the same object.
/// @post <code>*w/<code> was assigned the values of the difference vector.
void dx_vec3_sub3(DX_VEC3* w, DX_VEC3 const* u, DX_VEC3 const* v);

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

static inline void dx_vec3_lerp(DX_VEC3 const* u, DX_VEC3 const* v, dx_f32 t, DX_VEC3* w) {
  t = dx_clamp(t);
  if (t == 0.f) {
    *w = *u;
  } else if (t == 1.f) {
    *w = *v;
  } else {
    w->e[0] = (1.f - t) * u->e[0] + t * v->e[0];
    w->e[1] = (1.f - t) * u->e[1] + t * v->e[1];
    w->e[2] = (1.f - t) * u->e[2] + t * v->e[2];
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct DX_VEC4 {
  dx_f32 e[4];
} DX_VEC4;

static inline void dx_vec4_set(DX_VEC4* v, dx_f32 x, dx_f32 y, dx_f32 z, dx_f32 w) {
  v->e[0] = x;
  v->e[1] = y;
  v->e[2] = z;
  v->e[3] = w;
}

static inline dx_bool dx_vec4_are_equal(DX_VEC4 const* u, DX_VEC4 const* v) {
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

static inline void dx_vec4_lerp(DX_VEC4 const* u, DX_VEC4 const* v, dx_f32 t, DX_VEC4* w) {
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

// A 4x4 dx_f32 matrix POD.
typedef struct DX_MAT4 {
  // index i,j denotes i-th row and j-th colmn
  dx_f32 e[4][4];
} DX_MAT4;

// assign this matrix the values of an identity matrix
void dx_mat4_set_identity(DX_MAT4* m);

// assign this matrix the values of a translation matrix
void dx_mat4_set_translate(DX_MAT4* m, dx_f32 x, dx_f32 y, dx_f32 z);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the x-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_x(DX_MAT4* a, dx_f32 x);

// assign this matrix the values of transformation matrix representing
// a counter-clockwise rotation around the y-axis by the specified number of degrees.
// @param m A pointer to this matrix.
// @param a The angle of rotation, in degrees.
void dx_mat4_set_rotate_y(DX_MAT4* a, dx_f32 x);

/// Assign a DX_MAT4 object the values of an orthographic projection matrix such that
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
void dx_mat4_set_ortho(DX_MAT4* m, dx_f32 left, dx_f32 right, dx_f32 bottom, dx_f32 top, dx_f32 near, dx_f32 far);

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
void dx_mat4_set_perspective(DX_MAT4* m, dx_f32 field_of_view_y, dx_f32 aspect_ratio, dx_f32 near, dx_f32 far);

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

typedef struct DX_RECT2_F32 {
  dx_f32 left;
  dx_f32 bottom;
  dx_f32 right;
  dx_f32 top;
} DX_RECT2_F32;

static inline void dx_rect2_f32_union(DX_RECT2_F32* a, DX_RECT2_F32 const* x, DX_RECT2_F32 const* y) {
#pragma push_macro("MIN")
#pragma push_macro("MAX")
#define MIN(x,y) (x) < (y) ? (x) : (y)
#define MAX(x,y) (x) > (y) ? (x) : (y)
  a->left = MIN(x->left, y->left);
  a->bottom = MIN(x->bottom, y->bottom);
  a->right = MAX(x->right, y->right);
  a->top = MAX(x->top, y->top);
#undef MAX
#undef MIN
#pragma pop_macro("MAX")
#pragma pop_macro("MIN")
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_MATH_WITH_TESTS) && 1 == DX_MATH_WITH_TESTS
int dx_math_tests();
#endif

#endif // DX_MATH_H_INCLUDED
