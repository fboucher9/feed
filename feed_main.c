/* See LICENSE for license details */

#include "feed_os.h"

#include "feed_main.h"

#include "feed_client.h"

#include "feed_tty.h"


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

int
feed_main(
    unsigned int const
        argc,
    char const * const * const
        argv)
{
    struct feed_client
        o_client;

    struct feed_tty
        o_tty;

    (void)(
        argc);
    (void)(
        argv);

    if (
        feed_tty_init(
            &(
                o_client),
            &(
                o_tty)))
    {
        if (
            feed_tty_enable(
                &(
                    o_client),
                &(
                    o_tty)))
        {
            int
                x;

            int
                y;

            if (
                feed_tty_get_cursor_position(
                    &(
                        o_client),
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
                    &(
                        o_client),
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
                    &(
                        o_client),
                    &(
                        o_tty),
                    1);

                feed_tty_write_character_array(
                    &(
                        o_client),
                    &(
                        o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    &(
                        o_client),
                    &(
                        o_tty),
                    20);

                /* test line wrap disable */
                feed_tty_line_wrap(
                    &(
                        o_client),
                    &(
                        o_tty),
                    0);

                feed_tty_write_character_array(
                    &(
                        o_client),
                    &(
                        o_tty),
                    g_test_line_wrap_enable,
                    sizeof(
                        g_test_line_wrap_enable));

                feed_tty_move_cursor_backward(
                    &(
                        o_client),
                    &(
                        o_tty),
                    20);
            }
#endif

#if 0

            feed_tty_move_cursor_up(
                &(
                    o_client),
                &(
                    o_tty),
                3);

            feed_tty_move_cursor_forward(
                &(
                    o_client),
                &(
                    o_tty),
                40);

#endif

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
                            &(
                                o_client),
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

            feed_tty_disable(
                &(
                    o_client),
                &(
                    o_tty));
        }
        else
        {
            feed_dbg_print(
                "enable error!");
        }

        feed_tty_cleanup(
            &(
                o_client),
            &(
                o_tty));
    }
    else
    {
        feed_dbg_print(
            "init error!");
    }

    return
        0;

}

