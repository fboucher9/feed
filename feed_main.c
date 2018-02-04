/* See LICENSE for license details */

#include "feed_os.h"

#include "feed.h"

#include "feed_main.h"

#include "feed_buf.h"

struct feed_options
{
    struct feed_buf const *
        p_file_name_buf;

    struct feed_buf const *
        p_geometry_value;

    unsigned char
        b_file_name;

    unsigned char
        b_geometry;

    unsigned char
        b_dumpkeys;

    unsigned char
        uc_padding[5u];

}; /* struct feed_options */

struct feed_main_context
{
    struct feed_options
        o_options;

    struct feed_descriptor
        o_descriptor;

    struct feed_handle *
        p_feed_handle;

    void *
        pv_padding[1u];

}; /* struct feed_main_context */

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
void
feed_main_dump_key(
    unsigned short int const
        i_code,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    printf("\"");

    if (i_code)
    {
        unsigned char
            c;

        c =
            (unsigned char)(
                i_code & 0xFFu);

        if (
            FEED_KEY_CTRL & i_code)
        {
            printf("<ctrl>-");
        }
        if (
            FEED_KEY_SHIFT & i_code)
        {
            printf("<shift>-");
        }
        if (
            FEED_KEY_ALT & i_code)
        {
            printf("<alt>-");
        }

        if (
            c < 32u)
        {
            printf("<.>");
        }
        else if (
            c < 127u)
        {
            printf("<%c>", (char)(c));
        }
        else
        {
            if (
                FEED_KEY_UP == c)
            {
                printf("<up>");
            }
            else if (
                FEED_KEY_DOWN == c)
            {
                printf("<down>");
            }
            else if (
                FEED_KEY_LEFT == c)
            {
                printf("<left>");
            }
            else if (
                FEED_KEY_RIGHT == c)
            {
                printf("<right>");
            }
            else if (
                FEED_KEY_INSERT == c)
            {
                printf("<insert>");
            }
            else if (
                FEED_KEY_DELETE == c)
            {
                printf("<delete>");
            }
            else if (
                FEED_KEY_HOME == c)
            {
                printf("<home>");
            }
            else if (
                FEED_KEY_END == c)
            {
                printf("<end>");
            }
            else if (
                FEED_KEY_PAGEUP == c)
            {
                printf("<pageup>");
            }
            else if (
                FEED_KEY_PAGEDOWN == c)
            {
                printf("<pagedown>");
            }
            else if (
                FEED_KEY_F1 == c)
            {
                printf("<f1>");
            }
            else if (
                FEED_KEY_F2 == c)
            {
                printf("<f2>");
            }
            else if (
                FEED_KEY_F3 == c)
            {
                printf("<f3>");
            }
            else if (
                FEED_KEY_F4 == c)
            {
                printf("<f4>");
            }
            else if (
                FEED_KEY_F5 == c)
            {
                printf("<f5>");
            }
            else if (
                FEED_KEY_F6 == c)
            {
                printf("<f6>");
            }
            else if (
                FEED_KEY_F7 == c)
            {
                printf("<f7>");
            }
            else if (
                FEED_KEY_F8 == c)
            {
                printf("<f8>");
            }
            else if (
                FEED_KEY_F9 == c)
            {
                printf("<f9>");
            }
            else if (
                FEED_KEY_F10 == c)
            {
                printf("<f10>");
            }
            else if (
                FEED_KEY_F11 == c)
            {
                printf("<f11>");
            }
            else if (
                FEED_KEY_F12 == c)
            {
                printf("<f12>");
            }
            else
            {
                printf("<0x%02x>", (unsigned int)(i_code & 0xFFu));
            }
        }
    }
    else
    {
        if (1ul == i_data_length)
        {
            unsigned char
                c;

            c = (p_data[0u]);

            if ((c >= 32u) &&
                (c < 127u))
            {
                printf("<%c>", (char)(c));
            }
        }
    }

    printf("\"");

    {
        unsigned long int
            i_data_iterator;

        printf(" {");
        for (
            i_data_iterator = 0ul;
            i_data_iterator < i_data_length;
            i_data_iterator ++)
        {
            unsigned char
                c;

            c = p_data[i_data_iterator];

            if (i_data_iterator)
            {
                printf(",");
            }

            if (
                c < 32u)
            {
                printf(
                    " %u",
                    (unsigned int)(c));
            }
            else if (
                c < 127u)
            {
                printf(
                    " \'%c\'",
                    (char)(c));
            }
            else
            {
                printf(
                    " 0x%02x",
                    (unsigned int)( c));
            }
        }

        printf(" }");
    }

    printf("\n");
}

static
int
feed_main_notify_callback(
    void * const
        p_context,
    struct feed_handle * const
        p_feed_handle,
    unsigned short int const
        i_code,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length)
{
    int
        i_result;

    struct feed_main_context *
        p_main_context;

    (void)(
        p_feed_handle);
    (void)(
        i_code);
    (void)(
        p_data);
    (void)(
        i_data_length);

    p_main_context =
        (struct feed_main_context *)(
            p_context);

    (void)(
        p_main_context);

    i_result =
        0;

    if (
        p_main_context->o_options.b_dumpkeys)
    {
        feed_main_dump_key(
            i_code,
            p_data,
            i_data_length);
    }

#if 0 /* test of cursor position */
    if (
        (FEED_KEY_CTRL | 'G') == i_code)
    {
        unsigned long int
            i_cursor_offset;

        i_cursor_offset =
            feed_cursor(
                p_feed_handle);

        printf("cursor offset | %10lu bytes\n",
            i_cursor_offset);

        i_result =
            -1;
    }
#endif /* test of cursor position */

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

            i_result =
                -1;
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

            i_result =
                -1;
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

            i_result =
                -1;
        }
    }
#endif /* test of exec */

    if (i_data_length)
    {
        if (((FEED_KEY_CTRL | 'D') == i_code)
            || ((FEED_KEY_CTRL | 'C') == i_code))
        {
            feed_stop(
                p_feed_handle);
        }
    }

    /* ... */

    return
        i_result;

}

static
char
feed_options_init(
    struct feed_options * const
        p_options,
    struct feed_main_descriptor const * const
        p_main_descriptor)
{
    char
        b_result;

    struct feed_main_descriptor
        o_iterator;

    o_iterator =
        *(
            p_main_descriptor);

    p_options->p_file_name_buf =
        (struct feed_buf const *)(
            0);

    p_options->p_geometry_value =
        (struct feed_buf const *)(
            0);

    p_options->b_file_name =
        0;

    p_options->b_geometry =
        0;

    p_options->b_dumpkeys =
        0;

    b_result =
        1;

    if (
        o_iterator.p_min < o_iterator.p_max)
    {
        /* Consume argv[0u] */
        o_iterator.p_min ++;

        while (
            b_result
            && (o_iterator.p_min < o_iterator.p_max))
        {
            if (
                ((o_iterator.p_min->o_min.pc + 1) < o_iterator.p_min->o_max.pc)
                && ('-' == o_iterator.p_min->o_min.pc[0u]))
            {
                static unsigned char const g_optname_geometry[] =
                {
                    '-',
                    'g'
                };

                struct feed_buf
                    o_optname_geometry;

                feed_buf_init(
                    &(
                        o_optname_geometry),
                    g_optname_geometry,
                    g_optname_geometry + sizeof(g_optname_geometry));

                if (
                    0
                    == feed_buf_compare(
                        o_iterator.p_min,
                        &(
                            o_optname_geometry)))
                {
                    p_options->b_geometry =
                        1;

                    o_iterator.p_min ++;

                    if (
                        o_iterator.p_min < o_iterator.p_max)
                    {
                        p_options->p_geometry_value =
                            o_iterator.p_min;

                        o_iterator.p_min ++;
                    }
                }
                else
                {
                    static unsigned char const g_optname_dumpkeys[] =
                    {
                        '-',
                        'd'
                    };

                    struct feed_buf
                        o_optname_dumpkeys;

                    feed_buf_init(
                        &(
                            o_optname_dumpkeys),
                        g_optname_dumpkeys,
                        g_optname_dumpkeys + sizeof(g_optname_dumpkeys));

                    if (
                        0
                        == feed_buf_compare(
                            o_iterator.p_min,
                            &(
                                o_optname_dumpkeys)))
                    {
                        p_options->b_dumpkeys =
                            1;

                        o_iterator.p_min ++;
                    }
                    else
                    {
                        printf(
                            "usage:\n"
                            " feed [options] [file|-]\n"
                            "options:\n"
                            " -g WxH     set geometry\n"
                            " -d         dump key codes\n"
                            );

                        b_result =
                            0;
                    }
                }
            }
            else
            {
                p_options->b_file_name =
                    1;

                p_options->p_file_name_buf =
                    o_iterator.p_min;

                o_iterator.p_min =
                    o_iterator.p_max;
            }
        }
    }

    return
        b_result;

}

static
void
init_feed_descriptor(
    struct feed_main_context * const
        p_main_context,
    struct feed_descriptor * const
        p_feed_descriptor)
{
    p_feed_descriptor->p_context =
        p_main_context;

    p_feed_descriptor->p_notify =
        &(
            feed_main_notify_callback);

    p_feed_descriptor->p_device_intf =
        (struct feed_device_intf *)(
            0);

    if (p_main_context->o_options.b_geometry)
    {
        /* Get width and height values */
        struct feed_buf
            o_geometry_parser;

        signed long int
            i_geometry_width;

        signed long int
            i_geometry_height;

        o_geometry_parser =
            *(
                p_main_context->o_options.p_geometry_value);

        i_geometry_width =
            0ul;

        feed_buf_read_number(
            &(
                o_geometry_parser),
            &(
                i_geometry_width));

        /* Skip the delimiter */
        o_geometry_parser.o_min.pc ++;

        i_geometry_height =
            0ul;

        feed_buf_read_number(
            &(
                o_geometry_parser),
            &(
                i_geometry_height));

        if (
            0ul
            == i_geometry_height)
        {
            i_geometry_height =
                i_geometry_width;

            i_geometry_width =
                0ul;
        }

        p_feed_descriptor->i_max_screen_width =
            (unsigned short int)(
                i_geometry_width);

        p_feed_descriptor->i_max_screen_height =
            (unsigned short int)(
                i_geometry_height);
    }
    else
    {
        p_feed_descriptor->i_max_screen_width =
            0;

        p_feed_descriptor->i_max_screen_height =
            0;
    }
}

int
feed_main(
    struct feed_main_descriptor const * const
        p_main_descriptor)
{
    /* Test the library */
    struct feed_main_context
        o_main_context;

    struct feed_main_context *
        p_main_context;

    p_main_context =
        &(
            o_main_context);

    if (
        feed_options_init(
            &(
                p_main_context->o_options),
            p_main_descriptor))
    {
        init_feed_descriptor(
            p_main_context,
            &(
                p_main_context->o_descriptor));

        p_main_context->p_feed_handle =
            feed_create(
                &(
                    p_main_context->o_descriptor));

        if (
            p_main_context->p_feed_handle)
        {
            unsigned long int
                i_save_length;

            unsigned char *
                p_save_buffer;

            if (
                p_main_context->o_options.b_file_name
                || (
                    !isatty(STDIN_FILENO)))
            {
                FILE *
                    p_file_handle;

                if (p_main_context->o_options.b_file_name)
                {
                    static unsigned char const g_stdin_file_name[] =
                    {
                        '-'
                    };

                    struct feed_buf
                        o_stdin_file_name_buf;

                    feed_buf_init(
                        &(
                            o_stdin_file_name_buf),
                        g_stdin_file_name,
                        g_stdin_file_name + sizeof(g_stdin_file_name));

                    if (
                        0
                        == feed_buf_compare(
                            p_main_context->o_options.p_file_name_buf,
                            &(
                                o_stdin_file_name_buf)))
                    {
                        p_file_handle =
                            stdin;
                    }
                    else
                    {
                        unsigned char *
                            p_file_name0;

                        unsigned long int
                            argl;

                        argl =
                            (unsigned long int)(
                                p_main_context->o_options.p_file_name_buf->o_max.pc
                                - p_main_context->o_options.p_file_name_buf->o_min.pc);

                        p_file_name0 =
                            (unsigned char *)(
                                malloc(
                                    argl + 1ul));

                        if (
                            p_file_name0)
                        {
                            memcpy(
                                p_file_name0,
                                p_main_context->o_options.p_file_name_buf->o_min.pc,
                                argl);

                            p_file_name0[argl] =
                                '\000';

                            p_file_handle =
                                fopen(
                                    (char const *)(
                                        p_file_name0),
                                    "rt");

                            free(
                                (void *)(
                                    p_file_name0));
                        }
                        else
                        {
                            p_file_handle =
                                stdin;
                        }
                    }

                    feed_buf_cleanup(
                        &(
                            o_stdin_file_name_buf));
                }
                else
                {
                    p_file_handle =
                        stdin;
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

                        signed long int iResult;

                        iResult =
                            (signed long int)(
                                fread(
                                    a_block,
                                    sizeof(a_block[0u]),
                                    sizeof(a_block),
                                    p_file_handle));

                        if (
                            iResult > 0)
                        {
                            feed_load(
                                p_main_context->p_feed_handle,
                                a_block,
                                (unsigned long int)(
                                    iResult));

                            if (feof(p_file_handle))
                            {
                                b_eof =
                                    1;
                            }
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
                        p_main_context->p_feed_handle,
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
                        p_main_context->p_feed_handle,
                        s_prompt2,
                        sizeof(
                            s_prompt2));
                }
            }

            /* Setup of color theme */
            {
                feed_theme(
                    p_main_context->p_feed_handle,
                    feed_syntax_prompt1,
                    feed_color_dark_yellow,
                    feed_color_default);

                feed_theme(
                    p_main_context->p_feed_handle,
                    feed_syntax_prompt2,
                    feed_color_dark_magenta,
                    feed_color_default);

                feed_theme(
                    p_main_context->p_feed_handle,
                    feed_syntax_operator,
                    feed_color_dark_black,
                    feed_color_dark_white);

                feed_theme(
                    p_main_context->p_feed_handle,
                    feed_syntax_suggest,
                    feed_color_dark_green,
                    feed_color_default);
            }

            feed_start(
                p_main_context->p_feed_handle);

            /* Note: get text buffer to flush */
            {
                i_save_length =
                    feed_length(
                        p_main_context->p_feed_handle);

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
                            p_main_context->p_feed_handle,
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
                p_main_context->p_feed_handle);

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
    }

    return
        0;

}

