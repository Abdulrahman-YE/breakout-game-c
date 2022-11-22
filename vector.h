#ifndef _VECTOR_H_
#define _VECTOR_H_
/**
 * 2-dim vector (x,y)/

/**
 * Struct for vector data. Deliberately public.
 */
typedef struct Vector2D
{
    float x;
    float y;
} Vector2D;

/**
 * Create a new veector with both components 0.0.
 *
 * @returns
 *  Vector2D with x and y set to 0.0.
 */
Vector2D create_vec();

/**
 * Create a new Vector2D with supplied component values.
 *
 * @param x
 *   X component.
 *
 * @param y
 *   Y component.
 *
 * @returns
 *   Vector2D with x and y set to supplied values.
 */
Vector2D create_vec_xy(float x, float y);

/**
 * Add one vector to another.
 *
 * @param vec1
 *   The source vector, this will be modified.
 *
 * @param vec2
 *   The vector to add to vec1.
 */
void add_vec(Vector2D *vec1, const Vector2D *vec2);

/**
 * Add values to a vector.
 *
 * @param vec
 *   The source vector, this will be modified.
 *
 * @param x
 *   Value to add to x component.
 *
 * @param y
 *   Value to add to y component.
 */
void add_vec_xy(Vector2D *vec, float x, float y);

/**
 * Print vector to stdout.
 *
 * @param vec
 *
 *
 */
void print_vec(const Vector2D *vec);

#endif