
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

        CHECK_SUCCESS(_push(entities, e, &free), "failed to add brick");

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

/**
 * Helper function to check if two entities are colliding.
 *
 * @param entity1
 *   First entity to check.
 *
 * @param entity2
 *   Second entity to check.
 *
 * @returns
 *   True if entities collide, false otherwise.
 */
bool static check_collision(const Entity *entity1, const Entity *entity2)
{
    return (
        (entity1->block.position.x < entity2->block.position.x + entity2->block.width) &&
        (entity1->block.position.x + entity1->block.width > entity2->block.position.x) &&
        (entity1->block.position.y < entity2->block.position.y + entity2->block.height) &&
        (entity1->block.height + entity1->block.position.y > entity2->block.position.y));
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
        CHECK_SUCCESS(create_iter(entities, &iter), "could not create iterator");

        // move past ball and paddle
        next_node(&iter);
        next_node(&iter);

        // see if the ball intersects with any bricks
        while (!is_iter_end(iter))
        {
            Entity *block = (Entity *)iter_value(iter);

            if (check_collision(ball, block))
            {
                printf("Ball Postion : (%f,%f)", ball->block.position.x, ball->block.position.y);
                remove_node(entities, iter);
                ball_velocity->y *= -0.1f;

                // if we modify the list this will invalidate the iterator, so stop
                break;
            }

            next_node(&iter);
        }

        destroy_iter(iter);
    }

    // handle ball - paddle collision
    if (check_collision(ball, paddle))
    {
        printf("Ball Postion : (%f,%f)\n", ball->block.position.x, ball->block.position.y);
        printf("Paddle Postion : (%f,%f)\n", paddle->block.position.x, paddle->block.position.y);
        if (ball->block.position.x < paddle->block.position.x + 6.0f)
        {
            printf("Reverse x and y velocity by -0.5");

            ball_velocity->x = -0.5f;
            ball_velocity->y = -0.5f;
        }
        else if (ball->block.position.x < paddle->block.position.x + 14.0f)
        {
            printf("Reverse y velocity by -0.5");
            ball_velocity->x = 0.0f;
            ball_velocity->y = -0.5f;
        }
        else
        {
            printf("Inceease x velocity by 0.5 and Reverse y velctey by -0.5");
            ball_velocity->x = 0.5f;
            ball_velocity->y = -0.5f;
        }
    }
}

int main()
{
    printf("hello world\n");

    Entity paddle = {
        .block = create_block_xy(300.0f, 780.0f, 300.0f, 20.0f), .r = 0xff, .g = 0xff, .b = 0xff};
    Entity ball = {.block = create_block_xy(420.0f, 400.0f, 10.0f, 10.0f), .r = 0xff, .g = 0xff, .b = 0xff};

    Vector2D paddle_velocity = create_vec();
    Vector2D ball_velocity = create_vec_xy(0.f, 0.1f);

    List *entities = NULL;
    CHECK_SUCCESS(create_list(&entities), "failed to create entity list");

    CHECK_SUCCESS(push(entities, &paddle), "failed to add paddle to list");
    CHECK_SUCCESS(push(entities, &ball), "failed to add ball to list");

    create_brick_row(entities, 50.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 80.0f, 0xff, 0x00, 0x00);
    create_brick_row(entities, 110.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 140.0f, 0xff, 0xa5, 0x00);
    create_brick_row(entities, 170.0f, 0x00, 0xff, 0x00);
    create_brick_row(entities, 200.0f, 0x00, 0xff, 0x00);

    ListIter *iter = NULL;
    CHECK_SUCCESS(create_iter(entities, &iter), "failed to get entity iterator");

    // create window
    Window *window;
    CHECK_SUCCESS(create_window(&window), "failed to create window");

    KeyEvent event;
    bool running = true;

    const float paddle_speed = 1.0f;
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
                exit(1);
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

        CHECK_SUCCESS(pre_render_window(window), "pre render failed");

        while (!is_iter_end(iter))
        {
            Entity *entity = (Entity *)iter_value(iter);
            CHECK_SUCCESS(
                draw_block_window(window, &entity->block, entity->r, entity->g, entity->b),
                "failed to render entity");

            next_node(&iter);
        }

        post_render_window(window);
    }

    destroy_iter(iter);
    destroy_window(window);

    printf("Thank You for playing\n");

    return 0;
}
