/* See LICENSE for license details */

/*

Module: feed_suggest.c

Description:

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_list.h"

#include "feed_suggest.h"

#include "feed_object.h"

#include "feed_client.h"

#include "feed_heap.h"

static
char
feed_suggest_node_init(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_suggest_node *
        p_suggest_node;

    struct feed_suggest_descriptor const *
        p_suggest_descriptor;

    p_suggest_node =
        (struct feed_suggest_node *)(
            p_object);

    p_suggest_descriptor =
        (struct feed_suggest_descriptor const *)(
            p_descriptor);

    feed_list_init(
        &(
            p_suggest_node->o_list));

    p_suggest_node->p_client =
        p_client;

    if (p_suggest_descriptor->i_length)
    {
        p_suggest_node->p_buffer =
            (unsigned char *)(
                feed_heap_alloc(
                    p_client->p_heap,
                    p_suggest_descriptor->i_length));

        if (p_suggest_node->p_buffer)
        {
            memcpy(
                p_suggest_node->p_buffer,
                p_suggest_descriptor->p_buffer,
                p_suggest_descriptor->i_length);

            p_suggest_node->i_length =
                p_suggest_descriptor->i_length;

        }
        else
        {
            p_suggest_node->i_length =
                0ul;
        }
    }
    else
    {
        p_suggest_node->p_buffer =
            (unsigned char *)(
                0);

        p_suggest_node->i_length =
            0ul;
    }

    p_suggest_node->i_cursor_offset =
        p_suggest_descriptor->i_cursor_offset;

    b_result =
        1;

    return
        b_result;

} /* feed_suggest_node_init() */

struct feed_suggest_node *
feed_suggest_node_create(
    struct feed_client * const
        p_client,
    struct feed_suggest_descriptor const * const
        p_suggest_descriptor)
{
    struct feed_suggest_node *
        p_suggest_node;

    p_suggest_node =
        (struct feed_suggest_node *)(
            feed_object_create(
                p_client,
                sizeof(
                    struct feed_suggest_node),
                &(
                    feed_suggest_node_init),
                p_suggest_descriptor));

    return
        p_suggest_node;

} /* feed_suggest_node_create() */

static
void
feed_suggest_node_cleanup(
    void * const
        p_object)
{
    struct feed_suggest_node *
        p_suggest_node;

    p_suggest_node =
        (struct feed_suggest_node *)(
            p_object);

    feed_list_join(
        &(
            p_suggest_node->o_list),
        &(
            p_suggest_node->o_list));

    if (
        p_suggest_node->p_buffer)
    {
        feed_heap_free(
            p_suggest_node->p_client->p_heap,
            (void *)(
                p_suggest_node->p_buffer));

        p_suggest_node->p_buffer =
            (unsigned char *)(
                0);
    }

} /* feed_suggest_node_cleanup() */

void
feed_suggest_node_destroy(
    struct feed_suggest_node * const
        p_suggest_node)
{
    feed_object_destroy(
        p_suggest_node->p_client,
        p_suggest_node,
        &(
            feed_suggest_node_cleanup));

} /* feed_suggest_node_destroy() */

char
feed_suggest_list_init(
    struct feed_client * const
        p_client,
    struct feed_suggest_list * const
        p_suggest_list)
{
    char
        b_result;

    (void)(
        p_client);

    feed_list_init(
        &(
            p_suggest_list->o_nodes));

    b_result =
        1;

    return
        b_result;

} /* feed_suggest_list_init() */

void
feed_suggest_list_cleanup(
    struct feed_suggest_list * const
        p_suggest_list)
{
    feed_suggest_list_clear(
        p_suggest_list);

} /* feed_suggest_list_cleanup() */

void
feed_suggest_list_clear(
    struct feed_suggest_list * const
        p_suggest_list)
{
    char
        b_more;

    b_more =
        1;

    while (
        b_more)
    {
        struct feed_suggest_node *
            p_suggest_node;

        p_suggest_node =
            feed_suggest_list_first(
                p_suggest_list);

        if (
            p_suggest_node)
        {
            feed_suggest_node_destroy(
                p_suggest_node);

            p_suggest_list->i_count --;
        }
        else
        {
            b_more =
                0;
        }
    }

} /* feed_suggest_list_clear() */

char
feed_suggest_list_append(
    struct feed_suggest_list * const
        p_suggest_list,
    struct feed_suggest_node * const
        p_suggest_node)
{
    char
        b_result;

    feed_list_join(
        &(
            p_suggest_node->o_list),
        &(
            p_suggest_list->o_nodes));

    p_suggest_list->i_count ++;

    b_result =
        1;

    return
        b_result;

} /* feed_suggest_list_append() */

struct feed_suggest_node *
feed_suggest_list_first(
    struct feed_suggest_list * const
        p_suggest_list)
{
    struct feed_suggest_node *
        p_suggest_node;

    if (p_suggest_list->o_nodes.p_next != &(p_suggest_list->o_nodes))
    {
        p_suggest_node =
            (struct feed_suggest_node *)(
                p_suggest_list->o_nodes.p_next);
    }
    else
    {
        p_suggest_node =
            (struct feed_suggest_node *)(
                0);
    }

    return
        p_suggest_node;

} /* feed_suggest_list_first() */

struct feed_suggest_node *
feed_suggest_list_next(
    struct feed_suggest_list * const
        p_suggest_list,
    struct feed_suggest_node * const
        p_suggest_node)
{
    struct feed_suggest_node *
        p_next;

    if (p_suggest_node)
    {
        if (p_suggest_node->o_list.p_next != &(p_suggest_list->o_nodes))
        {
            p_next =
                (struct feed_suggest_node *)(
                    p_suggest_node->o_list.p_next);
        }
        else
        {
            p_next =
                (struct feed_suggest_node *)(
                    0);
        }
    }
    else
    {
        p_next =
            (struct feed_suggest_node *)(
                0);
    }

    return
        p_next;

} /* feed_suggest_list_next() */

struct feed_suggest_node *
feed_suggest_list_prev(
    struct feed_suggest_list * const
        p_suggest_list,
    struct feed_suggest_node * const
        p_suggest_node)
{
    struct feed_suggest_node *
        p_prev;

    if (p_suggest_node)
    {
        if (p_suggest_node->o_list.p_prev != &(p_suggest_list->o_nodes))
        {
            p_prev =
                (struct feed_suggest_node *)(
                    p_suggest_node->o_list.p_prev);
        }
        else
        {
            p_prev =
                (struct feed_suggest_node *)(
                    0);
        }
    }
    else
    {
        p_prev =
            (struct feed_suggest_node *)(
                0);
    }

    return
        p_prev;

} /* feed_suggest_list_prev() */

/* end-of-file: feed_suggest.c */
