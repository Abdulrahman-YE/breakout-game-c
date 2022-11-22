
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
typedef struct List
{
    Node *head;
} List;

/**
 * Iterator to store refrenced node
 */
typedef struct ListIter
{
    Node *node;
} ListIter;

Result create_list(List **list)
{
    assert(list != NULL);

    Result result = SUCCESS;

    // allocate multiple blocks of memory each of the same size all bytes is set to 1u(unisgned value with single bit 0 set)

    List *n_list = (List *)calloc(sizeof(List), 1u);
    if (n_list == NULL)
    {
        // Failed to allocaten_list
        result = FAILED;
        destory_list(n_list);
        return result;
    }

    // allocate the head node
    n_list->head = (Node *)calloc(sizeof(Node), 1u);

    if (n_list == NULL)
    {
        result = FAILED;
        destory_list(n_list);
        return result;
    }
    // user pointer
    *list = n_list;

    return result;
}

void destory_list(List *list)
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

Result push(List *list, void *value)
{
    return _push(list, value, NULL);
}

Result _push(List *list, void *value, void (*dtor)(void *))
{
    assert(list != NULL);

    Result result = SUCCESS;
    Node *curr = list->head;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    Node *n_node = (Node *)calloc(sizeof(Node), 1u);

    if (n_node == NULL)
    {
        // FAILED TO ALLOCATE NEW NODE
        result = FAILED;
        return result;
    }
    // Wire the new node to list
    curr->next = n_node;
    n_node->value = value;
    n_node->dtor = dtor;
    return result;
}

void remove_node(List *list, const ListIter *iter)
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

Result create_iter(const List *list, ListIter **iter)
{
    assert(list != NULL);
    assert(iter != NULL);

    Result result = SUCCESS;
    ListIter *n_iter = (ListIter *)calloc(sizeof(ListIter), 1u);

    if (n_iter == NULL)
    {
        // Failed to allocate iter
        result = FAILED;
        return result;
    }
    reset_iter(list, &n_iter);
    // user pointer assign new iter
    *iter = n_iter;
    return result;
}

void destroy_iter(ListIter *iter)
{
    free(iter);
}

void next_node(ListIter **iter)
{
    assert(*iter != NULL);
    (*iter)->node = (*iter)->node->next;
}

void reset_iter(const List *list, ListIter **iter)
{
    assert(*iter != NULL);
    (*iter)->node = list->head->next;
}

bool is_iter_end(ListIter *iter)
{
    assert(iter != NULL);

    return iter->node == NULL;
}

void *iter_value(const ListIter *iter)
{
    assert(iter != NULL);
    return iter->node->value;
}
