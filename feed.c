/* See LICENSE for license details */

/*

Module: feed.c

Description:

    Implementation of feed library API.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed.h"

#include "feed_client.h"

#include "feed_heap.h"

#include "feed_input.h"

#include "feed_keys.h"

#include "feed_prompt_iterator.h"

#include "feed_prompt.h"

#include "feed_screen.h"

#include "feed_screen_info.h"

#include "feed_screen_iterator.h"

#include "feed_list.h"

#include "feed_utf8.h"

#include "feed_glyph.h"

#include "feed_line.h"

#include "feed_text.h"

#include "feed_text_iterator.h"

#include "feed_buf.h"

#include "feed_object.h"

#include "feed_suggest.h"

#include "feed_view.h"

struct feed_handle
{
    /* -- */

    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    /* -- */

    struct feed_prompt *
        p_prompt;

    struct feed_input *
        p_input;

    /* -- */

    struct feed_screen *
        p_screen;

    struct feed_text *
        p_text;

    /* -- */

    struct feed_line *
        p_page_line;

    struct feed_screen_info *
        p_screen_info;

    /* -- */

    struct feed_suggest_node *
        p_suggest_node;

    void *
        pv_padding[1u];

    /* -- */

    struct feed_text_iterator
        o_cursor;

    struct feed_text_iterator
        o_suggest_iterator;

    struct feed_screen_iterator
        o_cursor_visible;

    /* -- */

    struct feed_descriptor
        o_descriptor;

    struct feed_client
        o_client;

    struct feed_screen_info
        o_screen_info;

    struct feed_suggest_list
        o_suggest_list;

    /* -- */

    /* Page */
    unsigned long int
        i_page_line_index;

    unsigned long int
        i_page_glyph_index;

    /* -- */

    unsigned long int
        i_final_line_index;

    unsigned long int
        i_final_glyph_index;

    /* -- */

    unsigned long int
        i_suggest_offset;

    unsigned long int
        i_suggest_length;

    /* -- */

    unsigned long int
        i_final_cursor_address;

    unsigned long int
        ul_padding[1u];

    /* -- */

    enum feed_view_state
        e_final_state;

    enum feed_view_state
        e_page_state;

    unsigned int
        ui_padding[2u];

    /* -- */

    char
        b_started;

    char
        b_verbose;

    char
        b_refresh_cursor;

    char
        b_refresh_text;

    char
        b_suggest;

    unsigned char
        uc_padding[3u];

}; /* struct feed_handle */

static
char
feed_init(
    struct feed_handle * const
        p_this,
    struct feed_heap * const
        p_heap,
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    char
        b_result;

    memset(
        p_this,
        0x00u,
        sizeof(
            *(
                p_this)));

    p_this->p_client =
        &(
            p_this->o_client);

    feed_client_init(
        p_this->p_client);

    p_this->p_heap =
        p_heap;

    p_this->o_client.p_heap =
        p_heap;

    p_this->o_descriptor =
        *(
            p_feed_descriptor);

    p_this->p_prompt =
        feed_prompt_create(
            p_this->p_client);

    p_this->o_client.p_prompt =
        p_this->p_prompt;

    p_this->p_input =
        feed_input_create(
            p_this->p_client);

    p_this->p_screen_info =
        &(
            p_this->o_screen_info);

    feed_screen_info_init(
        p_this->p_screen_info);

    p_this->p_client->p_screen_info =
        p_this->p_screen_info;

    p_this->p_screen =
        feed_screen_create(
            p_this->p_client);

    p_this->p_text =
        feed_text_create(
            p_this->p_client);

    p_this->o_client.p_text =
        p_this->p_text;

    feed_text_iterator_init(
        p_this->p_text,
        &(
            p_this->o_cursor));

    feed_suggest_list_init(
        p_this->p_client,
        &(
            p_this->o_suggest_list));

    p_this->p_suggest_node =
        (struct feed_suggest_node *)(
            0);

    p_this->b_suggest =
        0;

    b_result =
        1;

    return
        b_result;

} /* feed_init() */

static
void
feed_cleanup(
    struct feed_handle * const
        p_this)
{
    feed_suggest_list_cleanup(
        &(
            p_this->o_suggest_list));

    feed_text_iterator_cleanup(
        p_this->p_text,
        &(
            p_this->o_cursor));

    if (
        p_this->p_screen_info)
    {
        feed_screen_info_cleanup(
            p_this->p_screen_info);

        p_this->p_screen_info =
            (struct feed_screen_info *)(
                0);
    }

    if (
        p_this->p_text)
    {
        feed_text_destroy(
            p_this->p_text);

        p_this->p_text =
            (struct feed_text *)(
                0);
    }

    if (
        p_this->p_screen)
    {
        feed_screen_destroy(
            p_this->p_screen);

        p_this->p_screen =
            (struct feed_screen *)(
                0);
    }

    if (
        p_this->p_input)
    {
        feed_input_destroy(
            p_this->p_input);

        p_this->p_input =
            (struct feed_input *)(
                0);
    }

    if (
        p_this->p_prompt)
    {
        feed_prompt_destroy(
            p_this->p_prompt);

        p_this->p_prompt =
            (struct feed_prompt *)(
                0);
    }

    if (
        p_this->p_client)
    {
        feed_client_cleanup(
            p_this->p_client);

        p_this->p_client =
            (struct feed_client *)(
                0);
    }

} /* feed_cleanup() */

/*

Function: feed_create

Description:

    Create a feed handle and register application callbacks.  Initialize
    all objects used by feed library.

*/
struct feed_handle *
feed_create(
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    struct feed_handle *
        p_this;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_heap_create();

    if (
        p_heap)
    {
        p_this =
            (struct feed_handle *)(
                feed_heap_alloc(
                    p_heap,
                    sizeof(
                        struct feed_handle)));

        if (
            p_this)
        {
            if (
                feed_init(
                    p_this,
                    p_heap,
                    p_feed_descriptor))
            {
            }
            else
            {
                feed_heap_free(
                    p_heap,
                    (void *)(
                        p_this));

                p_this =
                    (struct feed_handle *)(
                        0);
            }
        }

        if (
            !(p_this))
        {
            feed_heap_destroy(
                p_heap);
        }
    }

    return
        p_this;

} /* feed_create() */

void
feed_destroy(
    struct feed_handle * const
        p_this)
{
    if (
        p_this)
    {
        struct feed_heap *
            p_heap;

        p_heap =
            p_this->p_heap;

        if (
            p_heap)
        {
            feed_cleanup(
                p_this);

            p_this->p_heap =
                (struct feed_heap *)(
                    0);

            feed_heap_free(
                p_heap,
                p_this);

            feed_heap_destroy(
                p_heap);
        }
    }

} /* feed_destroy() */

int
feed_prompt1(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            feed_prompt_set1(
                p_this->p_prompt,
                p_data,
                i_data_length))
        {
            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_prompt1() */

int
feed_prompt2(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            feed_prompt_set2(
                p_this->p_prompt,
                p_data,
                i_data_length))
        {
            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_prompt2() */

int
feed_load(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    if (
        feed_text_load(
            p_this->p_text,
            p_data,
            i_data_length))
    {
        i_result =
            0;
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_load() */

static
char
feed_main_move_cursor_xy(
    struct feed_handle * const
        p_this,
    unsigned long int const
        i_cursor_address,
    unsigned long int const
        i_start_address,
    unsigned long int const
        i_stop_address)
{
    char
        b_found;

    char
        b_found_after;

    struct feed_view
        o_iterator;

    b_found =
        0;

    b_found_after =
        0;

    feed_view_init(
        &(
            o_iterator),
        p_this->p_client);

    if (
        feed_view_head(
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        b_more =
            1;

        while (
            b_more)
        {
            /* Locate a glyph that is at same coord as cursor */
            if (
                (o_iterator.o_screen_iterator.i_cursor_address >= i_start_address)
                && (o_iterator.o_screen_iterator.i_cursor_address < i_stop_address)
                && (o_iterator.e_state != feed_view_state_prompt))
            {
                if (
                    o_iterator.o_screen_iterator.i_cursor_address < i_cursor_address)
                {
                    p_this->o_cursor.i_line_index =
                        o_iterator.o_text_iterator.i_line_index;

                    p_this->o_cursor.p_line =
                        o_iterator.o_text_iterator.p_line;

                    p_this->o_cursor.i_glyph_index =
                        o_iterator.o_text_iterator.i_glyph_index;

                    p_this->o_cursor.p_glyph =
                        o_iterator.o_text_iterator.p_glyph;

                    b_more =
                        1;

                    b_found =
                        1;
                }
                else if (
                    o_iterator.o_screen_iterator.i_cursor_address == i_cursor_address)
                {
                    p_this->o_cursor.i_line_index =
                        o_iterator.o_text_iterator.i_line_index;

                    p_this->o_cursor.p_line =
                        o_iterator.o_text_iterator.p_line;

                    p_this->o_cursor.i_glyph_index =
                        o_iterator.o_text_iterator.i_glyph_index;

                    p_this->o_cursor.p_glyph =
                        o_iterator.o_text_iterator.p_glyph;

                    b_more =
                        0;

                    b_found =
                        1;
                }
                else
                {
                    if (!b_found_after)
                    {
                        p_this->o_cursor.i_line_index =
                            o_iterator.o_text_iterator.i_line_index;

                        p_this->o_cursor.p_line =
                            o_iterator.o_text_iterator.p_line;

                        p_this->o_cursor.i_glyph_index =
                            o_iterator.o_text_iterator.i_glyph_index;

                        p_this->o_cursor.p_glyph =
                            o_iterator.o_text_iterator.p_glyph;

                        b_found_after =
                            1;
                    }

                    b_more =
                        0;

                    b_found =
                        1;
                }
            }

            if (b_more)
            {
                b_more =
                    feed_view_next(
                        &(
                            o_iterator));
            }
        }
    }

    feed_view_cleanup(
        &(
            o_iterator));

    if (b_found)
    {
        p_this->b_refresh_cursor =
            1;
    }

    return b_found;
}

static
char
feed_main_latch_next_page(
    struct feed_handle * const
        p_this)
{
    char
        b_result;

    if (p_this->e_final_state != feed_view_state_null)
    {
        p_this->p_page_line = NULL;

        p_this->i_page_line_index = p_this->i_final_line_index;

        p_this->i_page_glyph_index = p_this->i_final_glyph_index;

        if (p_this->e_final_state == feed_view_state_prompt)
        {
            p_this->e_page_state = feed_view_state_prompt;
        }
        else
        {
            p_this->e_page_state = feed_view_state_text;
        }

        p_this->b_refresh_text = 1;

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

static
char
feed_main_move_cursor_down(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (p_this->o_cursor_visible.i_cursor_address < (p_this->p_screen_info->i_screen_size - p_this->p_screen_info->i_screen_width))
    {
        b_found =
            feed_main_move_cursor_xy(
                p_this,
                p_this->o_cursor_visible.i_cursor_address + p_this->p_screen_info->i_screen_width,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                + p_this->p_screen_info->i_screen_width,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                + p_this->p_screen_info->i_screen_width
                + p_this->p_screen_info->i_screen_width);

        if (!b_found)
        {
            b_found =
                feed_main_move_cursor_xy(
                    p_this,
                    p_this->o_cursor_visible.i_cursor_address + p_this->p_screen_info->i_screen_width,
                    p_this->o_cursor_visible.i_cursor_address
                    - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                    + p_this->p_screen_info->i_screen_width,
                    p_this->p_screen_info->i_screen_size);
        }
    }

    if (!b_found)
    {
        if (feed_main_latch_next_page(p_this))
        {
            b_found =
                feed_main_move_cursor_xy(
                    p_this,
                    p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width,
                    0,
                    p_this->p_screen_info->i_screen_width);

            if (!b_found)
            {
                b_found =
                    feed_main_move_cursor_xy(
                        p_this,
                        p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width,
                        0,
                        p_this->p_screen_info->i_screen_size);
            }
        }
    }

    return b_found;
}

static
char
feed_main_scroll_pageup(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    struct feed_view
        o_iterator;

    feed_view_init(
        &(
            o_iterator),
        p_this->p_client);

    if (
        feed_view_tail(
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        char
            b_reach_top;

        struct feed_line *
            p_final_line;

        unsigned long int
            i_final_line_index;

        unsigned long int
            i_final_glyph_index;

        enum feed_view_state
            e_final_state;

        b_more =
            1;

        b_reach_top =
            0;

        if (0u == o_iterator.o_text_iterator.i_glyph_index)
        {
            feed_view_prev(
                &(
                    o_iterator));
        }

        while (
            b_more
            && (!b_reach_top))
        {
            struct feed_glyph *
                p_glyph;

            p_glyph =
                (feed_view_state_prompt == o_iterator.e_state)
                ? o_iterator.o_prompt_iterator.p_glyph
                : o_iterator.o_text_iterator.p_glyph;

            /* Remember this as a valid glyph */

            if (p_glyph)
            {
                if (p_glyph->o_utf8_code.i_raw_len)
                {
                    if (
                        '\n' != p_glyph->o_utf8_code.a_raw[0u])
                    {
                        p_final_line =
                            o_iterator.o_text_iterator.p_line;

                        i_final_line_index =
                            o_iterator.o_text_iterator.i_line_index;

                        i_final_glyph_index =
                            (feed_view_state_prompt == o_iterator.e_state)
                            ? o_iterator.o_prompt_iterator.i_glyph_index
                            : o_iterator.o_text_iterator.i_glyph_index;

                        e_final_state =
                            o_iterator.e_state;

                    }
                    else
                    {
                        p_final_line =
                            o_iterator.o_text_iterator.p_line;

                        i_final_line_index =
                            o_iterator.o_text_iterator.i_line_index;

                        i_final_glyph_index =
                            0u;

                        e_final_state =
                            feed_view_state_prompt;
                    }
                }
                else
                {
                    p_final_line =
                        o_iterator.o_text_iterator.p_line;

                    i_final_line_index =
                        o_iterator.o_text_iterator.i_line_index;

                    i_final_glyph_index =
                        0u;

                    e_final_state =
                        feed_view_state_prompt;
                }

                b_found =
                    1;
            }

            if (
                feed_view_prev(
                    &(
                        o_iterator)))
            {
            }
            else
            {
                if (o_iterator.b_screen_full)
                {
                }
                else
                {
                    b_reach_top =
                        1;
                }

                b_more =
                    0;
            }
        }

        if (b_found)
        {
            p_this->p_page_line =
                p_final_line;

            p_this->i_page_line_index =
                i_final_line_index;

            p_this->i_page_glyph_index =
                i_final_glyph_index;

            p_this->e_page_state =
                e_final_state;

            p_this->b_refresh_text = 1;

            if (b_reach_top)
            {
                b_found =
                    0;
            }
        }
    }
    else
    {
        b_found =
            0;
    }

    feed_view_cleanup(
        &(
            o_iterator));

    return
        b_found;

}

static
char
feed_main_move_cursor_up(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    /* try a misaligned up in same visible page */
    if (p_this->o_cursor_visible.i_cursor_address >= p_this->p_screen_info->i_screen_width)
    {
        b_found =
            feed_main_move_cursor_xy(
                p_this,
                p_this->o_cursor_visible.i_cursor_address
                - p_this->p_screen_info->i_screen_width,
                0,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width));
    }

    if (!b_found)
    {
        if (p_this->i_page_line_index
            || p_this->i_page_glyph_index)
        {
            if (
                feed_main_scroll_pageup(
                    p_this))
            {
                /* up with aligned page */
                b_found =
                    feed_main_move_cursor_xy(
                        p_this,
                        p_this->p_screen_info->i_screen_size
                        - p_this->p_screen_info->i_screen_width
                        + (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
                        0,
                        p_this->p_screen_info->i_screen_size);

            }
        }
    }

    return b_found;
}

/*

Function: feed_main_move_page_prev

Description:

    Search for page that is previous to page which holds cursor.

Comments:

    -   Problem may occur when current page is not aligned on a page
        boundary.  Perhaps first step is to align the current page,
        then to search for previous page.

    -   Concept of x,y relative to page is not valid when page is not
        aligned.

    -   We need to remember last N lines so that we may scroll by one
        line at a time.

            [C-N] line_index, glyph_index, state
            [C-1] line_index, glyph_index, state
            [C-0] etc.

    -   At each iteration, if screen y changes, then add a new entry into
        the cached line table.

*/
static
char
feed_main_move_page_prev(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (p_this->i_page_line_index
        || p_this->i_page_glyph_index)
    {
        if (
            feed_main_scroll_pageup(
                p_this))
        {
            /* Find cursor at same position */
            b_found =
                feed_main_move_cursor_xy(
                    p_this,
                    p_this->o_cursor_visible.i_cursor_address,
                    0,
                    p_this->p_screen_info->i_screen_size);
        }
    }

    return b_found;
}

static
char
feed_main_move_page_next(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    if (feed_main_latch_next_page(p_this))
    {
        b_found =
            feed_main_move_cursor_xy(
                p_this,
                p_this->o_cursor_visible.i_cursor_address,
                0,
                p_this->p_screen_info->i_screen_size);
    }

    return b_found;
}

static
void
feed_main_refresh_job(
    struct feed_handle * const
        p_this)
{
    struct feed_view
        o_iterator;

    feed_view_init(
        &(
            o_iterator),
        p_this->p_client);

    p_this->o_cursor_visible.i_cursor_address =
        p_this->p_screen_info->i_screen_size;

    if (p_this->b_refresh_text)
    {
        feed_screen_update_info(
            p_this->p_screen,
            0);

        feed_screen_set_cursor_pos(
            p_this->p_screen,
            0ul);
    }

    if (!(p_this->p_page_line))
    {
        p_this->p_page_line =
            feed_text_get_line(
                p_this->p_text,
                p_this->i_page_line_index);
    }

    if (
        feed_view_head(
            &(o_iterator),
            p_this->p_page_line,
            p_this->i_page_line_index,
            p_this->i_page_glyph_index,
            p_this->e_page_state))
    {
        char
            b_more;

        b_more =
            1;

        while (
            b_more)
        {
            /* Detect if cursor is visible */
            if (
                (
                    p_this->o_cursor.i_line_index == o_iterator.o_text_iterator.i_line_index)
                && (
                    p_this->o_cursor.i_glyph_index == o_iterator.o_text_iterator.i_glyph_index)
                && (
                    (o_iterator.e_state != feed_view_state_prompt)))
            {
                p_this->o_cursor_visible.i_cursor_address =
                    o_iterator.o_screen_iterator.i_cursor_address;
            }

            if (p_this->b_refresh_text)
            {
                struct feed_glyph *
                    p_glyph;

                p_glyph =
                    (feed_view_state_prompt == o_iterator.e_state)
                    ? o_iterator.o_prompt_iterator.p_glyph
                    : o_iterator.o_text_iterator.p_glyph;

                if (p_glyph)
                {
                    if (p_glyph->o_utf8_code.i_raw_len)
                    {
                        if (
                            '\n' == p_glyph->o_utf8_code.a_raw[0u])
                        {
                            feed_screen_newline(
                                p_this->p_screen);
                        }
                        else if (
                            '\t' == p_glyph->o_utf8_code.a_raw[0u])
                        {
                            unsigned char
                                a_visible[15u];

                            unsigned char
                                i_visible_length;

                            i_visible_length =
                                (unsigned char)(
                                    8u
                                    - (
                                        (
                                            o_iterator.o_screen_iterator.i_cursor_address %
                                            p_this->p_screen_info->i_screen_width)
                                        & 7u));

                            memset(
                                a_visible,
                                ' ',
                                i_visible_length);

                            feed_screen_write(
                                p_this->p_screen,
                                a_visible,
                                i_visible_length);
                        }
                        else
                        {
                            unsigned char
                                a_visible[15u];

                            unsigned char
                                i_visible_length;

                            i_visible_length =
                                feed_glyph_render_visible(
                                    p_glyph,
                                    a_visible);

                            feed_screen_write(
                                p_this->p_screen,
                                a_visible,
                                i_visible_length);
                        }
                    }
                    else
                    {
                        feed_screen_clear_region(
                            p_this->p_screen);
                    }
                }
            }

            if (b_more)
            {
                b_more =
                    feed_view_next(
                        &(
                            o_iterator));
            }
        }

        p_this->i_final_line_index =
            o_iterator.o_text_iterator.i_line_index;

        p_this->i_final_glyph_index =
            (feed_view_state_prompt == o_iterator.e_state)
            ? o_iterator.o_prompt_iterator.i_glyph_index
            : o_iterator.o_text_iterator.i_glyph_index;

        p_this->e_final_state =
            o_iterator.e_state;

        if (p_this->b_refresh_text)
        {
            p_this->i_final_cursor_address =
                feed_screen_get_cursor_pos(
                    p_this->p_screen);
        }

        if (p_this->b_refresh_text || p_this->b_refresh_cursor)
        {
            if (p_this->o_cursor_visible.i_cursor_address < p_this->p_screen_info->i_screen_size)
            {
                feed_screen_set_cursor_pos(
                    p_this->p_screen,
                    p_this->o_cursor_visible.i_cursor_address);
            }
        }
    }

    feed_view_cleanup(
        &(
            o_iterator));

    p_this->b_refresh_text =
        0;

    p_this->b_refresh_cursor =
        0;

}

static
void
feed_main_insert_event(
    struct feed_handle * const
        p_this,
    struct feed_event const * const
        p_event)
{
    struct feed_utf8_parser
        o_utf8_parser;

    if (
        feed_utf8_parser_init(
            &(
                o_utf8_parser)))
    {
        unsigned char
            i_raw_iterator;

        for (
            i_raw_iterator = 0u;
            i_raw_iterator < p_event->i_raw_len;
            i_raw_iterator ++)
        {
            struct feed_utf8_code
                o_utf8_code;

            if (
                0 < feed_utf8_parser_write(
                    &(
                        o_utf8_parser),
                    p_event->a_raw[i_raw_iterator],
                    &(
                        o_utf8_code)))
            {
                struct feed_glyph *
                    p_glyph;

                p_glyph =
                    feed_glyph_create(
                        p_this->p_client,
                        &(
                            o_utf8_code));

                if (
                    p_glyph)
                {
                    unsigned char
                        i_width;

                    i_width =
                        feed_glyph_get_visible_width(
                            p_glyph);

                    feed_text_iterator_write_glyph(
                        p_this->p_text,
                        &(
                            p_this->o_cursor),
                        p_glyph);

                    /* Advance cursor visible position */
                    if ((p_this->o_cursor_visible.i_cursor_address + i_width + 1u) < p_this->p_screen_info->i_screen_size)
                    {
                        p_this->o_cursor_visible.i_cursor_address += i_width;
                    }
                    else
                    {
                        /* Change page */
                        p_this->o_cursor_visible.i_cursor_address = 0ul;

                        p_this->p_page_line = p_this->o_cursor.p_line;

                        p_this->i_page_line_index = p_this->o_cursor.i_line_index;

                        p_this->i_page_glyph_index = p_this->o_cursor.i_glyph_index;

                        p_this->e_page_state = feed_view_state_text;
                    }

                    p_this->b_refresh_text = 1;
                }
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));
    }
}

static
char
feed_main_detect_visible_cursor(
    struct feed_handle * const
        p_this)
{
    char b_result;

    b_result = 0;

    if (p_this->o_cursor.i_line_index < p_this->i_page_line_index)
    {
    }
    else if (p_this->o_cursor.i_line_index == p_this->i_page_line_index)
    {
        if ((p_this->o_cursor.i_glyph_index < p_this->i_page_glyph_index)
            && (feed_view_state_text == p_this->e_page_state))
        {
        }
        else
        {
            b_result = 1;
        }
    }
    else
    {
        b_result = 1;
    }

    if (b_result)
    {
        if (p_this->o_cursor.i_line_index < p_this->i_final_line_index)
        {
        }
        else if (p_this->o_cursor.i_line_index == p_this->i_final_line_index)
        {
            if ((p_this->o_cursor.i_glyph_index < p_this->i_final_glyph_index)
                && (p_this->e_final_state == feed_view_state_text))
            {
            }
            else
            {
                b_result = 0;
            }
        }
        else
        {
            b_result = 0;
        }
    }

    return
        b_result;

}

static
char
feed_main_move_cursor_left(
    struct feed_handle * const
        p_this)
{
    char
        b_result;

    if (
        feed_text_iterator_prev_glyph(
            p_this->p_text,
            &(
                p_this->o_cursor)))
    {
        b_result =
            1;
    }
    else
    {
        if (
            feed_text_iterator_prev_line(
                p_this->p_text,
                &(
                    p_this->o_cursor)))
        {
            feed_text_iterator_end_glyph(
                p_this->p_text,
                &(
                    p_this->o_cursor));

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }

    if (b_result)
    {
        if (
            feed_main_detect_visible_cursor(
                p_this))
        {
            p_this->b_refresh_cursor = 1;
        }
        else
        {
            feed_main_scroll_pageup(
                p_this);
        }
    }

    return
        b_result;

}

static
void
feed_main_delete_glyph_next(
    struct feed_handle * const
        p_this)
{
    /* Detect eof */
    feed_text_iterator_validate(
        p_this->p_text,
        &(
            p_this->o_cursor));

    if (
        (
            (p_this->o_cursor.i_line_index + 1u) < p_this->p_text->i_line_count)
        || (
            p_this->o_cursor.p_line
            && (
                (p_this->o_cursor.i_glyph_index + 1u) < p_this->o_cursor.p_line->i_glyph_count)))
    {
        struct feed_glyph *
            p_glyph;

        p_glyph =
            feed_text_iterator_remove_glyph(
                p_this->p_text,
                &(
                    p_this->o_cursor));

        if (
            p_glyph)
        {
            if (p_glyph->o_utf8_code.i_raw_len && ('\n' == p_glyph->o_utf8_code.a_raw[0u]))
            {
                /* Bring next line into this line */
                /* Delete next line */
                feed_text_iterator_join_lines(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));
            }

            feed_glyph_destroy(
                p_this->p_client,
                p_glyph);
        }

        p_this->b_refresh_text = 1;
    }
}

static
char
feed_main_move_cursor_right(
    struct feed_handle * const
        p_this)
{
    char
        b_result;

    if (feed_text_iterator_next_glyph(
            p_this->p_text,
            &(
                p_this->o_cursor)))
    {
        b_result =
            1;
    }
    else
    {
        if (feed_text_iterator_next_line(
            p_this->p_text,
            &(
                p_this->o_cursor)))
        {
            feed_text_iterator_home_glyph(
                p_this->p_text,
                &(
                    p_this->o_cursor));

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }

    if (b_result)
    {
        /* Adjust visible cursor position */
        if (feed_main_detect_visible_cursor( p_this))
        {
            p_this->b_refresh_cursor =
                1;
        }
        else
        {
            feed_main_latch_next_page(p_this);
        }
    }

    return
        b_result;

}

static
void
feed_main_move_word_left(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    while (
        !b_found)
    {
        if (feed_main_move_cursor_left(p_this))
        {
            feed_text_iterator_validate(
                p_this->p_text,
                &(
                    p_this->o_cursor));

            if (p_this->o_cursor.p_glyph)
            {
                if ((0u == p_this->o_cursor.p_glyph->o_utf8_code.i_raw_len)
                    || (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                    || ('\t' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                    || ('\n' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u]))
                {
                }
                else
                {
                    b_found = 1;
                }
            }
            else
            {
            }
        }
        else
        {
            break;
        }
    }

    if (b_found)
    {
        b_found =
            0;

        while (
            !b_found)
        {
            if (feed_main_move_cursor_left(p_this))
            {
                feed_text_iterator_validate(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                if (p_this->o_cursor.p_glyph)
                {
                    if ((0u == p_this->o_cursor.p_glyph->o_utf8_code.i_raw_len)
                        || (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        || ('\t' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        || ('\n' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u]))
                    {
                        b_found =
                            1;
                    }
                }
                else
                {
                    b_found =
                        1;
                }
            }
            else
            {
                break;
            }
        }

        if (b_found)
        {
            feed_main_move_cursor_right(
                p_this);
        }
    }
}

static
void
feed_main_move_word_right(
    struct feed_handle * const
        p_this)
{
    char
        b_found;

    b_found =
        0;

    while (
        !b_found)
    {
        feed_text_iterator_validate(
            p_this->p_text,
            &(
                p_this->o_cursor));

        if (p_this->o_cursor.p_glyph)
        {
            if ((0u == p_this->o_cursor.p_glyph->o_utf8_code.i_raw_len)
                || (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                || ('\t' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                || ('\n' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u]))
            {
                b_found = 1;
            }
        }
        else
        {
            b_found = 1;
        }

        if (!b_found)
        {
            if (feed_main_move_cursor_right(p_this))
            {
            }
            else
            {
                break;
            }
        }
    }

    if (b_found)
    {
        b_found =
            0;

        while (
            !b_found)
        {
            if (feed_main_move_cursor_right(p_this))
            {
                feed_text_iterator_validate(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                if (p_this->o_cursor.p_glyph)
                {
                    if ((0u == p_this->o_cursor.p_glyph->o_utf8_code.i_raw_len)
                        || (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        || ('\t' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                        || ('\n' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u]))
                    {
                    }
                    else
                    {
                        b_found =
                            1;
                    }
                }
                else
                {
                }
            }
            else
            {
                break;
            }
        }
    }
}

#if 0 /* need to define a hotkey... */
static
void
feed_main_move_cursor_home(
    struct feed_handle * const
        p_this)
{
    feed_text_iterator_home_glyph(
        p_this->p_text,
        &(
            p_this->o_cursor));

    /* detect that cursor is still visible? */
    if (feed_main_detect_visible_cursor(
        p_this))
    {
        p_this->b_refresh_cursor = 1;
    }
    else
    {
        feed_main_scroll_pageup(
            p_this);
    }
}
#endif

static
void
feed_main_move_cursor_x0(
    struct feed_handle * const
        p_this)
{
    if (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
    {
        if (
            feed_main_move_cursor_xy(
                p_this,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                + p_this->p_screen_info->i_screen_width))
        {
            /* detect that cursor is still visible? */
        }
    }
}

static
void
feed_main_insert_newline(
    struct feed_handle * const
        p_this)
{
    /* Notify currently accumulated lines */

    /* Split current line */

    /* Create a new line */
    if (
        feed_text_iterator_insert_newline(
            p_this->p_text,
            &(
                p_this->o_cursor)))
    {
        /* Detect that cursor is visible? */
        if ((p_this->o_cursor_visible.i_cursor_address + p_this->p_screen_info->i_screen_width) >= p_this->p_screen_info->i_screen_size)
        {
            p_this->p_page_line = p_this->o_cursor.p_line;

            p_this->i_page_line_index = p_this->o_cursor.i_line_index;

            p_this->i_page_glyph_index = p_this->o_cursor.i_glyph_index;

            p_this->e_page_state = feed_view_state_prompt;
        }

        p_this->b_refresh_text =
            1;
    }
}

#if 0 /* need to define a hotkey ... */
static
void
feed_main_move_cursor_end(
    struct feed_handle * const
        p_this)
{
    feed_text_iterator_end_glyph(
        p_this->p_text,
        &(
            p_this->o_cursor));

    /* detect that cursor is still visible */
    if (feed_main_detect_visible_cursor(
        p_this))
    {
        p_this->b_refresh_cursor = 1;
    }
    else
    {
        feed_main_latch_next_page(p_this);
    }
}
#endif

static
void
feed_main_move_cursor_x1(
    struct feed_handle * const
        p_this)
{
    if ((p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width) < (p_this->p_screen_info->i_screen_width - 1u))
    {
        if (
            feed_main_move_cursor_xy(
                p_this,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                + p_this->p_screen_info->i_screen_width - 1u,
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
                p_this->o_cursor_visible.i_cursor_address
                - (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width)
                + p_this->p_screen_info->i_screen_width))
        {
        }
        else
        {
        }
    }
}

static
void
feed_main_move_page_home(
    struct feed_handle * const
        p_this)
{
    if (
        p_this->o_cursor.i_line_index
        || p_this->o_cursor.i_glyph_index)
    {
        p_this->p_page_line = NULL;

        p_this->i_page_line_index = 0ul;

        p_this->i_page_glyph_index = 0ul;

        p_this->e_page_state = feed_view_state_prompt;

        feed_text_iterator_set_line(
            p_this->p_text,
            &(
                p_this->o_cursor),
            p_this->i_page_line_index);

        feed_text_iterator_set_glyph(
            p_this->p_text,
            &(
                p_this->o_cursor),
            p_this->i_page_glyph_index);

        p_this->b_refresh_text = 1;
    }
}

static
void
feed_main_move_page_end(
    struct feed_handle * const
        p_this)
{
    p_this->p_page_line = NULL;

    p_this->i_page_line_index = p_this->p_text->i_line_count - 1u;

    p_this->i_page_glyph_index = 0ul;

    p_this->e_page_state = feed_view_state_prompt;

    feed_text_iterator_set_line(
        p_this->p_text,
        &(
            p_this->o_cursor),
        p_this->i_page_line_index);

    feed_text_iterator_set_glyph(
        p_this->p_text,
        &(
            p_this->o_cursor),
        p_this->i_page_glyph_index);

    p_this->b_refresh_text = 1;
}

static
void
feed_main_delete_word_prev(
    struct feed_handle * const
        p_this)
{
    if (p_this->o_cursor.i_line_index
        || p_this->o_cursor.i_glyph_index)
    {
        char
            b_found;

        b_found =
            0;

        while (
            !b_found)
        {
            if (feed_main_move_cursor_left(p_this))
            {
                feed_text_iterator_validate(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                if (p_this->o_cursor.p_glyph)
                {
                    if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                    {
                        feed_main_delete_glyph_next(p_this);
                    }
                    else
                    {
                        b_found = 1;
                    }
                }
                else
                {
                    feed_main_delete_glyph_next(p_this);
                }
            }
            else
            {
                break;
            }
        }

        if (b_found)
        {
            b_found =
                0;

            while (
                !b_found)
            {
                feed_text_iterator_validate(
                    p_this->p_text,
                    &(
                        p_this->o_cursor));

                if (p_this->o_cursor.p_glyph)
                {
                    if (' ' == p_this->o_cursor.p_glyph->o_utf8_code.a_raw[0u])
                    {
                        b_found =
                            1;
                    }
                    else
                    {
                        feed_main_delete_glyph_next(p_this);

                        if (feed_main_move_cursor_left(p_this))
                        {
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    b_found =
                        1;
                }
            }

            if (b_found)
            {
                feed_main_move_cursor_right(
                    p_this);
            }
        }
    }
}

static
void
feed_main_delete_glyph_prev(
    struct feed_handle * const
        p_this)
{
    if (p_this->o_cursor.i_line_index
        || p_this->o_cursor.i_glyph_index)
    {
        if (feed_main_move_cursor_left(p_this))
        {
            feed_main_delete_glyph_next(p_this);
        }
    }
}

static
void
feed_main_move_cursor_top(
    struct feed_handle * const
        p_this)
{
    feed_main_move_cursor_xy(
        p_this,
        (unsigned long int)(
            p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
        0,
        p_this->p_screen_info->i_screen_size);
}

static
void
feed_main_move_cursor_bottom(
    struct feed_handle * const
        p_this)
{
    feed_main_move_cursor_xy(
        p_this,
        p_this->p_screen_info->i_screen_size
        - p_this->p_screen_info->i_screen_width
        + (p_this->o_cursor_visible.i_cursor_address % p_this->p_screen_info->i_screen_width),
        0,
        p_this->p_screen_info->i_screen_size);
}

static
void
feed_main_suggest_node(
    struct feed_handle * const
        p_this,
    struct feed_suggest_node * const
        p_suggest_node)
{
    /* For suggestion change: recalculate page if page is part
    of modified range */

    p_this->p_page_line = NULL;

    p_this->i_page_line_index = 0ul;

    p_this->i_page_glyph_index = 0ul;

    p_this->e_page_state = feed_view_state_prompt;

    p_this->o_cursor.p_line = NULL;

    p_this->o_cursor.p_glyph = NULL;

    p_this->o_cursor.i_line_index = 0ul;

    p_this->o_cursor.i_glyph_index = 0ul;

    if (
        p_this->p_suggest_node)
    {
        struct feed_text_iterator
            o_text_iterator;

        o_text_iterator =
            p_this->o_suggest_iterator;

        feed_text_iterator_delete_region(
            p_this->p_text,
            &(
                o_text_iterator),
            p_this->i_suggest_length);
    }

    if (p_suggest_node)
    {
        p_this->p_suggest_node =
            p_suggest_node;

        p_this->i_suggest_length =
            p_suggest_node->i_length;

        {
            struct feed_text_iterator
                o_text_iterator;

            o_text_iterator =
                p_this->o_suggest_iterator;

            feed_text_iterator_load(
                p_this->p_text,
                &(
                    o_text_iterator),
                p_this->p_suggest_node->p_buffer,
                p_this->p_suggest_node->i_length);
        }
    }
    else
    {
        p_this->p_suggest_node =
            (struct feed_suggest_node *)(
                0);

        p_this->i_suggest_length =
            0ul;
    }

    p_this->b_refresh_text =
        1;

}

static
void
feed_main_suggest_next(
    struct feed_handle * const
        p_this)
{
    struct feed_suggest_node *
        p_suggest_node;

    p_suggest_node =
        feed_suggest_list_next(
            &(
                p_this->o_suggest_list),
            p_this->p_suggest_node);

    if (p_suggest_node)
    {
        feed_main_suggest_node(
            p_this,
            p_suggest_node);

    }
}

static
void
feed_main_suggest_prev(
    struct feed_handle * const
        p_this)
{
    struct feed_suggest_node *
        p_suggest_node;

    p_suggest_node =
        feed_suggest_list_prev(
            &(
                p_this->o_suggest_list),
            p_this->p_suggest_node);

    if (p_suggest_node)
    {
        feed_main_suggest_node(
            p_this,
            p_suggest_node);
    }
}

static
void
feed_main_suggest_exit(
    struct feed_handle * const
        p_this)
{
    feed_suggest_list_clear(
        &(
            p_this->o_suggest_list));

    p_this->b_suggest =
        0;

}

static
void
feed_main_suggest_reset(
    struct feed_handle * const
        p_this)
{
    /* Restore original */
    struct feed_suggest_node *
        p_suggest_node;

    p_suggest_node =
        feed_suggest_list_first(
            &(
                p_this->o_suggest_list));

    if (p_suggest_node)
    {
        feed_main_suggest_node(
            p_this,
            p_suggest_node);
    }

    feed_main_suggest_exit(
        p_this);

}

static
void
feed_main_event_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event,
    char const
        b_notify)
{
    char
        b_done;

    struct feed_handle *
        p_this;

    struct feed_text *
        p_text;

    p_this =
        (struct feed_handle *)(
            p_context);

    p_text =
        p_this->p_text;

    p_text->o_last_event =
        *(
            p_event);

    b_done =
        0;

    if (!b_done)
    {
        if (p_this->b_verbose)
        {
            p_this->b_verbose =
                0;

            /* Split event into unicode characters */
            if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
            {
                feed_main_insert_newline(
                    p_this);
            }
            else
            {
                feed_main_insert_event(
                    p_this,
                    p_event);
            }

            b_done =
                1;
        }
    }

    if (!b_done)
    {
        if (
            p_this->b_suggest)
        {
            if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'G') == p_event->i_code)
            {
                feed_main_suggest_reset(
                    p_this);

                b_done =
                    1;
            }
            else if (((FEED_EVENT_KEY_FLAG | FEED_KEY_UP) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'I') == p_event->i_code))
            {
                feed_main_suggest_next(
                    p_this);

                b_done =
                    1;
            }
            else if (((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
                || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_SHIFT | 'I') == p_event->i_code))
            {
                feed_main_suggest_prev(
                    p_this);

                b_done =
                    1;
            }
            else
            {
                feed_main_suggest_exit(
                    p_this);

                b_done =
                    0;
            }
        }
    }

    if (!b_done)
    {
        /* do notify */
        /* Provide one line at a time */
        if (b_notify
            && p_this->o_descriptor.p_notify)
        {
            int
                i_notify_status;

            i_notify_status =
                (*(p_this->o_descriptor.p_notify))(
                    p_this->o_descriptor.p_context,
                    p_this,
                    p_event->a_raw,
                    p_event->i_raw_len);

            if (
                0 > i_notify_status)
            {
                /* Event has been filtered by application */
                b_done =
                    1;
            }

            /* After notification, check for stop */
            /* check for suggestions */
            if (p_this->o_suggest_list.i_count)
            {
                p_this->b_suggest = 1;

                /* Load first suggestion */
                p_this->p_suggest_node =
                    feed_suggest_list_first(
                        &(
                            p_this->o_suggest_list));

                if (p_this->p_suggest_node)
                {
                    feed_main_suggest_next(
                        p_this);
                }

                /* entering suggestion mode */
                b_done =
                    1;
            }
        }
    }

    if (!b_done)
    {
        if (!p_this->b_started)
        {
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'H') == p_event->i_code)
        {
            feed_main_delete_glyph_prev(p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'W') == p_event->i_code)
        {
            feed_main_delete_word_prev(p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'L') == p_event->i_code)
        {
            p_this->b_refresh_text = 1;
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DELETE) == p_event->i_code)
        {
            feed_main_delete_glyph_next(
                p_this);
        }
        else if (
            ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
            || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'A') == p_event->i_code))
        {
            feed_main_move_cursor_x0(
                p_this);
        }
        else if (
            ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
            || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'E') == p_event->i_code))
        {
            feed_main_move_cursor_x1(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_PAGEUP) == p_event->i_code)
        {
            feed_main_move_page_home(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_PAGEDOWN) == p_event->i_code)
        {
            feed_main_move_page_end(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_HOME) == p_event->i_code)
        {
            feed_main_move_cursor_top(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_END) == p_event->i_code)
        {
            feed_main_move_cursor_bottom(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
        {
            feed_main_move_cursor_left(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
        {
            feed_main_move_cursor_right(
                p_this);
        }
        else if (
            ((FEED_EVENT_KEY_FLAG | FEED_KEY_UP) == p_event->i_code)
            || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'P') == p_event->i_code))
        {
            feed_main_move_cursor_up(
                p_this);
        }
        else if (
            ((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
            || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'N') == p_event->i_code))
        {
            feed_main_move_cursor_down(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEUP) == p_event->i_code)
        {
            feed_main_move_page_prev(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_PAGEDOWN) == p_event->i_code)
        {
            feed_main_move_page_next(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_LEFT) == p_event->i_code)
        {
            /* Go left until reach begin of word */
            feed_main_move_word_left(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | FEED_KEY_RIGHT) == p_event->i_code)
        {
            /* Go right until reach begin of next word */
            feed_main_move_word_right(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
        {
            feed_main_insert_newline(
                p_this);
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'V') == p_event->i_code)
        {
            /* Verbose state */
            p_this->b_verbose = 1;
        }
        else
        {
            feed_main_insert_event(
                p_this,
                p_event);
        }
    }

    feed_main_refresh_job(
        p_this);

}

static
void
feed_main_step(
    struct feed_handle * const
        p_this)
{
    unsigned char
        c;

    if (
        feed_screen_read_character(
            p_this->p_screen,
            &(
                c)))
    {
        int
            i_result;

        struct feed_event
            o_event;

        i_result =
            feed_input_write(
                p_this->p_input,
                c,
                &(
                    o_event));

        if (
            0
            <= i_result)
        {
            if (
                0
                < i_result)
            {
                feed_main_event_callback(
                    p_this,
                    &(
                        o_event),
                    1);
            }
        }
        else
        {
            p_this->b_started =
                0;
        }
    }
    else
    {
        p_this->b_started =
            0;
    }
}

static
void
feed_main_loop(
    struct feed_handle * const
        p_this)
{
    while (
        p_this->b_started)
    {
        feed_main_step(
            p_this);

    }
}

int
feed_start(
    struct feed_handle * const
        p_this)
{
    int
        i_result;

    if (
        p_this)
    {
        if (
            !(p_this->b_started))
        {
            p_this->b_started =
                1;

            if (
                feed_screen_enter(
                    p_this->p_screen))
            {
                /* insert a dummy glyph at end of document */
                {
                    struct feed_utf8_code
                        o_dummy_eof;

                    o_dummy_eof.i_raw_len =
                        0u;

                    o_dummy_eof.a_raw[0u] =
                        '\000';

                    feed_text_append_utf8_code(
                        p_this->p_text,
                        &(
                            o_dummy_eof));
                }

                /* Move cursor to begin of document? */
                {
                    p_this->o_cursor.i_glyph_index =
                        0u;

                    p_this->o_cursor.p_glyph =
                        NULL;

                    p_this->o_cursor.i_line_index =
                        0u;

                    p_this->o_cursor.p_line =
                        NULL;

                    p_this->p_page_line =
                        NULL;

                    p_this->i_page_line_index =
                        0u;

                    p_this->i_page_glyph_index =
                        0u;

                    p_this->e_page_state =
                        feed_view_state_prompt;
                }

                /* force initial refresh */
                p_this->b_refresh_cursor =
                    1;

                p_this->b_refresh_text =
                    1;

                feed_main_refresh_job(
                    p_this);

                feed_main_loop(
                    p_this);

                feed_screen_set_cursor_pos(
                    p_this->p_screen,
                    p_this->i_final_cursor_address);

                feed_screen_leave(
                    p_this->p_screen);

                i_result =
                    0;
            }
            else
            {
                i_result =
                    -1;
            }
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_start() */

int
feed_stop(
    struct feed_handle * const
        p_this)
{
    int
        i_result;

    if (
        p_this->b_started)
    {
        p_this->b_started =
            0;

        i_result =
            0;
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_stop() */

int
feed_suggest(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length,
    unsigned long int const
        i_cursor_offset)
{
    int
        i_result;

    struct feed_suggest_node *
        p_suggest_node;

    {
        struct feed_suggest_descriptor
            o_suggest_descriptor;

        {
            memset(
                &(
                    o_suggest_descriptor),
                0x00u,
                sizeof(
                    o_suggest_descriptor));

            o_suggest_descriptor.p_buffer =
                p_data;

            o_suggest_descriptor.i_length =
                i_data_length;

            o_suggest_descriptor.i_cursor_offset =
                i_cursor_offset;
        }

        p_suggest_node =
            feed_suggest_node_create(
                p_this->p_client,
                &(
                    o_suggest_descriptor));
    }

    if (
        p_suggest_node)
    {
        if (
            feed_suggest_list_append(
                &(
                    p_this->o_suggest_list),
                p_suggest_node))
        {
            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            -1;
    }

    return
        i_result;

} /* feed_suggest() */

unsigned long int
feed_length(
    struct feed_handle * const
        p_this)
{
    return
        feed_text_length(
            p_this->p_text);

} /* feed_length() */

unsigned long int
feed_save(
    struct feed_handle * const
        p_this,
    unsigned char * const
        p_data,
    unsigned long int const
        i_data_length)
{
    unsigned long int
        i_result;

    struct feed_buf
        o_raw_content;

    if (
        feed_buf_init(
            &(
                o_raw_content),
            p_data,
            i_data_length))
    {
        feed_text_save(
            p_this->p_text,
            &(
                o_raw_content));

        i_result =
            o_raw_content.i_len;

        feed_buf_cleanup(
            &(
                o_raw_content));
    }
    else
    {
        i_result =
            0;
    }

    return
        i_result;

} /* feed_save() */

unsigned long int
feed_consume(
    struct feed_handle * const
        p_this,
    unsigned char * const
        p_data,
    unsigned long int const
        i_data_length)
{
    unsigned long int
        i_data_iterator;

    struct feed_buf
        o_buf;

    i_data_iterator =
        0ul;

    if (
        feed_buf_init(
            &(
                o_buf),
            p_data,
            i_data_length))
    {
        feed_text_consume(
            p_this->p_text,
            &(
                o_buf));

        i_data_iterator =
            o_buf.i_len;

        feed_buf_cleanup(
            &(
                o_buf));
    }

    if (
        i_data_iterator)
    {
        /* Invalidate engine state */
        /* TODO: smart invalidate of affected iterators */
        p_this->p_page_line = NULL;

        p_this->i_page_line_index = 0ul;

        p_this->i_page_glyph_index = 0ul;

        p_this->e_page_state = feed_view_state_prompt;

        feed_text_iterator_set_line(
            p_this->p_text,
            &(
                p_this->o_cursor),
            p_this->i_page_line_index);

        feed_text_iterator_set_glyph(
            p_this->p_text,
            &(
                p_this->o_cursor),
            p_this->i_page_glyph_index);

        p_this->b_refresh_text = 1;
    }

    return
        i_data_iterator;

} /* feed_consume() */

int
feed_complete(
    struct feed_handle * const
        p_this,
    unsigned long int const
        i_word_offset,
    unsigned long int const
        i_word_length)
{
    int
        i_result;

    struct feed_text_iterator
        o_word_iterator;

    unsigned long int
        i_cursor_offset;

    i_cursor_offset =
        feed_text_iterator_get_offset(
            p_this->p_text,
            &(
                p_this->o_cursor));

    /* Clear the suggestion list */
    feed_suggest_list_clear(
        &(
            p_this->o_suggest_list));

    /* Create an iterator */
    feed_text_iterator_init(
        p_this->p_text,
        &(
            o_word_iterator));

    /* Goto word offset */
    feed_text_iterator_set_offset(
        p_this->p_text,
        &(
            o_word_iterator),
        i_word_offset);

    /* Remember position of original word */
    p_this->i_suggest_offset =
        i_word_offset;

    p_this->i_suggest_length =
        i_word_length;

    /* Keep a copy of iterator */
    p_this->o_suggest_iterator =
        o_word_iterator;

    /* Allocate a buffer for word */
    if (i_word_length)
    {
        unsigned char *
            p_word_buf;

        p_word_buf =
            (unsigned char *)(
                feed_heap_alloc(
                    p_this->p_heap,
                    i_word_length));

        if (
            p_word_buf)
        {
            struct feed_buf
                o_buf;

            /* Save the word */
            if (
                feed_buf_init(
                    &(
                        o_buf),
                    p_word_buf,
                    i_word_length))
            {
                feed_text_iterator_save(
                    p_this->p_text,
                    &(
                        o_word_iterator),
                    &(
                        o_buf));

                feed_suggest(
                    p_this,
                    p_word_buf,
                    i_word_length,
                    i_cursor_offset);

                feed_buf_cleanup(
                    &(
                        o_buf));
            }

            feed_heap_free(
                p_this->p_heap,
                (void *)(
                    p_word_buf));
        }
    }
    else
    {
        feed_suggest(
            p_this,
            (unsigned char *)(
                0),
            0ul,
            i_cursor_offset);
    }

    /* Destroy the iterator */
    feed_text_iterator_cleanup(
        p_this->p_text,
        &(
            o_word_iterator));

    /* Initialize state machine for completion engine */

    i_result =
        0;

    return
        i_result;

} /* feed_complete() */

unsigned long int
feed_cursor(
    struct feed_handle * const
        p_this)
{
    unsigned long int
        i_cursor_offset;

    i_cursor_offset =
        feed_text_iterator_get_offset(
            p_this->p_text,
            &(
                p_this->o_cursor));

    return
        i_cursor_offset;

} /* feed_cursor() */

int
feed_exec(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    unsigned long int
        i_data_iterator;

    i_result =
        0;

    i_data_iterator =
        0ul;

    while (
        (
            0 <= i_result)
        && (
            i_data_iterator < i_data_length))
    {
        unsigned char
            c;

        struct feed_event
            o_event;

        c =
            p_data[i_data_iterator];

        i_result =
            feed_input_write(
                p_this->p_input,
                c,
                &(
                    o_event));

        if (
            0
            <= i_result)
        {
            if (
                0
                < i_result)
            {
                feed_main_event_callback(
                    p_this,
                    &(
                        o_event),
                    0);
            }

            i_data_iterator ++;
        }
    }

    return
        i_result;

} /* feed_exec() */

/* end-of-file: feed.c */
