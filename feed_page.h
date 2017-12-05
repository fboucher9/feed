/* See LICENSE for license details */

/*

Module: feed_page.h

Description:

    History of visited pages.

*/

#if defined(INC_FEED_PAGE_H)
#error include feed_page.h once
#endif /* #if defined(INC_FEED_PAGE_H) */

#define INC_FEED_PAGE_H

struct feed_page_node
{
    struct feed_list
        o_list;

    struct feed_view_descriptor
        o_view_descriptor;

};

struct feed_page_history
{
    struct feed_client *
        p_client;

    struct feed_list
        o_pages;

    unsigned long int
        i_page_count;

};

void
feed_page_init(
    struct feed_page_history * const
        p_page,
    struct feed_client * const
        p_client);

void
feed_page_cleanup(
    struct feed_page_history * const
        p_page);

void
feed_page_reset(
    struct feed_page_history * const
        p_page);

void
feed_page_push(
    struct feed_page_history * const
        p_page,
    struct feed_view_descriptor const * const
        p_view_descriptor);

char
feed_page_pop(
    struct feed_page_history * const
        p_page,
    struct feed_view_descriptor * const
        p_view_descriptor);

/* end-of-file: feed_page.h */
