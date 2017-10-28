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


struct feed_glyph
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    /* Raw representation of character */
    unsigned char
        a_raw[7u];

    /* Length in bytes of raw character */
    unsigned char
        i_raw_length;

    /* Visible representation of character */
    unsigned char
        a_visible[30u];

    /* Length in bytes of visible character */
    unsigned char
        i_visible_length;

    /* Width in columns of visible character */
    unsigned char
        i_visible_width;

}; /* struct feed_glyph */


struct feed_line
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_list
        o_glyphs;

    /* Number of characters */
    unsigned int
        i_glyph_count;

    unsigned int
        a_padding[3u];

}; /* struct feed_line */


struct feed_text
{
    struct feed_client *
        p_client;

    struct feed_list
        o_lines;

    struct feed_line *
        p_prompt;

    struct feed_event
        o_last_event;

    /* Number of lines */
    unsigned int
        i_line_count;

    /* Cursor */
    unsigned int
        i_cursor_line_index;

    unsigned int
        i_cursor_glyph_index;

    unsigned int
        a_padding[1u];

}; /* struct feed_text */


static
void
feed_glyph_init(
    struct feed_glyph * const
        p_glyph,
    struct feed_client * const
        p_client)
{
    feed_list_init(
        &(
            p_glyph->o_list));

    p_glyph->p_client =
        p_client;

    p_glyph->i_raw_length =
        0u;

    p_glyph->i_visible_length =
        0u;

    p_glyph->i_visible_width =
        0u;

} /* feed_glyph_init() */


static
void
feed_glyph_cleanup(
    struct feed_glyph * const
        p_glyph)
{
    feed_list_join(
        &(
            p_glyph->o_list),
        &(
            p_glyph->o_list));

} /* feed_glyph_cleanup() */


static
struct feed_glyph *
feed_glyph_create(
    struct feed_client * const
        p_client)
{
    struct feed_glyph *
        p_glyph;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_glyph =
        (struct feed_glyph *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_glyph)));

    if (
        p_glyph)
    {
        feed_glyph_init(
            p_glyph,
            p_client);
    }

    return
        p_glyph;

}


static
void
feed_glyph_destroy(
    struct feed_glyph * const
        p_glyph)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_glyph->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_glyph_cleanup(
        p_glyph);

    feed_heap_free(
        p_heap,
        (void *)(
            p_glyph));

}


static
void
feed_glyph_write_event(
    struct feed_glyph * const
        p_glyph,
    struct feed_event const * const
        p_event)
{
    memcpy(
        p_glyph->a_raw,
        p_event->a_raw,
        p_event->i_raw_len);

    p_glyph->i_raw_length =
        p_event->i_raw_len;

    if (
        FEED_EVENT_KEY_FLAG & p_event->i_code)
    {
        struct feed_buf
            o_buf;

        memset(
            p_glyph->a_visible,
            0u,
            sizeof(
                p_glyph->a_visible));

        p_glyph->a_visible[0u] =
            '<';

        feed_buf_init(
            &(
                o_buf),
            p_glyph->a_visible + 1u,
            sizeof(
                p_glyph->a_visible) - 1u);

        feed_input_print(
            p_event,
            &(
                o_buf));

        p_glyph->a_visible[1u + o_buf.i_len] =
            '>';

        p_glyph->i_visible_length =
            (unsigned char)(
                o_buf.i_len + 2u);

        p_glyph->i_visible_width =
            (unsigned char)(
                o_buf.i_len + 2u);
    }
    else
    {
        memcpy(
            p_glyph->a_visible,
            p_event->a_raw,
            p_event->i_raw_len);

        p_glyph->i_visible_length =
            p_event->i_raw_len;

        p_glyph->i_visible_width =
            1u;

    }

}


static
void
feed_line_init(
    struct feed_line * const
        p_line,
    struct feed_client * const
        p_client)
{
    feed_list_init(
        &(
            p_line->o_list));

    p_line->p_client =
        p_client;

    feed_list_init(
        &(
            p_line->o_glyphs));

    p_line->i_glyph_count =
        0u;

}

static
void
feed_line_reset(
    struct feed_line * const
        p_line)
{
    struct feed_list *
        p_iterator;

    struct feed_list *
        p_next;

    struct feed_glyph *
        p_glyph;

    /* Free all characters... */
    p_iterator =
        p_line->o_glyphs.p_next;

    while (
        p_iterator != &(p_line->o_glyphs))
    {
        p_next =
            p_iterator->p_next;

        p_glyph =
            (struct feed_glyph *)(
                p_iterator);

        feed_glyph_destroy(
            p_glyph);

        p_iterator =
            p_next;

    }

    p_line->i_glyph_count =
        0u;

}

static
void
feed_line_cleanup(
    struct feed_line * const
        p_line)
{
    feed_list_join(
        &(
            p_line->o_list),
        &(
            p_line->o_list));

    /* Free all characters... */
    feed_line_reset(
        p_line);

}


static
struct feed_line *
feed_line_create(
    struct feed_client * const
        p_client)
{
    struct feed_line *
        p_line;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_line =
        (struct feed_line *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_line)));

    if (
        p_line)
    {
        feed_line_init(
            p_line,
            p_client);
    }

    return
        p_line;

}


static
void
feed_line_destroy(
    struct feed_line * const
        p_line)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_line->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_line_cleanup(
        p_line);

    feed_heap_free(
        p_heap,
        (void *)(
            p_line));

}


static
void
feed_line_write_event(
    struct feed_line * const
        p_line,
    struct feed_event const * const
        p_event)
{
    struct feed_client *
        p_client;

    struct feed_glyph *
        p_glyph;

    p_client =
        p_line->p_client;

    /* Create a character */
    p_glyph =
        feed_glyph_create(
            p_client);

    if (
        p_glyph)
    {
        /* Set char information */
        feed_glyph_write_event(
            p_glyph,
            p_event);

        /* Store the char into the list */
        feed_list_join(
            &(
                p_glyph->o_list),
            &(
                p_line->o_glyphs));

        p_line->i_glyph_count ++;
    }
}


static
char
feed_text_init(
    struct feed_text * const
        p_text,
    struct feed_client * const
        p_client)
{
    char
        b_result;

    memset(
        p_text,
        0x00u,
        sizeof(
            struct feed_text));

    p_text->p_client =
        p_client;

    feed_list_init(
        &(
            p_text->o_lines));

    p_text->i_line_count =
        0;

    p_text->i_cursor_line_index =
        0;

    p_text->i_cursor_glyph_index =
        0;

    p_text->p_prompt =
        feed_line_create(
            p_client);

    if (
        p_text->p_prompt)

    {
        struct feed_line *
            p_line;

        /* Create a line */
        p_line =
            feed_line_create(
                p_client);

        /* Store the line into the list */
        if (p_line)
        {
            feed_list_join(
                &(
                    p_line->o_list),
                &(
                    p_text->o_lines));

            p_text->i_line_count ++;

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

    return
        b_result;

}


static
void
feed_text_cleanup(
    struct feed_text * const
        p_text)
{
    /* Delete all the lines */
    struct feed_list *
        p_iterator;

    p_iterator =
        p_text->o_lines.p_next;

    while (
        p_iterator
        != &(
            p_text->o_lines))
    {
        struct feed_list *
            p_next;

        struct feed_line *
            p_line;

        p_next =
            p_iterator->p_next;

        p_line =
            (struct feed_line *)(
                p_iterator);

        feed_line_destroy(
            p_line);

        p_iterator =
            p_next;

    }

    if (
        p_text->p_prompt)
    {
        feed_line_destroy(
            p_text->p_prompt);

        p_text->p_prompt =
            (struct feed_line *)(
                0);
    }

} /* feed_text_cleanup() */


static
struct feed_text *
feed_text_create(
    struct feed_client * const
        p_client)
{
    struct feed_text *
        p_text;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_text =
        (struct feed_text *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_text)));

    if (
        p_text)
    {
        if (
            feed_text_init(
                p_text,
                p_client))
        {
        }
        else
        {
            feed_heap_free(
                p_heap,
                (void *)(
                    p_text));

            p_text =
                (struct feed_text *)(
                    0);
        }
    }

    return
        p_text;

}


static
void
feed_text_destroy(
    struct feed_text * const
        p_text)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_text->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_text_cleanup(
        p_text);

    feed_heap_free(
        p_heap,
        (void *)(
            p_text));

}


static
void
feed_text_write_event(
    struct feed_text * const
        p_text,
    struct feed_event const * const
        p_event)
{
#if 0
    /* Store the character into the body */
    if (p_text->o_lines.p_next
        == &(p_text->o_lines))
    {
        struct feed_client *
            p_client;

        struct feed_line *
            p_line;

        p_client =
            p_text->p_client;

        /* Create a line */
        p_line =
            feed_line_create(
                p_client);

        /* Store the line into the list */
        if (p_line)
        {
            feed_list_join(
                &(
                    p_line->o_list),
                &(
                    p_text->o_lines));
        }
    }
#endif

    /* Get last line */
    if (p_text->o_lines.p_prev
        != &(p_text->o_lines))
    {
        struct feed_line *
            p_line;

        p_line =
            (struct feed_line *)(
                p_text->o_lines.p_prev);

        feed_line_write_event(
            p_line,
            p_event);

        p_text->i_cursor_glyph_index ++;
    }

}


/* Create a char and add to prompt line */
static
void
feed_text_prompt_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    if (
        p_context)
    {
        struct feed_text *
            p_text;

        p_text =
            (struct feed_text *)(
                p_context);

        if (
            p_text->p_prompt)
        {
            feed_line_write_event(
                p_text->p_prompt,
                p_event);
        }
    }

} /* feed_text_prompt_callback() */

static
char
feed_text_prompt(
    struct feed_text * const
        p_text,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    char
        b_result;

    /* Delete the previous prompt */
    if (p_text->p_prompt)
    {
        struct feed_input *
            p_input;

        feed_line_reset(
            p_text->p_prompt);

        p_input =
            feed_input_create(
                p_text->p_client,
                &(
                    feed_text_prompt_callback),
                p_text);

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
            0;
    }

    return
        b_result;

}

static
void
feed_main_print_status(
    struct feed_event const * const
        p_event)
{
    unsigned char i;
    unsigned char c;

    printf("%08lx: [", p_event->i_code);
    for (i=0u; i<p_event->i_raw_len; i++)
    {
        c = p_event->a_raw[i];

        if ((c >= 32) && (c < 127))
        {
            printf(" '%c'", (char)(c));
        }
        else
        {
            printf(" %3u", (unsigned int)(p_event->a_raw[i]));
        }
    }
    printf(" ]");

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

        printf(" <%.*s>", (int)(o_name.i_len), (char const *)(o_name.p_buf));
    }
    printf("\033[0K");
}

static
void
feed_text_refresh(
    struct feed_text * const
        p_text,
    unsigned int const
        i_screen_height,
    unsigned int const
        i_screen_width)
{
    unsigned int
        i_width;

    unsigned int
        i_height;

    unsigned int
        i_cursor_visible_x;

    unsigned int
        i_cursor_glyph_iterator;

    (void)(
        i_screen_height);

    i_width =
        0u;

    i_height =
        0u;

    i_cursor_visible_x =
        0u;

    i_cursor_glyph_iterator =
        0u;

    /* Grow size of drawing region */

    /* Move cursor to beginning of drawing region */

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

                    i_width += p_glyph->i_visible_width;

                    p_glyph_iterator =
                        p_glyph_iterator->p_next;
                }

                i_cursor_visible_x =
                    i_width;
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

                i_width += p_glyph->i_visible_width;

                if (i_cursor_glyph_iterator < p_text->i_cursor_glyph_index)
                {
                    i_cursor_visible_x = i_width;
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
        printf("\r\nstatus: ");

        feed_main_print_status(
            &(
                p_text->o_last_event));

        /* Position the cursor */
        printf("\r\033[A");
        if (i_cursor_visible_x)
        {
            printf("\033[%dC", i_cursor_visible_x);
        }
    }

}


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

    char
        b_more;

    unsigned char
        a_padding[7u];

};

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
            p_text,
            p_main_context->i_wy,
            p_main_context->i_wx);
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
            'w', 'h', 'a', 'd', 'y', 'a', 'w', 'a', 'n', 't', '?', ' '
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
                    p_main_context->p_text,
                    p_main_context->i_wy,
                    p_main_context->i_wx);

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

