////////////////////////////////////////////////////////////////////////////////
//         Distributed under the Boost Software License, Version 1.0.         //
//            (See accompanying file LICENSE or copy at                       //
//                 https://www.boost.org/LICENSE_1_0.txt)                     //
////////////////////////////////////////////////////////////////////////////////


#include <assert.h>
#include <stdio.h>


#include "block.h"
#include "vector.h"

Block create_block(const Vector2D *position, float width, float height)
{
    return create_block_xy(position->x, position->y, width, height);
}

Block create_block_xy(float x, float y, float width, float height)
{
    Block block = {.position = {.x = x, .y = y}, .width = width, .height = height};
    return block;
}

void move_block(Block *block, const Vector2D *move_amount)
{
    add_vec(&block->position, move_amount);
}

void move_block_xy(Block *block, float x, float y)
{
    add_vec_xy(&block->position, x, y);
}

void set_block_pos(Block *block, const Vector2D *position)
{
    block->position = *position;
}

void set_block_pos_xy(Block *block, float x, float y)
{
    block->position.x = x;
    block->position.y = y;
}

void print_block(const Block *block)
{
    printf(
        "{ x: %f, y: %f, w: %f, h: %f }\n",
        block->position.x,
        block->position.y,
        block->width,
        block->height);
}
