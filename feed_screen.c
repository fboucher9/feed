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

#include "feed_utf8.h"

#include "feed_object.h"

#include "feed_screen_info.h"

struct feed_screen
{
    struct feed_client *
        p_client;

    struct feed_tty *
        p_tty;

    /* -- */

    unsigned int
        i_screen_cx;

    unsigned int
        i_screen_cy;

    unsigned int
        i_region_height;

    unsigned int
        i_region_cx;

    /* -- */

    unsigned int
        i_region_cy;

    unsigned int
        ui_padding[3u];

    /* -- */

    unsigned char
        i_foreground;

    unsigned char
        i_background;

    unsigned char
        uc_padding[6u];

}; /* struct feed_screen */

static
void
feed_screen_newline_raw(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            static unsigned char g_crlf[] =
            {
                '\r',
                '\n'
            };

            feed_tty_write_character_array(
                p_screen->p_tty,
                g_crlf,
                sizeof(
                    g_crlf));
        }
    }
}

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
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            struct feed_screen_info * const
                p_screen_info =
                feed_client_get_screen_info(
                    p_client);

            if (
                p_screen_info)
            {
                if (1 < p_screen_info->i_screen_width)
                {
                    if (
                        p_screen->i_region_cy < (p_screen_info->i_screen_height - 1u))
                    {
                        if (
                            p_screen->i_region_cx < (p_screen_info->i_screen_width - 1u))
                        {
                            feed_tty_write_character_array(
                                p_screen->p_tty,
                                p_data,
                                i_count);

                            p_screen->i_screen_cx += 1;
                        }
                        else if (
                            p_screen->i_region_cx == (p_screen_info->i_screen_width - 1u))
                        {
                            feed_tty_write_character_array(
                                p_screen->p_tty,
                                p_data,
                                i_count);

                            p_screen->i_screen_cx ++;

                            p_screen->i_region_cx ++;

                            feed_screen_newline(
                                p_screen);

                            p_screen->i_region_cx --;
                        }
                        else
                        {
                        }
                    }
                    else if (
                        p_screen->i_region_cy == (p_screen_info->i_screen_height - 1u))
                    {
                        if (
                            p_screen->i_region_cx < (p_screen_info->i_screen_width - 1u))
                        {
                            feed_tty_write_character_array(
                                p_screen->p_tty,
                                p_data,
                                i_count);

                            p_screen->i_screen_cx += 1;
                        }
                        else if (
                            p_screen->i_region_cx == (p_screen_info->i_screen_width - 1u))
                        {
                        }
                        else
                        {
                        }
                    }

                    p_screen->i_region_cx ++;
                }
            }
        }
    }
}

static
void
feed_screen_event_callback(
    void * const
        p_context,
    struct feed_utf8_code const * const
        p_event)
{
    struct feed_screen * const
        p_screen =
        (struct feed_screen *)(
            p_context);

    if (
        p_screen)
    {
        feed_screen_write_clip(
            p_screen,
            p_event->a_raw,
            p_event->i_raw_len);
    }
}

static
char
feed_screen_init(
    void * const
        p_buf,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    struct feed_screen *
        p_screen;

    p_screen =
        (struct feed_screen *)(
            p_buf);

    (void)(
        p_descriptor);

    memset(
        p_screen,
        0x00u,
        sizeof(
            struct feed_screen));

    p_screen->p_client =
        p_client;

    p_screen->p_tty =
        feed_tty_create(
            p_client);

    p_screen->i_screen_cx =
        0u;

    p_screen->i_screen_cy =
        0u;

    p_screen->i_region_height =
        1u;

    p_screen->i_region_cx =
        0u;

    p_screen->i_region_cy =
        0u;

    p_screen->i_foreground =
        0u;

    p_screen->i_background =
        0u;

    return
        1;

} /* feed_screen_init() */

static
void
feed_screen_cleanup(
    void * const
        p_buf)
{
    struct feed_screen *
        p_screen;

    p_screen =
        (struct feed_screen *)(
            p_buf);

    if (
        p_screen->p_tty)
    {
        feed_tty_destroy(
            p_screen->p_tty);

        p_screen->p_tty =
            (struct feed_tty *)(
                0);
    }

    p_screen->p_client =
        (struct feed_client *)(
            0);

    p_screen->i_region_height =
        0u;

    p_screen->i_region_cx =
        0u;

    p_screen->i_region_cy =
        0u;

} /* feed_screen_cleanup() */

struct feed_screen *
feed_screen_create(
    struct feed_client * const
        p_client)
{
    return
        (struct feed_screen *)(
            feed_object_create(
                p_client,
                sizeof(
                    struct feed_screen),
                &(
                    feed_screen_init),
                NULL));

} /* feed_screen_create() */

void
feed_screen_destroy(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        struct feed_client *
            p_client;

        p_client =
            p_screen->p_client;

        feed_object_destroy(
            p_client,
            (void *)(
                p_screen),
            &(
                feed_screen_cleanup));
    }

}

void
feed_screen_set_cursor_pos(
    struct feed_screen * const
        p_screen,
    unsigned long int const
        i_cursor_address)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
                p_screen->p_client;

        if (
            p_client)
        {
            struct feed_screen_info * const
                p_screen_info =
                feed_client_get_screen_info(
                    p_client);

            if (
                p_screen_info)
            {
                if (i_cursor_address >= (p_screen_info->i_screen_width * p_screen->i_region_height))
                {
                    feed_screen_set_cursor_pos(
                        p_screen,
                        p_screen_info->i_screen_width * p_screen->i_region_height - 1u);
                }
                else
                {
                    unsigned short int i_cursor_x;

                    unsigned short int i_cursor_y;

                    unsigned int i_screen_cx;

                    unsigned int i_screen_cy;

                    i_cursor_x = (unsigned short int)(i_cursor_address % p_screen_info->i_screen_width);

                    i_cursor_y = (unsigned short int)(i_cursor_address / p_screen_info->i_screen_width);

                    /* Calculate new screen cursor position */
                    if (i_cursor_y < (p_screen_info->i_screen_height))
                    {
                        i_screen_cy = i_cursor_y;

                        if (i_cursor_x < (p_screen_info->i_screen_width))
                        {
                            i_screen_cx = i_cursor_x;
                        }
                        else
                        {
                            i_screen_cx = p_screen_info->i_screen_width - 1u;
                        }
                    }
                    else
                    {
                        i_screen_cy = p_screen_info->i_screen_height - 1u;

                        i_screen_cx = p_screen_info->i_screen_width - 1u;
                    }

                    if (i_screen_cx == p_screen->i_screen_cx)
                    {
                        /* nothing to do */
                    }
                    else if (0 == i_screen_cx)
                    {
                        static unsigned char g_cr[] =
                        {
                            '\r'
                        };

                        feed_tty_write_character_array(
                            p_screen->p_tty,
                            g_cr,
                            sizeof(
                                g_cr));
                    }
                    else if (i_screen_cx < p_screen->i_screen_cx)
                    {
                        feed_tty_move_cursor_backward(
                            p_screen->p_tty,
                            (unsigned int)(p_screen->i_screen_cx - i_screen_cx));
                    }
                    else if (i_screen_cx > p_screen->i_screen_cx)
                    {
                        feed_tty_move_cursor_forward(
                            p_screen->p_tty,
                            (unsigned int)(i_screen_cx - p_screen->i_screen_cx));
                    }

                    if (i_screen_cy < p_screen->i_screen_cy)
                    {
                        feed_tty_move_cursor_up(
                            p_screen->p_tty,
                            (unsigned int)(p_screen->i_screen_cy - i_screen_cy));
                    }
                    else if (i_screen_cy > p_screen->i_screen_cy)
                    {
                        feed_tty_move_cursor_down(
                            p_screen->p_tty,
                            (unsigned int)(i_screen_cy - p_screen->i_screen_cy));
                    }

                    p_screen->i_screen_cx =
                        i_screen_cx;

                    p_screen->i_screen_cy =
                        i_screen_cy;

                    p_screen->i_region_cx =
                        i_cursor_x;

                    p_screen->i_region_cy =
                        i_cursor_y;
                }
            }
        }
    }
}

static
void
feed_screen_clear_line(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            struct feed_screen_info * const
                p_screen_info =
                feed_client_get_screen_info(
                    p_client);

            if (
                p_screen_info)
            {
                if (
                    (
                        p_screen->i_region_cy < (p_screen_info->i_screen_height))
                    && (
                        p_screen->i_region_cx < (p_screen_info->i_screen_width)))
                {
                    feed_tty_write_el(
                        p_screen->p_tty,
                        0u);
                }
            }
        }
    }
}

void
feed_screen_newline(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            struct feed_screen_info * const
                p_screen_info =
                feed_client_get_screen_info(
                    p_client);

            if (
                p_screen_info)
            {
                feed_screen_clear_line(
                    p_screen);

                if (
                    (
                        p_screen->i_region_cy < (p_screen_info->i_screen_height - 1u)))
                {
                    feed_screen_newline_raw(
                        p_screen);

                    p_screen->i_screen_cx = 0u;

                    p_screen->i_screen_cy ++;
                }

                p_screen->i_region_cx = 0u;

                p_screen->i_region_cy ++;

                if ((p_screen->i_region_cy + 1u) > p_screen->i_region_height)
                {
                    p_screen->i_region_height = (p_screen->i_region_cy + 1u);
                }
            }
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
    struct feed_utf8_parser
        o_utf8_parser;

    if (
        feed_utf8_parser_init(
            &(
                o_utf8_parser)))
    {
        unsigned int
            i;

        for (i=0u; i<i_count; i++)
        {
            int
                i_result;

            struct feed_utf8_code
                o_utf8_code;

            i_result =
                feed_utf8_parser_write(
                    &(
                        o_utf8_parser),
                    p_data[i],
                    &(
                        o_utf8_code));

            if (
                0
                < i_result)
            {
                feed_screen_event_callback(
                    p_screen,
                    &(
                        o_utf8_code));
            }
        }

        feed_utf8_parser_cleanup(
            &(
                o_utf8_parser));
    }
}

unsigned long int
feed_screen_get_cursor_pos(
    struct feed_screen * const
        p_screen)
{
    unsigned long int
        i_cursor_address;

    i_cursor_address =
        0ul;

    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            struct feed_screen_info * const
                p_screen_info =
                feed_client_get_screen_info(
                    p_client);

            if (
                p_screen_info)
            {
                i_cursor_address =
                    p_screen->i_region_cy * p_screen_info->i_screen_width + p_screen->i_region_cx;
            }
        }
    }

    return
        i_cursor_address;

}

void
feed_screen_clear_region(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        feed_tty_clear_bottom(
            p_screen->p_tty);
    }
}

char
feed_screen_enter(
    struct feed_screen * const
        p_screen)
{
    char
        b_result;

    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            if (
                feed_tty_enable(
                    p_screen->p_tty))
            {
                unsigned char
                    a_sgr_reset[1u];

                a_sgr_reset[0u] =
                    0u;

                feed_tty_write_sgr(
                    p_screen->p_tty,
                    a_sgr_reset,
                    1);

                b_result =
                    1;
            }
            else
            {
                b_result =
                    0;
            }
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

} /* feed_screen_enter() */

void
feed_screen_leave(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            feed_screen_newline_raw(
                p_screen);

            feed_screen_color(
                p_screen,
                FEED_SCREEN_COLOR_DEFAULT,
                FEED_SCREEN_COLOR_DEFAULT);

            feed_tty_flush(
                p_screen->p_tty);

            feed_tty_disable(
                p_screen->p_tty);
        }
    }

} /* feed_screen_leave() */

void
feed_screen_update_info(
    struct feed_screen * const
        p_screen,
    char const
        b_fallback)
{
    if (
        p_screen)
    {
        struct feed_client * const
            p_client =
            p_screen->p_client;

        if (
            p_client)
        {
            unsigned short int
                i_screen_width;

            unsigned short int
                i_screen_height;

            if (
                feed_tty_get_window_size(
                    p_screen->p_tty,
                    &(
                        i_screen_width),
                    &(
                        i_screen_height),
                    b_fallback))
            {
                struct feed_screen_info * const
                    p_screen_info =
                    feed_client_get_screen_info(
                        p_client);

                /* Todo: detect screen size change and invalidate current
                page information */

#if 0
                if (i_screen_height > 13u)
                {
                    i_screen_height = 13u;
                }

                if (i_screen_width > 50u)
                {
                    i_screen_width = 50u;
                }
#endif

                if (
                    p_screen_info)
                {
                    feed_screen_info_update(
                        p_screen_info,
                        i_screen_width,
                        i_screen_height);
                }
            }
        }
    }

} /* feed_screen_update_info() */

static
void
feed_screen_flush(
    struct feed_screen * const
        p_screen)
{
    if (
        p_screen)
    {
        feed_tty_flush(
            p_screen->p_tty);
    }

} /* feed_screen_flush() */

char
feed_screen_read_character(
    struct feed_screen * const
        p_screen,
    unsigned char * const
        p_data)
{
    char
        b_result;

    feed_screen_flush(
        p_screen);

    b_result =
        feed_tty_read_character(
            p_screen->p_tty,
            p_data);

    return
        b_result;

} /* feed_screen_read_character() */

void
feed_screen_color(
    struct feed_screen * const
        p_screen,
    unsigned char const
        i_foreground,
    unsigned char const
        i_background)
{
    if ((i_foreground != p_screen->i_foreground)
        || (i_background != p_screen->i_background))
    {
        unsigned char
            p_attr[3u];

        unsigned int
            i_count;

        i_count = 0u;

        if (i_foreground || i_background)
        {
            if (i_foreground != p_screen->i_foreground)
            {
                if (FEED_SCREEN_COLOR_DEFAULT == i_foreground)
                {
                    p_attr[i_count ++] = 39u;
                }
                else
                {
                    if (FEED_SCREEN_COLOR_BRIGHT_BLACK <= i_foreground)
                    {
                        p_attr[i_count ++] = 1;
                    }

                    p_attr[i_count ++] = (unsigned char)(30u + (i_foreground & 7u));
                }
            }

            if (i_background != p_screen->i_background)
            {
                if (FEED_SCREEN_COLOR_DEFAULT == i_background)
                {
                    p_attr[i_count ++] = 49u;
                }
                else if (FEED_SCREEN_COLOR_BRIGHT_BLACK < i_background)
                {
                    p_attr[i_count ++] = (unsigned char)(40u + (i_background & 7u));
                }
                else
                {
                    p_attr[i_count ++] = (unsigned char)(100u + (i_background & 7u));
                }
            }
        }
        else
        {
            p_attr[i_count ++] = 0u;
        }

        if (i_count)
        {
            feed_tty_write_sgr(
                p_screen->p_tty,
                p_attr,
                i_count);
        }

        p_screen->i_foreground =
            i_foreground;

        p_screen->i_background =
            i_background;
    }

} /* feed_screen_color() */

/* end-of-file: feed_screen.c */
