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


struct feed_body_char
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

}; /* struct feed_body_char */


struct feed_body_line
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_list
        o_chars;

    /* Number of characters */

}; /* struct feed_body_line */


struct feed_body_text
{
    struct feed_client *
        p_client;

    struct feed_list
        o_lines;

    struct feed_event
        o_last_event;

    /* Number of lines */

    /* Cursor */
    int
        i_cursor_line_index;

    int
        i_cursor_char_index;

}; /* struct feed_body_text */


static
void
feed_body_char_init(
    struct feed_body_char * const
        p_body_char,
    struct feed_client * const
        p_client)
{
    feed_list_init(
        &(
            p_body_char->o_list));

    p_body_char->p_client =
        p_client;

    p_body_char->i_raw_length =
        0u;

    p_body_char->i_visible_length =
        0u;

    p_body_char->i_visible_width =
        0u;

} /* feed_body_char_init() */


static
void
feed_body_char_cleanup(
    struct feed_body_char * const
        p_body_char)
{
    feed_list_join(
        &(
            p_body_char->o_list),
        &(
            p_body_char->o_list));

} /* feed_body_char_cleanup() */


static
struct feed_body_char *
feed_body_char_create(
    struct feed_client * const
        p_client)
{
    struct feed_body_char *
        p_body_char;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_body_char =
        (struct feed_body_char *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_body_char)));

    if (
        p_body_char)
    {
        feed_body_char_init(
            p_body_char,
            p_client);
    }

    return
        p_body_char;

}


static
void
feed_body_char_destroy(
    struct feed_body_char * const
        p_body_char)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_body_char->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_body_char_cleanup(
        p_body_char);

    feed_heap_free(
        p_heap,
        (void *)(
            p_body_char));

}


static
void
feed_body_char_write_event(
    struct feed_body_char * const
        p_body_char,
    struct feed_event const * const
        p_event)
{
    memcpy(
        p_body_char->a_raw,
        p_event->a_raw,
        p_event->i_raw_len);

    p_body_char->i_raw_length =
        p_event->i_raw_len;

    memcpy(
        p_body_char->a_visible,
        p_event->a_raw,
        p_event->i_raw_len);

    p_body_char->i_visible_length =
        p_event->i_raw_len;

    p_body_char->i_visible_width =
        1u;

}


static
void
feed_body_line_init(
    struct feed_body_line * const
        p_body_line,
    struct feed_client * const
        p_client)
{
    feed_list_init(
        &(
            p_body_line->o_list));

    p_body_line->p_client =
        p_client;

    feed_list_init(
        &(
            p_body_line->o_chars));

}

static
void
feed_body_line_cleanup(
    struct feed_body_line * const
        p_body_line)
{
    struct feed_list *
        p_iterator;

    struct feed_list *
        p_next;

    struct feed_body_char *
        p_body_char;

    feed_list_join(
        &(
            p_body_line->o_list),
        &(
            p_body_line->o_list));

    /* Free all characters... */
    p_iterator =
        p_body_line->o_chars.p_next;

    while (
        p_iterator != &(p_body_line->o_chars))
    {
        p_next =
            p_iterator->p_next;

        p_body_char =
            (struct feed_body_char *)(
                p_iterator);

        feed_body_char_destroy(
            p_body_char);

        p_iterator =
            p_next;

    }
}


static
struct feed_body_line *
feed_body_line_create(
    struct feed_client * const
        p_client)
{
    struct feed_body_line *
        p_body_line;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_body_line =
        (struct feed_body_line *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_body_line)));

    if (
        p_body_line)
    {
        feed_body_line_init(
            p_body_line,
            p_client);
    }

    return
        p_body_line;

}


static
void
feed_body_line_destroy(
    struct feed_body_line * const
        p_body_line)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_body_line->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_body_line_cleanup(
        p_body_line);

    feed_heap_free(
        p_heap,
        (void *)(
            p_body_line));

}


static
void
feed_body_line_write_event(
    struct feed_body_line * const
        p_body_line,
    struct feed_event const * const
        p_event)
{
    struct feed_client *
        p_client;

    struct feed_body_char *
        p_body_char;

    p_client =
        p_body_line->p_client;

    if (0ul == (0x80000000ul & p_event->i_code))
    {
        /* Create a character */
        p_body_char =
            feed_body_char_create(
                p_client);

        if (
            p_body_char)
        {
            /* Set char information */
            feed_body_char_write_event(
                p_body_char,
                p_event);

            /* Store the char into the list */
            feed_list_join(
                &(
                    p_body_char->o_list),
                &(
                    p_body_line->o_chars));
        }
    }
}


static
void
feed_body_text_init(
    struct feed_body_text * const
        p_body_text,
    struct feed_client * const
        p_client)
{
    p_body_text->p_client =
        p_client;

    feed_list_init(
        &(
            p_body_text->o_lines));

    p_body_text->i_cursor_line_index =
        0;

    p_body_text->i_cursor_char_index =
        0;

}


static
void
feed_body_text_cleanup(
    struct feed_body_text * const
        p_body_text)
{
    /* Delete all the lines */
    struct feed_list *
        p_iterator;

    p_iterator =
        p_body_text->o_lines.p_next;

    while (
        p_iterator
        != &(
            p_body_text->o_lines))
    {
        struct feed_list *
            p_next;

        struct feed_body_line *
            p_body_line;

        p_next =
            p_iterator->p_next;

        p_body_line =
            (struct feed_body_line *)(
                p_iterator);

        feed_body_line_destroy(
            p_body_line);

        p_iterator =
            p_next;

    }
}


static
struct feed_body_text *
feed_body_text_create(
    struct feed_client * const
        p_client)
{
    struct feed_body_text *
        p_body_text;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_client_get_heap(
            p_client);

    p_body_text =
        (struct feed_body_text *)(
            feed_heap_alloc(
                p_heap,
                sizeof(
                    struct feed_body_text)));

    if (
        p_body_text)
    {
        feed_body_text_init(
            p_body_text,
            p_client);
    }

    return
        p_body_text;

}


static
void
feed_body_text_destroy(
    struct feed_body_text * const
        p_body_text)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_body_text->p_client;

    p_heap =
        feed_client_get_heap(
            p_client);

    feed_body_text_cleanup(
        p_body_text);

    feed_heap_free(
        p_heap,
        (void *)(
            p_body_text));

}


static
void
feed_body_text_write_event(
    struct feed_body_text * const
        p_body_text,
    struct feed_event const * const
        p_event)
{
    struct feed_client *
        p_client;

    p_client =
        p_body_text->p_client;

    if (0ul == (0x80000000ul & p_event->i_code))
    {
        /* Store the character into the body */
        if (p_body_text->o_lines.p_next
            == &(p_body_text->o_lines))
        {
            struct feed_body_line *
                p_body_line;

            /* Create a line */
            p_body_line =
                feed_body_line_create(
                    p_client);

            /* Store the line into the list */
            if (p_body_line)
            {
                feed_list_join(
                    &(
                        p_body_line->o_list),
                    &(
                        p_body_text->o_lines));
            }
        }

        /* Get last line */
        if (p_body_text->o_lines.p_prev
            != &(p_body_text->o_lines))
        {
            struct feed_body_line *
                p_body_line;

            p_body_line =
                (struct feed_body_line *)(
                    p_body_text->o_lines.p_prev);

            feed_body_line_write_event(
                p_body_line,
                p_event);

            p_body_text->i_cursor_char_index ++;
        }
    }

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
feed_body_text_refresh(
    struct feed_body_text * const
        p_body_text)
{
    /* Grow size of drawing region */

    /* Move cursor to beginning of drawing region */

    /* Print state of body text */
    {
        struct feed_list *
            p_line_iterator;

        struct feed_list *
            p_char_iterator;

        /* Return to home position */
        printf("\r");

        /* For all lines in body text */
        p_line_iterator =
            p_body_text->o_lines.p_next;

        while (
            p_line_iterator
            != &(
                p_body_text->o_lines))
        {
            struct feed_body_line *
                p_body_line;

            p_body_line =
                (struct feed_body_line *)(
                    p_line_iterator);

            /* If line is within refresh window */

            /* For all chars in line */
            p_char_iterator =
                p_body_line->o_chars.p_next;

            while (
                p_char_iterator
                != &(
                    p_body_line->o_chars))
            {
                struct feed_body_char const *
                    p_body_char;

                p_body_char =
                    (struct feed_body_char const *)(
                        p_char_iterator);

                /* If char is within refresh window */

                printf("%.*s",
                    (int)(p_body_char->i_visible_length),
                    p_body_char->a_visible);

                p_char_iterator =
                    p_char_iterator->p_next;
            }

            p_line_iterator =
                p_line_iterator->p_next;
        }

        /* Erase to end-of-line */
        printf("\033[0K");

        /* Draw status line */
        printf("\r\nstatus: ");

        feed_main_print_status(
            &(
                p_body_text->o_last_event));

        /* Position the cursor */
        printf("\r\033[A");
        if (p_body_text->i_cursor_char_index)
        {
            printf("\033[%dC", p_body_text->i_cursor_char_index);
        }
    }

}


struct feed_main_context
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    struct feed_body_text *
        p_body_text;

    struct feed_client
        o_client;

    struct feed_tty
        o_tty;

    int
        i_ocx;

    int
        i_ocy;

    int
        i_wx;

    int
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

    struct feed_body_text *
        p_body_text;

    p_main_context =
        (struct feed_main_context *)(
            p_context);

    /* ((unsigned long int)(unsigned char)('q') == p_event->i_code) */
    if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'D') == p_event->i_code)
    {
        p_main_context->b_more =
            0;
    }

    p_body_text =
        p_main_context->p_body_text;

    p_body_text->o_last_event =
        *(
            p_event);

    if ((FEED_EVENT_KEY_FLAG | FEED_EVENT_KEY_CTRL | 'H') == p_event->i_code)
    {
        struct feed_body_line *
            p_body_line;

        struct feed_body_char *
            p_body_char;

        /* Find last line */
        if (p_body_text->o_lines.p_prev !=
            &(p_body_text->o_lines))
        {
            p_body_line =
                (struct feed_body_line *)(
                    p_body_text->o_lines.p_prev);

            /* Find last char */
            if (p_body_line->o_chars.p_prev !=
                &(p_body_line->o_chars))
            {
                /* Delete the selected char */
                p_body_char =
                    (struct feed_body_char *)(
                        p_body_line->o_chars.p_prev);

                feed_body_char_destroy(
                    p_body_char);

                p_body_text->i_cursor_char_index --;
            }
        }
    }
    else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_LEFT) == p_event->i_code)
    {
        if (p_body_text->i_cursor_char_index)
        {
            p_body_text->i_cursor_char_index --;
        }
    }
    else if ((FEED_EVENT_KEY_FLAG | FEED_KEY_RIGHT) == p_event->i_code)
    {
        p_body_text->i_cursor_char_index ++;
    }
    else
    {
        feed_body_text_write_event(
            p_body_text,
            p_event);
    }

    feed_body_text_refresh(
        p_body_text);

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
        feed_heap_create(
            p_main_context->p_client);

    p_main_context->p_body_text =
        feed_body_text_create(
            p_main_context->p_client);

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
                    x;

                p_main_context->i_ocy =
                    y;
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
                    x;

                p_main_context->i_wy =
                    y;
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

    feed_body_text_destroy(
        p_main_context->p_body_text);

    feed_heap_destroy(
        p_main_context->p_heap);

    feed_client_cleanup(
        p_main_context->p_client);

    return
        0;

}

