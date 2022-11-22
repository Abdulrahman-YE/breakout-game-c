#include "vector.h"

#include <assert.h>
#include <stdio.h>

Vector2D create_vec()
{
    return create_vec_xy(0.0f, 0.0f);
}

Vector2D create_vec_xy(float x, float y)
{
    Vector2D vec = {.x = x, .y = y};
    return vec;
}

void add_vec(Vector2D *vec1, const Vector2D *vec2)
{
    assert(vec1 != NULL);
    assert(vec2 != NULL);

    vec1->x += vec2->x;
    vec1->y += vec2->y;
}

void add_vec_xy(Vector2D *vec, float x, float y)
{
    assert(vec != NULL);

    vec->x += x;
    vec->y += y;
}

void print_vec(const Vector2D *vec)
{
    assert(vec != NULL);

    printf("{ x: %f, y: %f }\n", vec->x, vec->y);
}
