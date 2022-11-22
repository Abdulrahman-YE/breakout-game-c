

#include <stdint.h>

#include "key_event.h"
#include "block.h"
#include "result.h"

/**
 * Window is responsible fro creating and destroying a window as well as rendering to it and getting event.
 */

/**
 * Window internal events handler.
 */
typedef struct Window Window;

/**
 * Create a new platform window.
 *
 * Note this library only supports creating one window, any calls to this function after the first successful call are
 * undefined.
 *
 * @param window
 *  created window object.
 *
 * @returns
 *   SUCCESS on success
 *   FAILED on failure
 */
Result create_window(Window **window);

/**
 * Destroy a window.
 *
 * @param window
 *   The window to destroy.
 */
void destroy_window(Window *window);

/**
 * Get avaliable event
 *
 * @param window
 *   Window to get events for.
 *
 * @param event
 *   Even to write.
 *
 * @returns
 *   SUCCESS on success
 *   NO_EVENT if there was no event to get
 *   FAILED on failure
 */
Result get_window_event(const Window *window, KeyEvent *event);

/**
 * Perform an pre-render tasks.
 *
 * @param window
 *   Window to render to.
 *
 * @returns
 *   SUCCESS on success
 *   FAILED on failure
 */
Result pre_render_window(const Window *window);

/**
 * Perform an post-render tasks.
 *
 * @param window
 *   Window to render to.
 */
void post_render_window(const Window *window);

/**
 * Draw a rectangle to the screen.
 *
 * This *must* be called after window_pre_render and before window_post_render for any given frame.
 *
 * @param window
 *   The window to render to.
 *
 * @param rectangle
 *   The rectangle to draw.
 *
 * @param r
 *   Red channel value.
 *
 * @param g
 *   Green channel value .
 *
 * @param b
 *   Blue channel  .
 *
 * @returns
 *   SUCCESS on success
 *   FAILED in failure
 */
Result draw_block_window(const Window *window, const Block *rectangle, uint8_t r, uint8_t g, uint8_t b);
