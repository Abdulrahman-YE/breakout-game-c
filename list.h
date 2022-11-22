

#include <stdbool.h>

#include <result.h>

/**
 * List data.
*/
typedef struct List_C LIST_C;

/**
 * List iterator
*/
typedef struct List_Iter List_Iter;

/**
 * Create new list
 * @param list
 *      Create list
 * @returns
 *  SUCCESS on success
 *  Failed on failure
*/
Result create_list(List_C **list);

/**
 * Destory list 
 * @param list
 *      List to destory
*/
void destory_list(List_C *list);

/**
 * Push a new value to the end of the list.
 *
 * @param list
 *   List to add to.
 *
 * @param value
 *   Value to add.
 *
 * @returns
 *  SUCCESS on success
*   FAILED on failure
 */
Result  push(List_C *list, void *value);

/**
 * Push a new value to the end of the list with destructor function (dtor)
 * dtor called when node is deleted or list is destroied
 *
 * @param list
 *   List to add to.
 *
 * @param value
 *   Value to add.
 *
 * @returns
 *  SUCCESS on success
*   FAILED on failure
 */
Result  _push(List_C *list, void *value, void (* dtor) (void *));

/**
 * Remove the node referenced by an iterator from a list.
 *
 * @param list
 *   List to remove node from.
 *
 * @param iter
 *   Iterator to node to remove.
 */
void remove(List_C *list, const List_Iter *iter);

/**
 * Create a new iterator to the first node.
 *
 * Note this will be NULL if the list is empty.
 *
 * @param list
 *   List to create_list iterator in.
 *
 * @param iter
 *  Out parameter for created iterator.
 *
 * @returns
 *  SUCCESS on success
 *  FAILED on failure
 */
Result create_iter(const List_C *list, List_Iter **iter);

/**
 * Destroy an iterator.
 *
 * @param iter
 *   Iterator to destory_list.
 */
void destroy_iter(List_Iter *iter);


/**
 * Advance an iterator to the next node.
 *
 * @param iter
 *    iterator to advance to next node.
 */
void next_node(List_Iter **iter);

/**
 * Reset an iterator back to the start of the list.
 *
 * @param list
 *   List to reset iterator in.
 *
 * @param iter
 *   iterator to reset.
 */
void reset_iter(const List_C *list, List_Iter **iter);

/**
 * Check if an iterator past the end of the list
 *
 * @param iter
 *   Iterator to check.
 *
 * @returns
 *   True if iterator past the end of the list, otherwise false.
 */
bool is_iter_end(List_Iter *iter);

/**
 * Get the value the iterator is referencing.
 *
 * @param iter
 *   Iterator to get value of.
 *
 * @returns
 *   Value at iterator.
 */
void *iter_value(const List_Iter *iter);

