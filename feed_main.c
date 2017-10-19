/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_main.h"

#include "feed_client.h"

#include "feed_tty.h"

#include "feed_input.h"

#include "feed_heap.h"

#include "feed_keys.h"

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

static
void
feed_main_event_callback(
    void * const
        p_context,
    struct feed_event const * const
        p_event)
{
    char *
        p_more;

    unsigned char
        i;

    unsigned char
        c;

    p_more =
        (char *)(
            p_context);

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

        memset(a_name, 0, sizeof(a_name));

        if (
            feed_input_print(
                p_event,
                a_name,
                (unsigned int)(sizeof(a_name) - 1u)))
        {
            printf(" <%s>", a_name);
        }
    }

    if ((unsigned long int)(unsigned char)('q') == p_event->i_code)
    {
        *(p_more) =
            0;
    }

    printf("\r\n");
}

int
feed_main(
    unsigned int const
        argc,
    char const * const * const
        argv)
{
    struct feed_client
        o_client;

    struct feed_client *
        p_client;

    struct feed_tty
        o_tty;

    struct feed_heap *
        p_heap;

    (void)(
        argc);
    (void)(
        argv);

    p_client =
        &(
            o_client);

    feed_client_init(
        p_client);

    p_heap =
        feed_heap_create(
            p_client);

    if (
        feed_tty_init(
            p_client,
            &(
                o_tty)))
    {
        if (
            feed_tty_enable(
                p_client,
                &(
                    o_tty)))
        {
            int
                x;

            int
                y;

            if (
                feed_tty_get_cursor_position(
                    p_client,
                    &(
                        o_tty),
                    &(
                        x),
                    &(
                        y)))
            {
                feed_dbg_print(
                    "pos = %d , %d",
                    x,
                    y);
            }
            else
            {
                feed_dbg_print(
                    "get_cursor_position error!");
            }

            if (
                feed_tty_get_window_size(
                    p_client,
                    &(
                        o_tty),
                    &(
                        x),
                    &(
                        y)))
            {
                feed_dbg_print(
                    "winsize = %d , %d",
                    x,
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
                    p_client,
                    &(
                        o_tty),
                    1);

                feed_tty_write_character_array(
                    p_client,
                    &(
                        o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_client,
                    &(
                        o_tty),
                    20);

                /* test line wrap disable */
                feed_tty_line_wrap(
                    p_client,
                    &(
                        o_tty),
                    0);

                feed_tty_write_character_array(
                    p_client,
                    &(
                        o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    p_client,
                    &(
                        o_tty),
                    20);
            }
#endif

#if 0

            feed_tty_move_cursor_up(
                p_client,
                &(
                    o_tty),
                3);

            feed_tty_move_cursor_forward(
                p_client,
                &(
                    o_tty),
                40);

#endif

            if (0)
            {
                char
                    b_more;

                b_more =
                    1;

                while (
                    b_more)
                {
                    unsigned char
                        a_escape[64u];

                    unsigned int
                        i_escape_len;

                    if (
                        feed_tty_read_escape_sequence(
                            p_client,
                            &(
                                o_tty),
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
                                b_more = 0;
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
                char
                    b_more;

                struct feed_input *
                    p_input;

                b_more =
                    1;

                p_input =
                    feed_input_create(
                        p_client);

                while (
                    b_more)
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
                                    c),
                                &(
                                    feed_main_event_callback),
                                &(
                                    b_more)))
                        {
                        }
                        else
                        {
                            b_more =
                                0;
                        }
                    }
                    else
                    {
                        b_more =
                            0;
                    }
                }

                feed_input_destroy(
                    p_input);
            }

            feed_tty_disable(
                p_client,
                &(
                    o_tty));
        }
        else
        {
            feed_dbg_print(
                "enable error!");
        }

        feed_tty_cleanup(
            p_client,
            &(
                o_tty));
    }
    else
    {
        feed_dbg_print(
            "init error!");
    }

    feed_heap_destroy(
        p_heap);

    feed_client_cleanup(
        p_client);

    return
        0;

}

