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

#include "feed_prompt.h"

#include "feed_screen.h"

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

struct feed_main_context
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    struct feed_text *
        p_text;

    struct feed_prompt *
        p_prompt;

    struct feed_screen *
        p_screen;

    struct feed_client
        o_client;

    /* Cursor */
    unsigned int
        i_cursor_line_index;

    unsigned int
        i_cursor_glyph_index;

    unsigned int
        ui_padding[2u];

    char
        b_more;

    char
        b_verbose;

    unsigned char
        a_padding[6u];

};

static
void
feed_main_print_status(
    struct feed_main_context * const
        p_main_context,
    struct feed_event const * const
        p_event)
{
    struct feed_screen *
        p_screen;

    unsigned char
        i;

    unsigned char
        c;

    unsigned char
        buf[80u];

    int
        i_buf_len;

    p_screen =
        p_main_context->p_screen;

    i_buf_len =
        sprintf(
            (char *)(buf),
            "%08lx: [",
            p_event->i_code);

    if (
        i_buf_len > 0)
    {
        feed_screen_write(
            p_screen,
            buf,
            (unsigned int)(
                i_buf_len));
    }

    for (i=0u; i<p_event->i_raw_len; i++)
    {
        c = p_event->a_raw[i];

        if ((c >= 32) && (c < 127))
        {
            i_buf_len =
                sprintf(
                    (char *)(buf),
                    " '%c'",
                    (char)(c));

            if (
                i_buf_len > 0)
            {
                feed_screen_write(
                    p_screen,
                    buf,
                    (unsigned int)(
                        i_buf_len));
            }
        }
        else
        {
            i_buf_len =
                sprintf(
                    (char *)(buf),
                    " %3u",
                    (unsigned int)(p_event->a_raw[i]));

            if (
                i_buf_len > 0)
            {
                feed_screen_write(
                    p_screen,
                    buf,
                    (unsigned int)(
                        i_buf_len));
            }
        }
    }

    {
        static unsigned char const g_close[] =
        {
            ' ',
            ']',
            ' ',
            '<'
        };

        feed_screen_write(
            p_screen,
            g_close,
            (unsigned int)(
                sizeof(g_close)));
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

        feed_screen_write(
            p_screen,
            o_name.p_buf,
            o_name.i_len);
    }

    {
        static unsigned char const g_name_end[] =
        {
            '>'
        };

        feed_screen_write(
            p_screen,
            g_name_end,
            (unsigned int)(
                sizeof(
                    g_name_end)));
    }

    feed_screen_clear_line(
        p_screen);

}

static
void
feed_main_refresh_text(
    struct feed_main_context * const
        p_main_context)
{
    struct feed_screen *
        p_screen;

    struct feed_text *
        p_text;

    unsigned int
        i_cursor_line_iterator;

    unsigned int
        i_cursor_visible_y;

    unsigned int
        i_cursor_visible_x;

    unsigned int
        i_cursor_glyph_iterator;

    p_screen =
        p_main_context->p_screen;

    p_text =
        p_main_context->p_text;

    i_cursor_line_iterator =
        0u;

    i_cursor_visible_y =
        0u;

    i_cursor_visible_x =
        0u;

    /* Grow size of drawing region */

    /* Move cursor to beginning of drawing region */
    feed_screen_set_cursor_pos(
        p_main_context->p_screen,
        0u,
        0u);

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

            {
                struct feed_line *
                    p_prompt_line;

                if (0u == i_cursor_line_iterator)
                {
                    /* Draw a prompt on first line */
                    p_prompt_line =
                        feed_prompt_get1(
                            p_main_context->p_prompt);
                }
                else
                {
                    feed_screen_newline(
                        p_main_context->p_screen);

                    p_prompt_line =
                        feed_prompt_get2(
                            p_main_context->p_prompt);
                }

                if (p_prompt_line)
                {
                    p_glyph_iterator =
                        p_prompt_line->o_glyphs.p_next;

                    while (
                        p_glyph_iterator
                        != &(
                            p_prompt_line->o_glyphs))
                    {
                        struct feed_glyph const *
                            p_glyph;

                        p_glyph =
                            (struct feed_glyph const *)(
                                p_glyph_iterator);

                        /* If char is within refresh window */

                        feed_screen_write(
                            p_main_context->p_screen,
                            p_glyph->a_visible,
                            p_glyph->i_visible_length);

                        p_glyph_iterator =
                            p_glyph_iterator->p_next;
                    }
                }
            }

            if (i_cursor_line_iterator == p_main_context->i_cursor_line_index)
            {
                feed_screen_get_cursor_pos(
                    p_screen,
                    &(
                        i_cursor_visible_x),
                    &(
                        i_cursor_visible_y));

            }

            /* For all chars in line */
            p_glyph_iterator =
                p_line->o_glyphs.p_next;

            i_cursor_glyph_iterator =
                0u;

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

                feed_screen_write(
                    p_screen,
                    p_glyph->a_visible,
                    p_glyph->i_visible_length);

                if ((i_cursor_line_iterator == p_main_context->i_cursor_line_index)
                    && (i_cursor_glyph_iterator < p_main_context->i_cursor_glyph_index))
                {
                    feed_screen_get_cursor_pos(
                        p_screen,
                        &(
                            i_cursor_visible_x),
                        &(
                            i_cursor_visible_y));
                }

                i_cursor_glyph_iterator ++;

                p_glyph_iterator =
                    p_glyph_iterator->p_next;
            }

            /* Erase to end-of-line */
            feed_screen_clear_line(
                p_screen);

            i_cursor_line_iterator ++;

            p_line_iterator =
                p_line_iterator->p_next;
        }
    }

    /* Draw status line */
    {
        feed_screen_newline(p_screen);

        {
            static unsigned char const g_status_header[] =
            {
                's',
                't',
                'a',
                't',
                'u',
                's',
                ':',
                ' '
            };

            feed_screen_write(
                p_screen,
                g_status_header,
                (unsigned int)(
                    sizeof(
                        g_status_header)));
        }

        feed_main_print_status(
            p_main_context,
            &(
                p_text->o_last_event));
    }

    /* Erase old lines */
    feed_screen_clear_region(
        p_screen);

    /* Position the cursor */
    {
        feed_screen_set_cursor_pos(
            p_screen,
            i_cursor_visible_x,
            i_cursor_visible_y);
    }

    feed_tty_flush(
        p_main_context->p_client->p_tty);

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

    p_text =
        p_main_context->p_text;

    p_text->o_last_event =
        *(
            p_event);

    if (p_main_context->b_verbose)
    {
        p_main_context->b_verbose =
            0;

        feed_text_write_event(
            p_text,
            p_event,
            p_main_context->i_cursor_line_index,
            p_main_context->i_cursor_glyph_index);

        p_main_context->i_cursor_glyph_index ++;

        feed_main_refresh_text(
            p_main_context);
    }
    else
    {
        /* ((unsigned long int)(unsigned char)('q') == p_event->i_code) */
        if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'D') == p_event->i_code)
        {
            p_main_context->b_more =
                0;
        }
        else
        {
            p_text->o_last_event =
                *(
                    p_event);

            if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'H') == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_main_context->i_cursor_line_index);

                if (
                    p_line)
                {
                    /* Find last char */
                    if (p_main_context->i_cursor_glyph_index)
                    {
                        struct feed_glyph *
                            p_glyph;

                        p_glyph =
                            feed_line_get_glyph(
                                p_line,
                                p_main_context->i_cursor_glyph_index - 1u);

                        if (
                            p_glyph)
                        {
                            /* Delete the selected char */
                            feed_glyph_destroy(
                                p_glyph);

                            p_main_context->i_cursor_glyph_index --;

                            if (p_line->i_glyph_count)
                            {
                                p_line->i_glyph_count --;
                            }
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        /* Take all glyphs of current line and append at
                        end of previous line */
                        if (p_main_context->i_cursor_line_index)
                        {
                            struct feed_line *
                                p_line_up;

                            p_main_context->i_cursor_line_index --;

                            p_line_up =
                                (struct feed_line *)(
                                    p_line->o_list.p_prev);

                            p_main_context->i_cursor_glyph_index =
                                p_line_up->i_glyph_count;

                            while (
                                p_line->i_glyph_count)
                            {
                                struct feed_glyph *
                                    p_glyph;

                                p_glyph =
                                    (struct feed_glyph *)(
                                        p_line->o_glyphs.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line_up->o_glyphs));

                                p_line_up->i_glyph_count ++;
                            }

                            feed_line_destroy(
                                p_line);

                            p_text->i_line_count --;
                        }
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DELETE) == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_main_context->i_cursor_line_index);

                if (
                    p_line)
                {
                    /* Find last char */
                    if (p_main_context->i_cursor_glyph_index < p_line->i_glyph_count)
                    {
                        struct feed_glyph *
                            p_glyph;

                        p_glyph =
                            feed_line_get_glyph(
                                p_line,
                                p_main_context->i_cursor_glyph_index);

                        if (
                            p_glyph)
                        {
                            /* Delete the selected char */
                            feed_glyph_destroy(
                                p_glyph);

                            if (p_line->i_glyph_count)
                            {
                                p_line->i_glyph_count --;
                            }
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        /* Bring next line into this line */
                        /* Delete next line */
                        if ((p_main_context->i_cursor_line_index + 1u) < p_text->i_line_count)
                        {
                            struct feed_line *
                                p_line_down;

                            p_line_down =
                                (struct feed_line *)(
                                    p_line->o_list.p_next);

                            while (
                                p_line_down->i_glyph_count)
                            {
                                struct feed_glyph *
                                    p_glyph;

                                p_glyph =
                                    (struct feed_glyph *)(
                                        p_line_down->o_glyphs.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line_down->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line->o_glyphs));

                                p_line->i_glyph_count ++;
                            }

                            feed_line_destroy(
                                p_line_down);

                            p_text->i_line_count --;
                        }
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_HOME) == p_event->i_code)
            {
                p_main_context->i_cursor_glyph_index =
                    0;
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_END) == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_main_context->i_cursor_line_index);

                if (
                    p_line)
                {
                    p_main_context->i_cursor_glyph_index =
                        p_line->i_glyph_count;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
            {
                if (p_main_context->i_cursor_glyph_index)
                {
                    p_main_context->i_cursor_glyph_index --;
                }
                else
                {
                    /* Go to previous line */
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
            {
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_main_context->i_cursor_line_index);

                if (
                    p_line)
                {
                    if (p_main_context->i_cursor_glyph_index < p_line->i_glyph_count)
                    {
                        p_main_context->i_cursor_glyph_index ++;
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_UP) == p_event->i_code)
            {
                if (p_main_context->i_cursor_line_index)
                {
                    p_main_context->i_cursor_line_index --;

                    p_main_context->i_cursor_glyph_index = 0u;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_DOWN) == p_event->i_code)
            {
                if ((p_main_context->i_cursor_line_index + 1u) < p_text->i_line_count)
                {
                    p_main_context->i_cursor_line_index ++;

                    p_main_context->i_cursor_glyph_index = 0u;
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_ALT | FEED_KEY_UP) == p_event->i_code)
            {
                feed_screen_modify_visible_pos(
                    p_main_context->p_screen,
                    0,
                    -1);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_ALT | FEED_KEY_DOWN) == p_event->i_code)
            {
                feed_screen_modify_visible_pos(
                    p_main_context->p_screen,
                    0,
                    1);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_ALT | FEED_KEY_RIGHT) == p_event->i_code)
            {
                feed_screen_modify_visible_pos(
                    p_main_context->p_screen,
                    1,
                    0);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_ALT | FEED_KEY_LEFT) == p_event->i_code)
            {
                feed_screen_modify_visible_pos(
                    p_main_context->p_screen,
                    -1,
                    0);
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
            {
                /* Notify currently accumulated lines */

                /* Split current line */

                /* Create a new line */
                struct feed_line *
                    p_line;

                p_line =
                    feed_text_get_line(
                        p_text,
                        p_main_context->i_cursor_line_index);

                if (
                    p_line)
                {
                    struct feed_glyph *
                        p_glyph;

                    struct feed_line *
                        p_line_down;

                    p_glyph =
                        feed_line_get_glyph(
                            p_line,
                            p_main_context->i_cursor_glyph_index);

                    p_line_down =
                        feed_line_create(
                            p_main_context->p_client);

                    if (
                        p_line_down)
                    {
                        feed_list_join(
                            &(
                                p_line->o_list),
                            &(
                                p_line_down->o_list));

                        p_text->i_line_count ++;

                        if (p_glyph)
                        {
                            /* Transfer characters from this line to next */
                            while (
                                &(
                                    p_glyph->o_list)
                                != &(
                                    p_line->o_glyphs))
                            {
                                struct feed_glyph *
                                    p_glyph_next;

                                p_glyph_next =
                                    (struct feed_glyph *)(
                                        p_glyph->o_list.p_next);

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_glyph->o_list));

                                p_line->i_glyph_count --;

                                feed_list_join(
                                    &(
                                        p_glyph->o_list),
                                    &(
                                        p_line_down->o_glyphs));

                                p_line_down->i_glyph_count ++;

                                p_glyph =
                                    p_glyph_next;
                            }
                        }

                        p_main_context->i_cursor_line_index ++;

                        p_main_context->i_cursor_glyph_index =
                            0u;
                    }
                }
            }
            else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'V') == p_event->i_code)
            {
                /* Verbose state */
                p_main_context->b_verbose = 1;
            }
            else
            {
                feed_text_write_event(
                    p_text,
                    p_event,
                    p_main_context->i_cursor_line_index,
                    p_main_context->i_cursor_glyph_index);

                p_main_context->i_cursor_glyph_index ++;
            }

            feed_main_refresh_text(
                p_main_context);
        }
    }

}

static
void
feed_main_set_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    struct feed_main_context * const
        p_main_context =
        (struct feed_main_context *)(
            p_context);

    struct feed_text * const
        p_text =
        p_main_context->p_text;

    if (((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'M') == p_event->i_code)
        || ((FEED_EVENT_KEY_FLAG | FEED_KEY_CTRL | 'J') == p_event->i_code))
    {
        /* Notify currently accumulated lines */

        /* Split current line */

        /* Create a new line */
        if (
            feed_text_append_line(
                p_text))
        {
            p_main_context->i_cursor_line_index ++;

            p_main_context->i_cursor_glyph_index =
                0u;
        }
    }
    else
    {
        feed_text_append_event(
            p_text,
            p_event);

        p_main_context->i_cursor_glyph_index ++;
    }

}

static
char
feed_main_set(
    struct feed_main_context * const
        p_main_context,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    char
        b_result;

    if (p_main_context)
    {
        /* Go to end of buffer */

        if (
            i_data_length)
        {
            struct feed_input *
                p_input;

            p_input =
                feed_input_create(
                    p_main_context->p_client,
                    &(
                        feed_main_set_callback),
                    p_main_context);

            if (
                p_input)
            {
                unsigned int
                    i_data_iterator;

                i_data_iterator =
                    0u;

                b_result =
                    1;

                while (
                    b_result
                    && (
                        i_data_iterator
                        < i_data_length))
                {
                    if (
                        feed_input_write(
                            p_input,
                            p_data[i_data_iterator]))
                    {
                        i_data_iterator ++;
                    }
                    else
                    {
                        b_result =
                            0;
                    }
                }

                feed_input_destroy(
                    p_input);
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
                1;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

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

    memset(
        p_main_context,
        0x00u,
        sizeof(
            *(
                p_main_context)));

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

    p_main_context->p_prompt =
        feed_prompt_create(
            p_main_context->p_client);

    {
        static unsigned char const s_prompt1[] =
        {
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' ',
        };

        static unsigned char const s_prompt2[] =
        {
            ' ',
            '>',
            ' '
        };

        feed_prompt_set1(
            p_main_context->p_prompt,
            s_prompt1,
            sizeof(
                s_prompt1));

        feed_prompt_set2(
            p_main_context->p_prompt,
            s_prompt2,
            sizeof(
                s_prompt2));
    }

    p_main_context->p_client->p_tty =
        feed_tty_create(
            p_main_context->p_client);

    if (
        p_main_context->p_client->p_tty)
    {
        if (
            feed_tty_enable(
                p_main_context->p_client->p_tty))
        {
            unsigned int
                x;

            unsigned int
                y;

            if (
                feed_tty_get_cursor_position(
                    p_main_context->p_client->p_tty,
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
            }
            else
            {
                feed_dbg_print(
                    "get_cursor_position error!");
            }

            if (
                feed_tty_get_window_size(
                    p_main_context->p_client->p_tty,
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

                p_main_context->p_screen =
                    feed_screen_create(
                        p_main_context->p_client,
                        x,
                        y);

            }
            else
            {
                feed_dbg_print(
                    "get_window_size error!");
            }

            p_main_context->b_verbose =
                0;

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
                    p_main_context->p_client->p_tty,
                    1);

                feed_tty_write_character_array(
                    p_main_context->p_client->p_tty,
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_main_context->p_client->p_tty,
                    20);

                /* test line wrap disable */
                feed_tty_line_wrap(
                    p_main_context->p_client->p_tty,
                    0);

                feed_tty_write_character_array(
                    p_main_context->p_client->p_tty,
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_main_context->p_client->p_tty,
                    20);
            }
#endif

#if 0

            feed_tty_move_cursor_up(
                p_main_context->p_client->p_tty,
                3);

            feed_tty_move_cursor_forward(
                p_main_context->p_client->p_tty,
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
                            p_main_context->p_client->p_tty,
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

            {
                static unsigned char const s_load[] =
                {
                    'a', 'b', 'c', '\n',
                    '1', '2', '3', '\n',
                    '\n',
                    '\n',
                    '!'
                };

                if (0)
                {
                    feed_text_set(
                        p_main_context->p_text,
                        s_load,
                        sizeof(
                            s_load));

                    /* Move cursor to end... */
                }

                if (0)
                {
                    /* Load using direct */
                    feed_main_set(
                        p_main_context,
                        s_load,
                        sizeof(
                            s_load));
                }
            }

            if (1)
            {
                struct feed_input *
                    p_input;

                p_main_context->b_more =
                    1;

                feed_main_refresh_text(
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

                feed_input_destroy(
                    p_input);
            }

            feed_screen_destroy(
                p_main_context->p_screen);

            feed_tty_disable(
                p_main_context->p_client->p_tty);
        }
        else
        {
            feed_dbg_print(
                "enable error!");
        }

        feed_tty_destroy(
            p_main_context->p_client->p_tty);
    }
    else
    {
        feed_dbg_print(
            "init error!");
    }

    feed_prompt_destroy(
        p_main_context->p_prompt);

    {
        unsigned int
            i_text_length;

        /* Transfer text to a linear buffer */
        i_text_length =
            feed_text_get_raw_length(
                p_main_context->p_text);

        if (
            i_text_length)
        {
            unsigned char *
                p_raw_buffer;

            p_raw_buffer =
                (unsigned char *)(
                    feed_heap_alloc(
                        p_main_context->p_client->p_heap,
                        i_text_length));

            if (
                p_raw_buffer)
            {
                struct feed_buf
                    o_raw_content;

                if (
                    feed_buf_init(
                        &(
                            o_raw_content),
                        p_raw_buffer,
                        i_text_length))
                {
                    feed_text_get_raw_buffer(
                        p_main_context->p_text,
                        &(
                            o_raw_content));

                    if (0)
                    {
                        printf("content: [\n%.*s]\n",
                            (int)(
                                o_raw_content.i_len),
                            (char const *)(
                                o_raw_content.p_buf));
                    }

                    feed_buf_cleanup(
                        &(
                            o_raw_content));
                }

                feed_heap_free(
                    p_main_context->p_client->p_heap,
                    (void *)(
                        p_raw_buffer));
            }
        }
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

