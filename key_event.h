
/**
 * Possible key states.
 */
typedef enum KeyState
{
    K_UP,
    K_DOWN
} KeyState;

/**
 * Possible input keys 
 */
typedef enum Key
{
    ESCAPE_K,
    LEFT_K,
    RIGHT_K,
} Key;

/**
 * Key events struct
 */
typedef struct KeyEvent
{
    KeyState key_state;
    Key key;
} KeyEvent;
