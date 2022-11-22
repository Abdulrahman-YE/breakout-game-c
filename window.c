
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "window.h"

#include <SDL2/SDL.h>

typedef struct Window
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Window;

static Result map_sdl_key(Key *key, SDL_Keycode sdl_code)
{
    switch (sdl_code)
    {
    case SDLK_ESCAPE:
        *key = ESCAPE_K;
        return SUCCESS;
    case SDLK_LEFT:
        *key = LEFT_K;
        return SUCCESS;
    case SDLK_RIGHT:
        *key = RIGHT_K;
        return SUCCESS;
    fault:
        return NO_EVENT;
    }
}

Result create_window(Window **window)
{
    Result res = SUCCESS;

    // Initalize SDL
    // allocate space for the window object
    Window *n_window = (Window *)calloc(1u, sizeof(Window));
    if ((SDL_Init(SDL_INIT_VIDEO) != 0) || (n_window == NULL))
    {
        res = FAILED;
        destroy_window(n_window);
        return res;
    }

    // create an SDL window
    n_window->window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    if (n_window->window == NULL)
    {
        res = FAILED;
        destroy_window(n_window);
        return res;
    }

    // create an SDL renderer
    n_window->renderer = SDL_CreateRenderer(n_window->window, -1, 0);
    if (n_window->renderer == NULL)
    {
        res = FAILED;
        destroy_window(n_window);
        return res;
    }
    n_window->renderer = SDL_CreateRenderer(n_window->window, -1, 0);

    // assign the window to the user supplied pointer
    *window = n_window;
    return res;
}

void destroy_window(Window *window)
{
    if (window == NULL)
    {
        return;
    }

    if (window->window != NULL)
    {
        SDL_DestroyWindow(window->window);
    }

    free(window);

    SDL_Quit();
}

Result get_window_event(const Window *window, KeyEvent *event)
{
    assert(window != NULL);
    assert(event != NULL);

    Result result = NO_EVENT;

    // try and get an SDL event
    SDL_Event sdl_event;

    if (SDL_PollEvent(&sdl_event) != 0u)
    {
        bool is_key_mapped = false;
        if (sdl_event.type == SDL_KEYDOWN)
        {
            event->key_state = K_DOWN;
            is_key_mapped = true;
        }
        else if (sdl_event.type == SDL_KEYUP)
        {
            event->key_state = K_UP;
            is_key_mapped = true;
        }

        if (is_key_mapped)
        {
            // if we get here then we know we had a key event (either press or release), so convert the SDL key code
            // to our internal representation

            result = map_sdl_key(&(event->key), sdl_event.key.keysym.sym);
        }
    }
    return result;
}

Result pre_render_window(const Window *window)
{
    Result result = SUCCESS;

    // clear the window to black

    if (SDL_SetRenderDrawColor(window->renderer, 0x0, 0x0, 0x0, 0x0) != 0)
    {
        result = FAILED;
        return result;
    }

    if (SDL_RenderClear(window->renderer) != 0)
    {
        result = FAILED;
        return result;
    }

    return result;
}

void post_render_window(const Window *window)
{
    return SDL_RenderPresent(window->renderer);
}

Result draw_block_window(const Window *window, const Block *block, uint8_t r, uint8_t g, uint8_t b)
{
    Result result = SUCCESS;

    // convert our internal rect to an SDL rect
    SDL_Rect sdl_block = {
        .x = block->position.x, .y = block->position.y, .w = block->width, .h = block->height};

    // set the draw color
    if (SDL_SetRenderDrawColor(window->renderer, r, g, b, 0xff) != 0)
    {
        result = FAILED;
        return result;
    }

    // draw!
    if (SDL_RenderFillRect(window->renderer, &sdl_block) != 0)
    {
        result = FAILED;
        return result;
    }

    return result;
}
