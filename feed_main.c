/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_main.h"

#include "feed_client.h"

#include "feed_tty.h"

#include "feed_input.h"

#include "feed_heap.h"

#include "feed_keys.h"

#include "feed_buf.h"

#include "feed_list.h"

#include "feed_glyph.h"

#include "feed_line.h"

#include "feed_text.h"

static
void
#if defined(__GNUC__)
__attribute__((format (printf,1,2)))
#endif
feed_dbg_print(
    char const * const
        p_format, ...)
{
    va_list
        o_args;

    va_start(
        o_args,
        p_format);

    vfprintf(
        stdout,
        p_format,
        o_args);

    fprintf(
        stdout,
        "\r\n");

    fflush(
        stdout);

    va_end(
        o_args);
}

/*

Module: feed_screen.h

Description:

    Manager for refresh of visible characters on terminal screen.

Comments:

    Handling of limited screen width and height.

    Avoid writing on last character of screen

    Handle grow of region, which may do a vertical scroll

    Backbuffer and front buffer

    Wrap of wide characters

    Do relative movements for cursor, so that region may grow.

    When cursor y > region height, then grow region height or clip or scroll.

    When cursor x > region width, then grow or clip or wrap or scroll.

*/
struct feed_screen
{
    unsigned int
        i_screen_width;

    unsigned int
        i_screen_height;

    unsigned int
        i_region_x;

    unsigned int
        i_region_y;

    unsigned int
        i_region_width;

    unsigned int
        i_region_height;

    unsigned int
        i_cursor_x;

    unsigned int
        i_cursor_y;

}; /* struct feed_screen */

/*
feed_screen_create
feed_screen_destroy
feed_screen_cursor
feed_screen_write_glyph
feed_screen_write_line
feed_screen_write_text
*/

struct feed_main_context
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    struct feed_text *
        p_text;

    struct feed_client
        o_client;

    struct feed_tty
        o_tty;

    unsigned int
        i_ocx;

    unsigned int
        i_ocy;

    unsigned int
        i_wx;

    unsigned int
        i_wy;

    unsigned int
        i_cy;

    unsigned int
        a_padding_int[3u];

    char
        b_more;

    unsigned char
        a_padding[7u];

};

static
unsigned int
feed_main_print_status(
    struct feed_event const * const
        p_event,
    unsigned int const
        i_screen_width)
{
    unsigned int i_width;
    unsigned char i;
    unsigned char c;

    i_width = 0u;
    if (i_width + 11u < i_screen_width)
    {
        printf("%08lx: [", p_event->i_code);
        i_width += 11u;
    }
    for (i=0u; i<p_event->i_raw_len; i++)
    {
        c = p_event->a_raw[i];

        if ((c >= 32) && (c < 127))
        {
            if (i_width + 4u < i_screen_width)
            {
                printf(" '%c'", (char)(c));
                i_width += 4u;
            }
        }
        else
        {
            if (i_width + 4u < i_screen_width)
            {
                printf(" %3u", (unsigned int)(p_event->a_raw[i]));
                i_width += 4u;
            }
        }
    }
    if (i_width + 2u < i_screen_width)
    {
        printf(" ]");
        i_width += 2u;
    }

    {
        unsigned char a_name[64u];

        struct feed_buf o_name;

        memset(a_name, 0, sizeof(a_name));

        feed_buf_init(
            &(
                o_name),
            a_name,
            sizeof(
                a_name));

        feed_input_print(
            p_event,
            &(
                o_name));

        if (i_width + 3u + o_name.i_len < i_screen_width)
        {
            printf(" <%.*s>", (int)(o_name.i_len), (char const *)(o_name.p_buf));
            i_width += 3u + o_name.i_len;
        }
    }

    printf("\033[0K");

    return
        i_width;
}

static
void
feed_text_refresh(
    struct feed_main_context * const
        p_main_context)
{
    struct feed_text *
        p_text;

    unsigned int
        i_screen_height;

    unsigned int
        i_screen_width;

    unsigned int
        i_width;

    unsigned int
        i_height;

    unsigned int
        i_cursor_visible_y;

    unsigned int
        i_cursor_visible_x;

    unsigned int
        i_cursor_glyph_iterator;

    (void)(
        i_screen_height);

    p_text =
        p_main_context->p_text;

    i_screen_width =
        p_main_context->i_wx;

    i_screen_height =
        p_main_context->i_wy;

    i_width =
        0u;

    i_height =
        0u;

    i_cursor_visible_y =
        0u;

    i_cursor_visible_x =
        0u;

    i_cursor_glyph_iterator =
        0u;

    /* Grow size of drawing region */

    /* Move cursor to beginning of drawing region */
    printf("\r");
    if (p_main_context->i_cy)
    {
        printf("\033[%dA", p_main_context->i_cy);
    }

    /* Print state of body text */
    {
        struct feed_list *
            p_line_iterator;

        struct feed_list *
            p_glyph_iterator;

        /* For all lines in body text */
        p_line_iterator =
            p_text->o_lines.p_next;

        while (
            p_line_iterator
            != &(
                p_text->o_lines))
        {
            struct feed_line *
                p_line;

            p_line =
                (struct feed_line *)(
                    p_line_iterator);

            /* If line is within refresh window */

            /* Return to home position */
            printf("\r");

            i_width = 0;

            if (0u == i_height)
            {
                /* Draw a prompt on first line */
                /*
                printf("prompt$ ");
                */

                p_glyph_iterator =
                    p_text->p_prompt->o_glyphs.p_next;

                while (
                    p_glyph_iterator
                    != &(
                        p_text->p_prompt->o_glyphs))
                {
                    struct feed_glyph const *
                        p_glyph;

                    p_glyph =
                        (struct feed_glyph const *)(
                            p_glyph_iterator);

                    /* If char is within refresh window */

                    if ((i_width + p_glyph->i_visible_width) < i_screen_width)
                    {
                        printf("%.*s",
                            (int)(p_glyph->i_visible_width),
                            p_glyph->a_visible);
                    }
                    else
                    {
                        i_height ++;

                        i_width = 0;

                        printf("\033[0K\r\n%.*s",
                            (int)(p_glyph->i_visible_width),
                            p_glyph->a_visible);
                    }

                    i_width += p_glyph->i_visible_width;

                    p_glyph_iterator =
                        p_glyph_iterator->p_next;
                }

                i_cursor_visible_x =
                    i_width;

                i_cursor_visible_y =
                    i_height;
            }
            else
            {
                printf("\n");
            }


            /* For all chars in line */
            p_glyph_iterator =
                p_line->o_glyphs.p_next;

            while (
                p_glyph_iterator
                != &(
                    p_line->o_glyphs))
            {
                struct feed_glyph const *
                    p_glyph;

                p_glyph =
                    (struct feed_glyph const *)(
                        p_glyph_iterator);

                /* If char is within refresh window */

                if ((i_width + p_glyph->i_visible_width) < i_screen_width)
                {
                    printf("%.*s",
                        (int)(p_glyph->i_visible_width),
                        p_glyph->a_visible);
                }
                else
                {
                    i_height ++;

                    i_width = 0;

                    printf("\033[0K\r\n%.*s",
                        (int)(p_glyph->i_visible_width),
                        p_glyph->a_visible);
                }

                i_width += p_glyph->i_visible_width;

                if (i_cursor_glyph_iterator < p_text->i_cursor_glyph_index)
                {
                    i_cursor_visible_x =
                        i_width;

                    i_cursor_visible_y =
                        i_height;
                }

                i_cursor_glyph_iterator ++;

                p_glyph_iterator =
                    p_glyph_iterator->p_next;
            }

            /* Erase to end-of-line */
            printf("\033[0K");

            i_height ++;

            p_line_iterator =
                p_line_iterator->p_next;
        }

        /* Draw status line */
        {
            i_width = 0u;

            if ((i_width + 8u) < i_screen_width)
            {
                printf("\r\nstatus: ");

                i_width += 8u;
            }

            i_width +=
                feed_main_print_status(
                    &(
                        p_text->o_last_event),
                    i_screen_width - i_width);

            i_height ++;
        }

        /* Position the cursor */
        printf("\r");
        if (i_height > 1u)
        {
            printf("\033[%dA", i_height - 1u);
        }
        if (i_cursor_visible_y)
        {
            printf("\033[%dB", i_cursor_visible_y);
        }
        if (i_cursor_visible_x)
        {
            printf("\033[%dC", i_cursor_visible_x);
        }

        p_main_context->i_cy =
            i_cursor_visible_y;
    }

}


static
void
feed_main_event_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    struct feed_main_context *
        p_main_context;

    struct feed_text *
        p_text;

    p_main_context =
        (struct feed_main_context *)(
            p_context);

    /* ((unsigned long int)(unsigned char)('q') == p_event->i_code) */
    if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'D') == p_event->i_code)
    {
        p_main_context->b_more =
            0;
    }
    else
    {
        p_text =
            p_main_context->p_text;

        p_text->o_last_event =
            *(
                p_event);

        if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'H') == p_event->i_code)
        {
            struct feed_line *
                p_line;

            struct feed_glyph *
                p_glyph;

            /* Find last line */
            if (p_text->o_lines.p_prev !=
                &(p_text->o_lines))
            {
                p_line =
                    (struct feed_line *)(
                        p_text->o_lines.p_prev);

                /* Find last char */
                if (p_line->o_glyphs.p_prev !=
                    &(p_line->o_glyphs))
                {
                    /* Delete the selected char */
                    p_glyph =
                        (struct feed_glyph *)(
                            p_line->o_glyphs.p_prev);

                    feed_glyph_destroy(
                        p_glyph);

                    if (p_text->i_cursor_glyph_index)
                    {
                        p_text->i_cursor_glyph_index --;
                    }

                    if (p_line->i_glyph_count)
                    {
                        p_line->i_glyph_count --;
                    }
                }
            }
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
        {
            p_text->i_cursor_glyph_index =
                0;
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
        {
            struct feed_line *
                p_line;

            /* Find last line */
            if (p_text->o_lines.p_prev !=
                &(p_text->o_lines))
            {
                p_line =
                    (struct feed_line *)(
                        p_text->o_lines.p_prev);

                p_text->i_cursor_glyph_index =
                    p_line->i_glyph_count;

#if 0
                struct feed_list *
                    p_iterator;

                p_text->i_cursor_glyph_index =
                    0;

                    p_iterator =
                        p_line->o_glyphs.p_next;

                    while (
                        p_iterator
                        != &(
                            p_line->o_glyphs))
                    {
                        p_text->i_cursor_glyph_index ++;

                        p_iterator =
                            p_iterator->p_next;
                    }
                }
#endif

            }
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
        {
            if (p_text->i_cursor_glyph_index)
            {
                p_text->i_cursor_glyph_index --;
            }
        }
        else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
        {
            p_text->i_cursor_glyph_index ++;
        }
        else
        {
            feed_text_write_event(
                p_text,
                p_event);
        }

        feed_text_refresh(
            p_main_context);
    }

}


int
feed_main(
    unsigned int const
        argc,
    char const * const * const
        argv)
{
    struct feed_main_context
        o_main_context;

    struct feed_main_context *
        p_main_context;

    (void)(
        argc);
    (void)(
        argv);

    p_main_context =
        &(
            o_main_context);

    p_main_context->p_client =
        &(
            p_main_context->o_client);

    feed_client_init(
        p_main_context->p_client);

    p_main_context->p_heap =
        feed_heap_create();

    p_main_context->p_client->p_heap =
        p_main_context->p_heap;

    p_main_context->p_text =
        feed_text_create(
            p_main_context->p_client);

    {
        static unsigned char const s_prompt[] =
        {
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
        };

        feed_text_prompt(
            p_main_context->p_text,
            s_prompt,
            sizeof(
                s_prompt));
    }

    if (
        feed_tty_init(
            p_main_context->p_client,
            &(
                p_main_context->o_tty)))
    {
        if (
            feed_tty_enable(
                p_main_context->p_client,
                &(
                    p_main_context->o_tty)))
        {
            int
                x;

            int
                y;

            if (
                feed_tty_get_cursor_position(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    &(
                        y),
                    &(
                        x)))
            {
                if (0)
                {
                    feed_dbg_print(
                        "pos = %d , %d",
                        x,
                        y);
                }

                p_main_context->i_ocx =
                    (unsigned int)(
                        x);

                p_main_context->i_ocy =
                    (unsigned int)(
                        y);
            }
            else
            {
                feed_dbg_print(
                    "get_cursor_position error!");
            }

            if (
                feed_tty_get_window_size(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    &(
                        y),
                    &(
                        x)))
            {
                if (0)
                {
                    feed_dbg_print(
                        "winsize = %d , %d",
                        x,
                        y);
                }

                p_main_context->i_wx =
                    (unsigned int)(
                        x);

                p_main_context->i_wy =
                    (unsigned int)(
                        y);

                p_main_context->i_cy =
                    0u;
            }
            else
            {
                feed_dbg_print(
                    "get_window_size error!");
            }

#if 0
            /* test line wrap enable */
            {
                static unsigned char const g_test_line_wrap_enable[] =
                {
                    '\r',
                    '\n',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'a',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'b',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'c',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'd',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'e',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f',
                    'f'
                };

                feed_tty_line_wrap(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    1);

                feed_tty_write_character_array(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    20);

                /* test line wrap disable */
                feed_tty_line_wrap(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    0);

                feed_tty_write_character_array(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_main_context->p_client,
                    &(
                        p_main_context->o_tty),
                    20);
            }
#endif

#if 0

            feed_tty_move_cursor_up(
                p_main_context->p_client,
                &(
                    p_main_context->o_tty),
                3);

            feed_tty_move_cursor_forward(
                p_main_context->p_client,
                &(
                    p_main_context->o_tty),
                40);

#endif

            if (0)
            {
                p_main_context->b_more =
                    1;

                while (
                    p_main_context->b_more)
                {
                    unsigned char
                        a_escape[64u];

                    unsigned int
                        i_escape_len;

                    if (
                        feed_tty_read_escape_sequence(
                            p_main_context->p_client,
                            &(
                                p_main_context->o_tty),
                            a_escape,
                            sizeof(
                                a_escape),
                            &(
                                i_escape_len)))
                    {
                        feed_dbg_print(
                            "read escape len=%u",
                            i_escape_len);

                        {
                            unsigned int
                                i_index;

                            for (
                                i_index = 0;
                                i_index < i_escape_len;
                                i_index ++)
                            {
                                feed_dbg_print(
                                    " [%u] %u \'%c\'",
                                    i_index,
                                    (unsigned int)(
                                        a_escape[i_index]),
                                    (a_escape[i_index] >= 32 && a_escape[i_index] < 127) ? a_escape[i_index] : '.');
                            }
                        }

                        if (1 == i_escape_len)
                        {
                            if ('q' == a_escape[0u])
                            {
                                p_main_context->b_more = 0;
                            }
                        }
                    }
                    else
                    {
                        feed_dbg_print(
                            "read escape error!");
                    }
                }
            }

            if (1)
            {
                struct feed_input *
                    p_input;

                p_main_context->b_more =
                    1;

                /* Reserve an extra line on screen */
                printf("\r\n\033[A");

                feed_text_refresh(
                    p_main_context);

                p_input =
                    feed_input_create(
                        p_main_context->p_client,
                        &(
                            feed_main_event_callback),
                        p_main_context);

                while (
                    p_main_context->b_more)
                {
                    int
                        c;

                    c = getchar();

                    if (
                        EOF != c)
                    {
                        if (
                            feed_input_write(
                                p_input,
                                (unsigned char)(
                                    c)))
                        {
                        }
                        else
                        {
                            p_main_context->b_more =
                                0;
                        }
                    }
                    else
                    {
                        p_main_context->b_more =
                            0;
                    }
                }

                printf("\r\n\n");

                feed_input_destroy(
                    p_input);
            }

            feed_tty_disable(
                p_main_context->p_client,
                &(
                    p_main_context->o_tty));
        }
        else
        {
            feed_dbg_print(
                "enable error!");
        }

        feed_tty_cleanup(
            p_main_context->p_client,
            &(
                p_main_context->o_tty));
    }
    else
    {
        feed_dbg_print(
            "init error!");
    }

    feed_text_destroy(
        p_main_context->p_text);

    feed_heap_destroy(
        p_main_context->p_heap);

    feed_client_cleanup(
        p_main_context->p_client);

    return
        0;

}

