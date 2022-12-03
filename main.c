#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "window.h"

/**
 * Struct encapsulating the data for a renderable entity.
 */
typedef struct Entity
{
    Block block;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Entity;

/**
 * Helper macro for checking if a value is SUCCESS. If not it prints a FAILED
 */
#define CHECK_SUCCESS(X, MSG)                   \
    do                                          \
    {                                           \
        Result r = X;                           \
        if (r != SUCCESS)                       \
        {                                       \
            printf("%s [error: %i]\n", MSG, r); \
            exit(1);                            \
        }                                       \
    } while (false)

/**
 * Helper function to create a row of ten bricks.
 *
 * @param entities
 *   List to store entities in.
 *
 * @param y
 *   Y coordinate of row.
 *
 * @param r
 *   Red component of brick colour.
 *
 * @param g
 *   Green component of brick colour.
 *
 * @param b
 *  Blue component of brick colour.
 */
static void create_brick_row(List *entities, float y, uint8_t r, uint8_t g, uint8_t b)
{
    float x = 20.0f;

    for (int i = 0; i < 10; ++i)
    {
        Entity *e = (Entity *)calloc(sizeof(Entity), 1u);
        e->block.position.x = x;
        e->block.position.y = y;
        e->block.width = 58.0f;
        e->block.height = 20.0f;
        e->r = r;
        e->g = g;
        e->b = b;

        CHECK_SUCCESS(_push(entities, e, &free), "failed to add brick\n");

        x += 78.0f;
    }
}

/**
 * Helper function to update the ball.
 *
 * @param ball
 *   Entity for ball.
 *
 * @param ball_velocity
 *   Velocity of ball.
 */
static void update_ball(Entity *ball, Vector2D *ball_velocity)
{
    add_vec(&ball->block.position, ball_velocity);

    // if ball does out of the screen then invert the y velocity
    if ((ball->block.position.y < 0.0f) || (ball->block.position.y > 800.0f))
    {
        ball_velocity->y *= -1.0f;
    }

    if ((ball->block.position.x < 0.0f) || (ball->block.position.x > 800.0f))
    {
        ball_velocity->x *= -1.0f;
    }
}

typedef struct CollosionResult
{
    bool overlap;
    float shift_b_x;
    float shift_b_y;
} CollosionResult;

/**
 * Helper function to check if two entities are colliding.
 *
 * @param a
 *   First entity to check.
 *
 * @param b
 *   Second entity to check.
 *
 * @returns
 *   TCollosionResult : (overlap : true if collosion detedted)
 */
CollosionResult static check_collision(const Entity *a, const Entity *b)
{
    bool overlap = false;
    float shift_b_x, shift_b_y = 0.0f;

    // b is the one that has to be displaced, and the a should remain in place.
    if (!((a->block.position.x + a->block.width < b->block.position.x) || (b->block.position.x + b->block.width < a->block.position.x) || (a->block.position.y + a->block.height < b->block.position.y) || (b->block.position.y + b->block.height < a->block.position.y)))
    {
        overlap = true;
        if ((a->block.position.x + a->block.width / 2) < (b->block.position.x + b->block.width / 2))
        {
            // b to the right from the center of a; shift b to the right
            shift_b_x = (a->block.position.x + a->block.width) - b->block.position.x;
        }
        else
        {
            // b to the left from a; shift to the left
            shift_b_x = a->block.position.x - (b->block.position.x + b->block.width);
        }
        if ((a->block.position.y + a->block.height / 2) < (b->block.position.y + b->block.height / 2))
        {
            // same for y axis
            shift_b_y = (a->block.position.y + a->block.height) - b->block.position.y;
        }
        else
        {
            // same for y axis
            shift_b_y = a->block.position.y - (b->block.position.y + b->block.height);
        }
    }
    CollosionResult result = {overlap, shift_b_x, shift_b_y};
    return result;
}

static void ball_rebound(Entity *ball, CollosionResult *result, Vector2D *ball_velocity)
{
    printf("Ball Postion : (%f,%f)\n", ball->block.position.x, ball->block.position.y);

    float min_shift = abs(result->shift_b_x) < abs(result->shift_b_y) ? result->shift_b_x : result->shift_b_y;

    if (abs(result->shift_b_x) == min_shift)
    {
        result->shift_b_y = 0.0f;
        printf("Shift ball y = 0\n");
    }
    else
    {
        printf("Shift ball x = 0\n");
        result->shift_b_x = 0.0f;
    }
    ball->block.position.x += result->shift_b_x;
    ball->block.position.y += result->shift_b_y;

    if (result->shift_b_x != 0)
    {
        printf("reverse ball velocity on x\n");
        ball_velocity->x = -ball_velocity->x;
    }
    if (result->shift_b_y != 0)
    {
        printf("reverse ball velocity on y\n");
        ball_velocity->y = -ball_velocity->y;
    }
}

/**
 * Helper function to handle collisions between the ball and other entities.
 *
 * @param entities
 *   List of all entities.
 *
 * @param ball
 *   Ball entity.
 *
 * @param ball_velocity
 *   The velocity of the ball.
 *
 * @param paddle
 *   Paddle entity.
 */
static void handle_collisions(List *entities, Entity *ball, Vector2D *ball_velocity, const Entity *paddle)
{
    // keep iterator scoped so we can't use it after it's been destroyed
    {
        ListIter *iter;
        CHECK_SUCCESS(create_iter(entities, &iter), "could not create iterator\n");

        // move past ball and paddle
        next_node(&iter);
        next_node(&iter);

        // see if the ball intersects with any bricks
        while (!is_iter_end(iter))
        {
            Entity *block = (Entity *)iter_value(iter);

            CollosionResult result = check_collision(block, ball);
            if (result.overlap)
            {
                remove_node(entities, iter);
                ball_rebound(ball, &result, ball_velocity);
                // if we modify the list this will invalidate the iterator, so stop
                break;
            }

            next_node(&iter);
        }

        destroy_iter(iter);
    }

    // handle ball - paddle collision
    CollosionResult result = check_collision(paddle, ball);
    if (result.overlap)
    {
        ball_rebound(ball, &result, ball_velocity);
    }
}

int main()
{
    printf("Game Starting\n");

    Entity paddle = {
        .block = create_block_xy(100.0f, 780.0f, 100.0f, 20.0f), .r = 0xff, .g = 0xff, .b = 0xff};
    Entity ball = {.block = create_block_xy(420.0f, 400.0f, 10.0f, 10.0f), .r = 0xff, .g = 0xff, .b = 0xff};

    Vector2D paddle_velocity = create_vec();
    Vector2D ball_velocity = create_vec_xy(0.2f, 0.2f);

    List *entities = NULL;
    CHECK_SUCCESS(create_list(&entities), "failed to create entity list\n");

    CHECK_SUCCESS(push(entities, &paddle), "failed to add paddle to list\n");
    CHECK_SUCCESS(push(entities, &ball), "failed to add ball to list\n");

    create_brick_row(entities, 50.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 80.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 110.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 140.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 170.0f, 0x00, 0xff, 0x00);
    create_brick_row(entities, 200.0f, 0x00, 0xff, 0x00);

    ListIter *iter = NULL;
    CHECK_SUCCESS(create_iter(entities, &iter), "failed to get entity iterator\n");

    // create window
    Window *window;
    CHECK_SUCCESS(create_window(&window), "failed to create window\n");

    KeyEvent event;
    bool running = true;

    const float paddle_speed = 0.4f;
    bool left_press = false;
    bool right_press = false;

    while (running)
    {
        // process all events
        for (;;)
        {
            Result event_result = get_window_event(window, &event);
            if (event_result == SUCCESS)
            {
                if ((event.key_state == K_DOWN) && (event.key == ESCAPE_K))
                {
                    running = false;
                }
                else if (event.key == LEFT_K)
                {
                    left_press = (event.key_state == K_DOWN) ? true : false;
                }
                else if (event.key == RIGHT_K)
                {
                    right_press = (event.key_state == K_DOWN) ? true : false;
                }
            }
            else if (event_result == NO_EVENT)
            {
                break;
            }
            else
            {
                printf("error getting event\n");
            }
        }

        if ((left_press && right_press) || (!left_press && !right_press))
        {
            paddle_velocity.x = 0.0f;
        }
        else if (left_press)
        {
            paddle_velocity.x = -paddle_speed;
        }
        else if (right_press)
        {
            paddle_velocity.x = paddle_speed;
        }

        add_vec(&paddle.block.position, &paddle_velocity);
        update_ball(&ball, &ball_velocity);
        handle_collisions(entities, &ball, &ball_velocity, &paddle);

        // reset iterator as we may have modified the list and we will want to start from the beginning anyway
        reset_iter(entities, &iter);

        // render our scene

        CHECK_SUCCESS(pre_render_window(window), "pre render failed\n");

        while (!is_iter_end(iter))
        {
            Entity *entity = (Entity *)iter_value(iter);
            CHECK_SUCCESS(
                draw_block_window(window, &entity->block, entity->r, entity->g, entity->b),
                "failed to render entity\n");

            next_node(&iter);
        }

        post_render_window(window);
    }

    destroy_iter(iter);
    destroy_window(window);

    printf("Thank You for playing\n");

    return 0;
}
