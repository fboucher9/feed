/* See LICENSE for license details */

#include "feed_os.h"

#include "feed.h"

#include "feed_main.h"

#if 0

/* MOVE ME */
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

}
#endif

static
int
feed_main_notify_callback(
    void * const
        p_context,
    struct feed_handle * const
        p_feed_handle,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    (void)(
        p_context);
    (void)(
        p_feed_handle);
    (void)(
        p_data);
    (void)(
        i_data_length);

#if 0 /* test of stop */
    if (i_data_length)
    {
        /* Detect that Ctrl-M was pressed */
        if (('\r' == p_data[0u])
            || ('\n' == p_data[0u]))
        {
            feed_stop(
                p_feed_handle);
        }
    }
#endif /* test of stop */

#if 0 /* test of complete */
    if (i_data_length)
    {
        /* Detect that Ctrl-I was pressed */
        if (9 == p_data[0u])
        {
            unsigned long int
                i_cursor_offset;

            unsigned long int
                i_total_length;

            i_cursor_offset =
                feed_cursor(
                    p_feed_handle);

            i_total_length =
                feed_length(
                    p_feed_handle);

            (void)(
                i_total_length);

            feed_complete(
                p_feed_handle,
                i_cursor_offset,
                0);

            {
                /* test of suggestions */
                static unsigned char const s_suggest1[] =
                { 'a', 'b', 'c' };

                feed_suggest(
                    p_feed_handle,
                    s_suggest1,
                    sizeof(s_suggest1),
                    i_cursor_offset + 1);
            }

            {
                /* test of suggestions */
                static unsigned char const s_suggest2[] =
                { 'x', 'y', 'z' };

                feed_suggest(
                    p_feed_handle,
                    s_suggest2,
                    sizeof(s_suggest2),
                    i_cursor_offset + 3);
            }
        }
    }
#endif /* test of complete */

#if 0 /* test of consume */
    if (i_data_length)
    {
        if (9 == p_data[0u])
        {
            unsigned char a_data[3u];

            unsigned long int
                i_data_len;

            i_data_len =
                feed_consume(
                    p_feed_handle,
                    a_data,
                    (unsigned long int)(
                        sizeof(
                            a_data)));

            printf(
                "consumed : %lu bytes [%.*s]\n",
                i_data_len,
                (int)(
                    i_data_len),
                (char const *)(
                    a_data));

            return -1;
        }
    }
#endif /* test of consume */

#if 0 /* test of cursor */
    if (i_data_length)
    {
        if (7 == p_data[0u])
        {
            unsigned long int
                i_cursor_offset;

            i_cursor_offset =
                feed_cursor(
                    p_feed_handle);

            printf(
                "cursor -> %lu\n",
                i_cursor_offset);

            return -1;
        }
    }
#endif /* test of cursor */

#if 0 /* test of exec */
    if (i_data_length)
    {
        if (9 == p_data[0u])
        {
            static unsigned char const cmd[] =
            {
                'a',
                'b',
                'c'
            };

            int
                i_exec_result;

            i_exec_result =
                feed_exec(
                    p_feed_handle,
                    cmd,
                    (unsigned long int)(sizeof(cmd)));

            (void)(
                i_exec_result);

            return -1;
        }
    }
#endif /* test of exec */

    if (i_data_length)
    {
        if ((4 == p_data[0u])
            || (3 == p_data[0u]))
        {
            feed_stop(
                p_feed_handle);
        }
    }

    /* ... */

    return 0;
}

int
feed_main(
    unsigned int const
        argc,
    char const * const * const
        argv)
{
    /* Test the library */
    struct feed_handle *
        p_feed_handle;

    {
        struct feed_descriptor
            o_feed_descriptor;

        {
            o_feed_descriptor.p_context =
                NULL;

            o_feed_descriptor.p_notify =
                &(
                    feed_main_notify_callback);

            o_feed_descriptor.i_max_screen_width =
                0;

            o_feed_descriptor.i_max_screen_height =
                0;
        }

        p_feed_handle =
            feed_create(
                &(
                    o_feed_descriptor));
    }

    if (
        p_feed_handle)
    {
        unsigned long int
            i_save_length;

        unsigned char *
            p_save_buffer;

        if (
            (
                argc > 1u)
            || (
                !isatty(STDIN_FILENO)))
        {
            FILE *
                p_file_handle;

            if (
                (argc < 2u)
                || (
                    0
                    == strcmp(
                        argv[1u],
                        "-")))
            {
                p_file_handle =
                    stdin;
            }
            else
            {
                p_file_handle =
                fopen(
                    argv[1u],
                    "rt");
            }

            if (
                p_file_handle)
            {
                char
                    b_eof;

                b_eof =
                    0;

                while (
                    (!b_eof))
                {
                    unsigned char a_block[1024u];

                    int iResult;

                    iResult =
                        (int)(
                            fread(
                                a_block,
                                sizeof(a_block[0u]),
                                sizeof(a_block),
                                p_file_handle));

                    if (
                        iResult > 0)
                    {
                        feed_load(
                            p_feed_handle,
                            a_block,
                            (unsigned int)(
                                iResult));
                    }
                    else
                    {
                        b_eof =
                            1;
                    }
                }

                if (stdin != p_file_handle)
                {
                    fclose(
                        p_file_handle);
                }
            }
        }
        else
        {
            {
                static unsigned char const s_prompt1[] =
                {
                    'f', 'e', 'e', 'd', ' ', '>', ' '
                };

                feed_prompt1(
                    p_feed_handle,
                    s_prompt1,
                    sizeof(
                        s_prompt1));
            }

            {
                static unsigned char const s_prompt2[] =
                {
                    ' ',
                    '>',
                    ' '
                };

                feed_prompt2(
                    p_feed_handle,
                    s_prompt2,
                    sizeof(
                        s_prompt2));
            }
        }

        /* Setup of color theme */
        {
            feed_theme(
                p_feed_handle,
                feed_syntax_prompt1,
                feed_color_dark_yellow,
                feed_color_default);

            feed_theme(
                p_feed_handle,
                feed_syntax_prompt2,
                feed_color_dark_magenta,
                feed_color_default);

            feed_theme(
                p_feed_handle,
                feed_syntax_operator,
                feed_color_dark_black,
                feed_color_dark_white);
        }

        feed_start(
            p_feed_handle);

        /* Note: get text buffer to flush */
        {
            i_save_length =
                feed_length(
                    p_feed_handle);

            if (
                i_save_length)
            {
                p_save_buffer =
                    (unsigned char *)(
                        malloc(
                            i_save_length));
            }
            else
            {
                p_save_buffer =
                    (unsigned char *)(
                        0);
            }

            if (
                p_save_buffer)
            {
                if (
                    0
                    != feed_save(
                        p_feed_handle,
                        p_save_buffer,
                        i_save_length))
                {
                }
                else
                {
                    i_save_length =
                        0ul;
                }
            }
        }

        feed_destroy(
            p_feed_handle);

        /* Print the text buffer here. */
        {
            if (
                p_save_buffer)
            {
                if (
                    i_save_length)
                {
                    if (!isatty(STDOUT_FILENO))
                    {
                        fwrite(
                            p_save_buffer,
                            i_save_length,
                            1,
                            stdout);
                    }
                }

                free(
                    (void *)(
                        p_save_buffer));
            }
        }
    }

    return
        0;

}

