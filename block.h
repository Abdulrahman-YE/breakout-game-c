#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "vector.h"

/**
 * Block represented by poistion x,y , width and height
 */
typedef struct Block
{
    Vector2D position;
    float width;
    float height;
} Block;

/**
 * Create a new Block.
 *
 * @param position
 *   Position of block (upper left corner).
 *
 * @param width
 *   Block width.
 *
 * @param height
 *   Block height.
 *
 * @returns
 *   Block constructed with supplied values.
 */
Block create_block(const Vector2D *position, float width, float height);

/**
 * Create a new Block.
 *
 * @param x
 *   X coordinate of position (upper left corner).
 *
 * @param y
 *   Y coordinate of position (upper left corner).
 *
 * @param width
 *   Block widht.
 *
 * @param height
 *   Block height.
 *
 * @returns
 *   Block constructed with supplied values.
 */
Block create_block_xy(float x, float y, float width, float height);

/**
 * Move block.
 *
 * @param blcok
 *   block to move_amount.
 *
 * @param move_amount
 *   Move amount.
 */
void move_block(Block *block, const Vector2D *move_amount);

/**
 * Move block by x,y.
 *
 * @param block
 *   block to move_amount
 *
 * @param x
 *   Amount to move_amount along x axis.
 *
 * @param y
 *   Amount to move_amount along y axis.
 */
void move_block_xy(Block *block, float x, float y);

/**
 * Set the position of a block.
 *
 * @param block
 *   Block to set position of.
 *
 * @param position
 *   New block position.
 */
void set_block_pos(Block *block, const Vector2D *position);

/**
 * Set the position of a Block.
 *
 * @param block
 *   Block to set position of.
 *
 * @param x
 *   X coordinate of new position.
 *
 * @param y
 *   Y coordinate of new position.
 */
void set_block_pos_xy(Block *block, float x, float y);

/**
 * Print block to stdout.
 *
 * @param block
 *   block to print.
 */
void print_block(const Block *block);

#endif