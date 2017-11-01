/* See LICENSE for license details */

/*

Module: feed_screen.c

Description:

*/

#include "feed_os.h"

#include "feed_screen.h"

#include "feed_client.h"

#include "feed_heap.h"

#include "feed_tty.h"

#include "feed_input.h"

static
void
feed_screen_write_clip(
    struct feed_screen * const
        p_screen,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count)
{
    struct feed_client *
        p_client;

    struct feed_tty *
        p_tty;

    p_client =
        p_screen->p_client;

    p_tty =
        feed_client_get_tty(
            p_client);

    if (1 < p_screen->i_screen_width)
    {
        if ((p_screen->i_cursor_x + 1) < p_screen->i_screen_width)
        {
            feed_tty_write_character_array(
                p_tty,
                p_data,
                i_count);

            p_screen->i_cursor_x += 1;
        }
    }
}

static
void
feed_screen_event_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    struct feed_screen *
        p_screen;

    struct feed_client *
        p_client;

    struct feed_tty *
        p_tty;

    p_screen =
        (struct feed_screen *)(
            p_context);

    p_client =
        p_screen->p_client;

    p_tty =
        feed_client_get_tty(
            p_client);

    if (FEED_EVENT_KEY_FLAG & p_event->i_code)
    {
        /* Special keys should already by converted to visual... */
        /* This could be an attribute, send direct */
        feed_tty_write_character_array(
            p_tty,
            p_event->a_raw,
            p_event->i_raw_len);
    }
    else
    {
        if (1 < p_screen->i_screen_width)
        {
            if ((p_screen->i_cursor_x + 1) >= p_screen->i_screen_width)
            {
                feed_screen_newline(p_screen);
            }

            feed_screen_write_clip(p_screen, p_event->a_raw, p_event->i_raw_len);
        }
    }
}

static
void
feed_screen_init(
    struct feed_screen * const
        p_screen,
    struct feed_client * const
        p_client,
    unsigned int const
        i_screen_width,
    unsigned int const
        i_screen_height)
{
    memset(
        p_screen,
        0x00u,
        sizeof(
            struct feed_screen));

    p_screen->p_client =
        p_client;

    p_screen->p_input =
        feed_input_create(
            p_client,
            &(
                feed_screen_event_callback),
            (void *)(
                p_screen));

    p_screen->i_screen_width =
        i_screen_width;

    p_screen->i_screen_height =
        i_screen_height;

    p_screen->i_region_height =
        1u;

    p_screen->i_cursor_x =
        0u;

    p_screen->i_cursor_y =
        0u;

} /* feed_screen_init() */

static
void
feed_screen_cleanup(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen->p_input)
    {
        feed_input_destroy(
            p_screen->p_input);

        p_screen->p_input =
            (struct feed_input *)(
                0);
    }

    p_screen->p_client =
        (struct feed_client *)(
            0);

    p_screen->i_screen_width =
        0u;

    p_screen->i_screen_height =
        0u;

    p_screen->i_region_height =
        0u;

    p_screen->i_cursor_x =
        0u;

    p_screen->i_cursor_y =
        0u;

} /* feed_screen_cleanup() */

struct feed_screen *
feed_screen_create(
    struct feed_client * const
        p_client,
    unsigned int const
        i_screen_width,
    unsigned int const
        i_screen_height)
{
    struct feed_screen *
        p_screen;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_screen =
        (struct feed_screen *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_screen)));

    if (
        p_screen)
    {
        feed_screen_init(
            p_screen,
            p_client,
            i_screen_width,
            i_screen_height);
    }

    return
        p_screen;

} /* feed_screen_create() */

void
feed_screen_destroy(
    struct feed_screen * const
        p_screen)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_screen->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_screen_cleanup(
        p_screen);

    feed_heap_free(
        p_heap,
        (void *)(
            p_screen));

}

void
feed_screen_cursor(
    struct feed_screen * const
        p_screen,
    unsigned int const
        i_cursor_x,
    unsigned int const
        i_cursor_y)
{
    if (i_cursor_x >= p_screen->i_screen_width)
    {
        feed_screen_cursor(
            p_screen,
            p_screen->i_screen_width - 1u,
            i_cursor_y);
    }
    else if (i_cursor_y >= p_screen->i_region_height)
    {
        feed_screen_cursor(
            p_screen,
            i_cursor_x,
            p_screen->i_region_height - 1u);
    }
    else
    {
        static unsigned char const g_cr[] =
        {
            '\r'
        };

        struct feed_client *
            p_client;

        struct feed_tty *
            p_tty;

        p_client =
            p_screen->p_client;

        p_tty =
            feed_client_get_tty(
                p_client);

        feed_tty_write_character_array(
            p_tty,
            g_cr,
            sizeof(g_cr));

        if (i_cursor_x)
        {
            feed_tty_move_cursor_forward(
                p_tty,
                i_cursor_x);
        }

        if (i_cursor_y < p_screen->i_cursor_y)
        {
            feed_tty_move_cursor_up(
                p_tty,
                (unsigned int)(p_screen->i_cursor_y - i_cursor_y));
        }
        else if (i_cursor_y > p_screen->i_cursor_y)
        {
            feed_tty_move_cursor_down(
                p_tty,
                (unsigned int)(i_cursor_y - p_screen->i_cursor_y));
        }

        p_screen->i_cursor_x =
            i_cursor_x;

        p_screen->i_cursor_y =
            i_cursor_y;

    }
}

void
feed_screen_newline(
    struct feed_screen * const
        p_screen)
{
    struct feed_client *
        p_client;

    struct feed_tty *
        p_tty;

    p_client =
        p_screen->p_client;

    p_tty =
        feed_client_get_tty(
            p_client);

    if (
        (p_screen->i_cursor_y + 1u) >= p_screen->i_screen_height)
    {
        p_screen->i_cursor_y=
            p_screen->i_screen_height - 1u;

        p_screen->i_cursor_x =
            p_screen->i_screen_width - 1u;
    }
    else
    {
        {
            static unsigned char g_crlf[] =
            {
                '\r',
                '\n'
            };

            feed_tty_write_character_array(
                p_tty,
                g_crlf,
                sizeof(
                    g_crlf));
        }

        p_screen->i_cursor_x = 0;

        p_screen->i_cursor_y ++;

        if (p_screen->i_cursor_y >= p_screen->i_region_height)
        {
            p_screen->i_region_height = p_screen->i_cursor_y + 1u;
        }
    }
}

void
feed_screen_write(
    struct feed_screen * const
        p_screen,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count)
{
    unsigned int
        i;

    for (i=0u; i<i_count; i++)
    {
        feed_input_write(
            p_screen->p_input,
            p_data[i]);
    }
}

void
feed_screen_clear_line(
    struct feed_screen * const
        p_screen)
{
    struct feed_client *
        p_client;

    struct feed_tty *
        p_tty;

    p_client =
        p_screen->p_client;

    p_tty =
        feed_client_get_tty(
            p_client);

    feed_tty_write_el(
        p_tty,
        0u);

}

