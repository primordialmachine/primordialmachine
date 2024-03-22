// Copyright (c) 2023-2024 Michael Heilmann.All rights reserved.
#if !defined(IDLIB_MATRIX_4X4_H_INCLUDED)
#define IDLIB_MATRIX_4X4_H_INCLUDED

#include "configure.h"
#include "scalar.h"
// NULL
#include <stddef.h>
#include "vector_3.h"

// 'Windows.h', which is frequently included in Windows
// programs, defines the macros 'near' and 'far' causing
// an unintended substitution of names of parameters in
// 'matrix_4x4.h'.This prevents this substitution.
#if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
	#pragma push_macro("near")
	#undef near
	#pragma push_macro("far")
	#undef far
#endif

/// @since 1.0
/// @brief A row-major matrix with elements of type idlib_f32.
/// Row major means: The first index denotes the row, the second index denotes the column.
typedef struct idlib_matrix_4x4_f32 {
	idlib_f32 e[4][4];
} idlib_matrix_4x4_f32;

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of an identity matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object.
static inline void
idlib_matrix_4x4_f32_set_identity
	(
		idlib_matrix_4x4_f32* target
	);

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a translation matrix.
/// @param target Pointer to the idlib_matrix_4x4_f32 object.
/// @param operand Pointer to the idlib_vector_3_f32 object representing the translation vector.
/// @remarks
/// @code
/// | 1 | 0 | 0 | x |
/// | 0 | 1 | 0 | y |
/// | 0 | 0 | 1 | z |
/// | 0 | 0 | 0 | 1 |
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_translation
	(
		idlib_matrix_4x4_f32* target,
		idlib_vector_3_f32* operand
	);

/// @since 1.0
/// @brief Assign an idlib_matrix_4x4_f32 object the values of a rotation matrix.
/// @details The rotation matrix is a counter-clockwise rotation around the x-axis by the specified number of degrees.
/// @param m A pointer to this matrix.
/// @param a The angle of rotation, in degrees.
/// @remarks
/// @code
/// | 1 | 0 |  0 | 0 |
/// | 0 | c | -s | 0 |
/// | 0 | s |  c | 0 |
/// | 0 | 0 |  0 | 1 |
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_rotation_x
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	);

/// @since 1.0
/// @brief Assign this matrix the values of transformation matrix representing
/// a counter-clockwise rotation around the y-axis by the specified number of degrees.
/// @param m A pointer to this matrix.
/// @param a The angle of rotation, in degrees.
/// @remarks
/// @code
/// |  c | 0 | s | 0 |
/// |  0 | 1 | 0 | 0 |
/// | -s | 0 | c | 0 |
/// |  0 | 0 | 0 | 1 |
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_rotation_y
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	);

/// @since 1.0
/// @brief Assign this matrix the values of transformation matrix representing
/// a counter-clockwise rotation around the z-axis by the specified number of degrees.
/// @param target A pointer to this matrix.
/// @param operand The angle of rotation, in degrees.
/// @remarks
/// @code
/// | c | -s | 0 | 0 |
/// | s |  c | 0 | 0 |
/// | 0 |  0 | 1 | 0 |
/// | 0 |  0 | 0 | 1 |
/// @endcode
static inline void
idlib_matrix_4x4_f32_set_rotation_z
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	);

/// @since 1.0
/// @brief Assign this matrix the values of an orthographic projection matrix.
/// @param target A pointer to this matrix.
/// @param left The distance to the left clip plane.
/// @param right The distance to the right clip plane.
/// @param bottom The distance to the bottom clip plane.
/// @param top The distance to the top clip plane.
/// @param near The distance to the near clip plane.
/// @param far The distance to the far clip plane.
/// @remarks
/// The following matrix is created
/// @code
/// | 2/a | 0   | 0     | u |
/// | 0   | 2/b | 0     | v |
/// | 0   | 0   | - 2/c | w |
/// | 0   | 0   | 0     | 1 |
/// @endcode
/// where
/// @code
/// a = right - left
/// b = top - bottom
/// c = far - near
/// u = -(right + left) / a
/// v = -(top + bottom) / b
/// w = -(far + near) / 2c
/// @endcode
/// @remarks
/// A few properties of the transformation
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
static inline void
idlib_matrix_4x4_f32_set_ortho
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 left,
		idlib_f32 right,
		idlib_f32 bottom,
		idlib_f32 top,
		idlib_f32 near,
		idlib_f32 far
	);

/// @since 1.0
/// @brief Assign this matrix the values of a perspective projection matrix.
/// @param target A pointer to this matrix.
/// @param field_of_view_y The field of view along the y-axis in degrees.
/// In other terms: The angle, in degrees, in between a plane passing through the camera position as well as the top of your screen and another plane passing
/// through the camera position and the bottom of your screen.  The bigger this angle is, the more you can see of the world - but at the same time, the objects
/// you can see will become smaller.
/// @param aspect_ratio The aspect ratio, that is, the ratio of the width to the height of the screen.
/// An aspect ratio of x means that the width is x times the height.
/// The aspect ratio is usually computed by width / height.
/// @param near The distance to the near clip plane.
/// @param far The distance to the far clip plane.
/// @remarks
/// @remarks
/// This function creates the following matrix
/// @code
/// | f / aspectRatio | 0                       | 0                          0 |
/// | 0               | f                       | 0                          0 |
/// | 0               | 0 (far+near)/(near-far) | (2 * far *  near)/(near-far) |
/// | 0               | 0                    -1 |                            0 |
/// @endcode
/// where
/// @code
/// f = cot(fieldOfVision/2)
/// @endcode
/// @remarks
/// A few properties of the transformation
/// - the positive z-axis points out of the screen (negative z-axis points into the screen)
/// - the positive x-axis points to the right
/// - the positive y-axis points to the top
static inline void
idlib_matrix_4x4_f32_set_perspective
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 field_of_view_y,
		idlib_f32 aspect_ratio,
		idlib_f32 near,
		idlib_f32 far
	);

/// @since 1.0
/// @brief Compute the product of two matrices.
/// @param target Pointer to a idlib_matrix_4x4_f32 object to assign the result to.
/// @param operand1 Pointer to a idlib_matrix_4x4_f32 object, the multiplier (first operand).
/// @param operand2 Pointer to a idlib_matrix_4x4_f32 object, the multiplicand (second operand).
/// @remarks @a target, @a operand1, and @a operand2 all may refer to the same object.
static inline void
idlib_matrix_4x4_f32_multiply
	(
		idlib_matrix_4x4_f32* target,
		idlib_matrix_4x4_f32 const* operand1,
		idlib_matrix_4x4_f32 const* operand2
	);

/// @since 1.0
/// @brief Assign this matrix the value a of a view matrix.
/// @param target A pointer to the idlib_matrix_4x4_f32 object to assign the result to.
/// @param source A pointer to the idlib_matrix_4x4_f32 object representing the point at which the viewer is positioned at
/// @param target A pointer to the idlib_matrix_4x4_f32 object representing the point at which the viewer is looking at
/// @param up A pointer to the idlib_matrix_4x4_f32 object representing the upward direction of the viewer.
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
static inline void
idlib_matrix_4x4_f32_set_look_at
	(
		idlib_matrix_4x4_f32* target,
		idlib_vector_3_f32 const* operand1,
		idlib_vector_3_f32 const* operand2,
		idlib_vector_3_f32 const* operand3
	);

static inline void
idlib_matrix_4x4_f32_set_identity
	(
		idlib_matrix_4x4_f32* target
	)
{
	IDLIB_DEBUG_ASSERT(NULL != target);

	// first column
	target->e[0][0] = 1.f;
	target->e[1][0] = 0.f;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// second column
	target->e[0][1] = 0.f;
	target->e[1][1] = 1.f;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// third column
	target->e[0][2] = 0.f;
	target->e[1][2] = 0.f;
	target->e[2][2] = 1.0f;
	target->e[3][2] = 0.f;

	// fourth column
	target->e[0][3] = 0.f;
	target->e[1][3] = 0.f;
	target->e[2][3] = 0.0f;
	target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_translation
	(
		idlib_matrix_4x4_f32* target,
		idlib_vector_3_f32* operand
	)
{
	// first column
	target->e[0][0] = 1.f;
	target->e[1][0] = 0.f;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// second column
	target->e[0][1] = 0.f;
	target->e[1][1] = 1.f;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// third column
	target->e[0][2] = 0.f;
	target->e[1][2] = 0.f;
	target->e[2][2] = 1.f;
	target->e[3][2] = 0.f;

	// column #4
	target->e[0][3] = operand->e[0];
	target->e[1][3] = operand->e[1];
	target->e[2][3] = operand->e[2];
	target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_x
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	)
{
	idlib_f32 a = idlib_deg_to_rad_f32(operand);
	idlib_f32 c = idlib_cos_f32(a);
	idlib_f32 s = idlib_sin_f32(a);

	// First column.
	target->e[0][0] = 1.f;
	target->e[1][0] = 0.f;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// Second column.
	target->e[0][1] = 0.f;
	target->e[1][1] = c;
	target->e[2][1] = s;
	target->e[3][1] = 0.f;

	// Third column.
	target->e[0][2] = 0.f;
	target->e[1][2] = -s;
	target->e[2][2] = c;
	target->e[3][2] = 0.f;

	// Fourth column.
	target->e[0][3] = 0.f;
	target->e[1][3] = 0.f;
	target->e[2][3] = 0.f;
	target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_y
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	)
{
	idlib_f32 a = idlib_deg_to_rad_f32(operand);
	idlib_f32 c = idlib_cos_f32(a);
	idlib_f32 s = idlib_sin_f32(a);

	// First column.
	target->e[0][0] = c;
	target->e[1][0] = 0.f;
	target->e[2][0] = -s;
	target->e[3][0] = 0.f;

	// Second column.
	target->e[0][1] = 0.f;
	target->e[1][1] = 1.f;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// Third column.
	target->e[0][2] = s;
	target->e[1][2] = 0.f;
	target->e[2][2] = c;
	target->e[3][2] = 0.f;

	// Fourth column.
	target->e[0][3] = 0.f;
	target->e[1][3] = 0.f;
	target->e[2][3] = 0.f;
	target->e[3][3] = 1.f;
}

static inline void
idlib_matrix_4x4_f32_set_rotation_z
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 operand
	)
{
	idlib_f32 a = idlib_deg_to_rad_f32(operand);
	idlib_f32 c = idlib_cos_f32(a);
	idlib_f32 s = idlib_sin_f32(a);

	// First column.
	target->e[0][0] = c;
	target->e[1][0] = s;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// Second column.
	target->e[0][1] = -s;
	target->e[1][1] = c;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// Third column.
	target->e[0][2] = 0.f;
	target->e[1][2] = 0.f;
	target->e[2][2] = 1.f;
	target->e[3][2] = 0.f;

	// Fourth column.
	target->e[0][3] = 0.f;
	target->e[1][3] = 0.f;
	target->e[2][3] = 0.f;
	target->e[3][3] = 1.f;
}

inline void
idlib_matrix_4x4_f32_set_ortho
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 left,
		idlib_f32 right,
		idlib_f32 bottom,
		idlib_f32 top,
		idlib_f32 near,
		idlib_f32 far
	)
{
	idlib_f32 a = right - left;
	idlib_f32 b = top - bottom;
	idlib_f32 c = far - near;

	idlib_f32 u = -(right + left) / a;
	idlib_f32 v = -(top + bottom) / b;
	idlib_f32 w = -(far + near) / c;

	// column #1
	target->e[0][0] = 2.f / a;
	target->e[1][0] = 0.f;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// column #2
	target->e[0][1] = 0.f;
	target->e[1][1] = 2.f / b;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// column #3
	target->e[0][2] = 0.f;
	target->e[1][2] = 0.f;
	target->e[2][2] = -2.f / c;
	target->e[3][2] = 0.f;

	// column #4
	target->e[0][3] = u;
	target->e[1][3] = v;
	target->e[2][3] = w;
	target->e[3][3] = 1.f;
}

inline void
idlib_matrix_4x4_f32_set_perspective
	(
		idlib_matrix_4x4_f32* target,
		idlib_f32 field_of_view_y,
		idlib_f32 aspect_ratio,
		idlib_f32 near,
		idlib_f32 far
	)
{
	field_of_view_y = idlib_deg_to_rad_f32(field_of_view_y); // rad(x) = x / 360 * 2 * PI = x * (PI * / 180)
	idlib_f32 f = 1.f / idlib_tan_f32(field_of_view_y / 2.f); // cot(x) = 1 / tan(x)

	// column #1
	target->e[0][0] = f / aspect_ratio;
	target->e[1][0] = 0.f;
	target->e[2][0] = 0.f;
	target->e[3][0] = 0.f;

	// column #2
	target->e[0][1] = 0.f;
	target->e[1][1] = f;
	target->e[2][1] = 0.f;
	target->e[3][1] = 0.f;

	// column #3
	target->e[0][2] = 0.f;
	target->e[1][2] = 0.f;
	target->e[2][2] = (far + near) / (near - far);
	target->e[3][2] = -1.f;

	// column #4
	target->e[0][3] = 0.f;
	target->e[1][3] = 0.f;
	target->e[2][3] = (2.f * far * near) / (near - far); // - (2 far near) / (far - near)
	target->e[3][3] = 0.f;
}

#if IDLIB_COMPILER_C == IDLIB_COMPILER_C_MSVC
	#pragma pop_macro("near")
	#pragma pop_macro("far")
#endif

static inline void
idlib_matrix_4x4_f32_multiply
	(
		idlib_matrix_4x4_f32* target,
		idlib_matrix_4x4_f32 const* operand1,
		idlib_matrix_4x4_f32 const* operand2
	)
{
	if (target != operand1 && target != operand2) {
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				target->e[i][j] = 0.f;
				for (size_t k = 0; k < 4; ++k) {
					target->e[i][j] += operand1->e[i][k] * operand2->e[k][j];
				}
			}
		}
	} else {
		idlib_f32 t[4][4];

		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				t[i][j] = 0.f;
				for (size_t k = 0; k < 4; ++k) {
					t[i][j] += operand1->e[i][k] * operand2->e[k][j];
				}
			}
		}
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				target->e[i][j] = t[i][j];
			}
		}
	}
}

static inline void
idlib_matrix_4x4_f32_set_look_at
	(
		idlib_matrix_4x4_f32* target,
		idlib_vector_3_f32 const* operand1,
		idlib_vector_3_f32 const* operand2,
		idlib_vector_3_f32 const* operand3
	)
{
	idlib_vector_3_f32 forward, right, up1, up2;
	idlib_matrix_4x4_f32 r, t;

	// forward := norm(target - source)
	idlib_vector_3_f32_subtract(&forward, operand2, operand1);
	idlib_vector_3_f32_normalize(&forward, &forward);
	// right := forward x norm(up)
	idlib_vector_3_f32_normalize(&up1, operand3);
	idlib_vector_3_f32_cross(&right, &forward, &up1);
	// up' := right x forward
	idlib_vector_3_f32_cross(& up2, & right, & forward);

	// First column.
	r.e[0][0] = right.e[0];
	r.e[1][0] = up2.e[0];
	r.e[2][0] = -forward.e[0];
	r.e[3][0] = 0.f;
	// Second column.
	r.e[0][1] = right.e[1];
	r.e[1][1] = up2.e[1];
	r.e[2][1] = -forward.e[1];
	r.e[3][1] = 0.f;
	// Third column.
	r.e[0][2] = right.e[2];
	r.e[1][2] = up2.e[2];
	r.e[2][2] = -forward.e[2];
	r.e[3][2] = 0.f;
	// Fourth column.
	r.e[0][3] = 0.f;
	r.e[1][3] = 0.f;
	r.e[2][3] = 0.f;
	r.e[3][3] = 1.f;

	idlib_vector_3_f32 negae;
	negae = *operand1;
	idlib_vector_3_f32_negate(&negae, &negae);
	idlib_matrix_4x4_f32_set_translation(&t, &negae);

	idlib_matrix_4x4_f32_multiply(target, &r, &t);
}

#endif // IDLIB_MATRIX_4X4_H_INCLUDED
