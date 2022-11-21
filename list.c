
#include <assert.h>
#include <stdlib.h>

#include <list.h>
#include "result.h"
/**
 * Node struct
 * value
 * next node ref
 * destructor of node
 */
typedef struct Node
{
    void *value;
    struct Node *next;
    void (*dtor)(void *);
} Node;

/**
 * List struct store head of list.
 */
typedef struct List_C
{
    Node *head;
} List_C;

/**
 * Iterator to store refrenced node
 */
typedef struct List_Iter
{
    Node *node;
} List_Iter;

Result create(List_C **list)
{
    assert(list != NULL);

    Result result = SUCCESS;

    // allocate multiple blocks of memory each of the same size all bytes is set to 1u(unisgned value with single bit 0 set)

    List_C *n_list = (List_C *)calloc(sizeof(List_C), 1u);
    if (n_list == NULL)
    {
        // Failed to allocaten_list
        result = FAILED;
        destroy(n_list);
        return result;
    }

    // allocate the head node
    n_list->head = (Node *)calloc(sizeof(Node), 1u);

    if (n_list == NULL)
    {
        result = FAILED;
        destroy(n_list);
        return result;
    }
    // user pointer
    *list = n_list;

    return result;
}

void destroy(List_C *list)
{
    // if list is null return
    if (list == NULL)
    {
        return;
    }
    // assign list head to node
    Node *curr = list->head;
    // walk through the linked list
    // if current node dtor is not null call it on value
    // free curr node
    // set curr to next(advance)
    do
    {
        Node *next = curr->next;

        if (curr->dtor != NULL)
        {
            curr->dtor(curr->value);
        }
        free(curr);
        curr = next;
    } while (curr != NULL);

    // free list
    free(list);
}

Result push(List_C *list, void *value)
{
    // TODO :implement
}

void remove(List_C *list, const List_Iter *iter)
{
    assert(list != NULL);
    assert(iter != NULL);
    Node *curr = list->head;
    // walkthrough list to find the node to delete.
    while ((curr->next != NULL) && (curr->next != iter->node))
    {
        curr = curr->next;
    }

    // did we reached the end
    if (curr->next != NULL)
    {
        // next is the node to delete
        Node *trash = curr->next;
        // store next next node
        Node *next = trash->next;
        // vall dtor is set
        if (trash->dtor != NULL)
        {
            trash->dtor(trash->value);
        }

        // delete trash node
        free(trash);
        // reconnect the curr node to next next
        curr->next = next;
    }
}

Result create_iter(const List_C *list, List_Iter **iter)
{
    assert(list != NULL);
    assert(iter != NULL);

    Result result = SUCCESS;
    List_Iter *n_iter = (List_Iter *)calloc(sizeof(List_Iter), 1u);

    if (n_iter == NULL)
    {
        // Failed to allocate iter
        result = FAILED;
        return result;
    }
    reset_iter(list, &n_iter);
    //user pointer assign new iter
    *iter = n_iter;
    return result;
}

void destroy_iter(List_C *iter)
{
    free(iter);
}

void next_node(List_Iter **iter)
{
    assert(*iter != NULL);
    (*iter)->node = (*iter)->node->next;
}

void reset_iter(const List_C *list, List_Iter **iter)
{
    assert(*iter != NULL);
    (*iter)->node = list->head->next;
}

bool is_iter_end(List_Iter *iter)
{
    assert(iter != NULL);

    return iter->node == NULL;
}

void *iter_value(const List_Iter *iter)
{
   assert(iter != NULL);
   return iter->node->value;
}
