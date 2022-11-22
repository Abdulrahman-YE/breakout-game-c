

#include <stdbool.h>

#include <result.h>

/**
 * List data.
*/
typedef struct List List;

/**
 * List iterator
*/
typedef struct ListIter ListIter;

/**
 * Create new list
 * @param list
 *      Create list
 * @returns
 *  SUCCESS on success
 *  Failed on failure
*/
Result create_list(List **list);

/**
 * Destory list 
 * @param list
 *      List to destory
*/
void destory_list(List *list);

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
Result  push(List *list, void *value);

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
Result  _push(List *list, void *value, void (* dtor) (void *));

/**
 * Remove the node referenced by an iterator from a list.
 *
 * @param list
 *   List to remove_node node from.
 *
 * @param iter
 *   Iterator to node to remove_node.
 */
void remove_node(List *list, const ListIter *iter);

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
Result create_iter(const List *list, ListIter **iter);

/**
 * Destroy an iterator.
 *
 * @param iter
 *   Iterator to destory_list.
 */
void destroy_iter(ListIter *iter);


/**
 * Advance an iterator to the next node.
 *
 * @param iter
 *    iterator to advance to next node.
 */
void next_node(ListIter **iter);

/**
 * Reset an iterator back to the start of the list.
 *
 * @param list
 *   List to reset iterator in.
 *
 * @param iter
 *   iterator to reset.
 */
void reset_iter(const List *list, ListIter **iter);

/**
 * Check if an iterator past the end of the list
 *
 * @param iter
 *   Iterator to check.
 *
 * @returns
 *   True if iterator past the end of the list, otherwise false.
 */
bool is_iter_end(ListIter *iter);

/**
 * Get the value the iterator is referencing.
 *
 * @param iter
 *   Iterator to get value of.
 *
 * @returns
 *   Value at iterator.
 */
void *iter_value(const ListIter *iter);

