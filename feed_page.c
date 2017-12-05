/* See LICENSE for license details */

/*

Module: feed_page.c

Description:

    History of visited pages.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_prompt_iterator.h"

#include "feed_text_iterator.h"

#include "feed_screen_iterator.h"

#include "feed_view.h"

#include "feed_list.h"

#include "feed_page.h"

#include "feed_client.h"

#include "feed_heap.h"

void
feed_page_init(
    struct feed_page * const
        p_page,
    struct feed_client * const
        p_client)
{
    p_page->p_client =
        p_client;

    feed_list_init(
        &(
            p_page->o_pages));

    p_page->i_page_count =
        0ul;

}

void
feed_page_cleanup(
    struct feed_page * const
        p_page)
{
    feed_page_reset(
        p_page);

}

void
feed_page_reset(
    struct feed_page * const
        p_page)
{
    struct feed_view_descriptor
        o_view_descriptor;

    while (
        feed_page_pop(
            p_page,
            &(
                o_view_descriptor)));
}

void
feed_page_push(
    struct feed_page * const
        p_page,
    struct feed_view_descriptor const * const
        p_view_descriptor)
{
    struct feed_page_node *
        p_page_node;

    p_page_node =
        (struct feed_page_node *)(
            feed_heap_alloc(
                p_page->p_client->p_heap,
                (unsigned long int)(
                    sizeof(
                        struct feed_page_node))));

    if (
        p_page_node)
    {
        feed_list_init(
            &(
                p_page_node->o_list));

        p_page_node->o_view_descriptor =
            *(
                p_view_descriptor);

        feed_list_join(
            &(
                p_page_node->o_list),
            &(
                p_page->o_pages));

        p_page->i_page_count ++;
    }
}

char
feed_page_pop(
    struct feed_page * const
        p_page,
    struct feed_view_descriptor * const
        p_view_descriptor)
{
    char
        b_result;

    if (
        p_page->i_page_count)
    {
        struct feed_page_node *
            p_page_node;

        p_page_node =
            (struct feed_page_node *)(
                p_page->o_pages.p_prev);

        feed_list_join(
            &(
                p_page_node->o_list),
            &(
                p_page_node->o_list));

        *(
            p_view_descriptor) =
            p_page_node->o_view_descriptor;

        feed_heap_free(
            p_page->p_client->p_heap,
            (void *)(
                p_page_node));

        p_page->i_page_count --;

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

