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

            o_feed_descriptor.p_complete =
                NULL;

            o_feed_descriptor.p_notify =
                NULL;
        }

        p_feed_handle =
            feed_create(
                &(
                    o_feed_descriptor));
    }

    if (
        p_feed_handle)
    {
        if (
            argc > 1u)
        {
            FILE * const
                p_file_handle =
                fopen(
                    argv[1u],
                    "rt");

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

                fclose(
                    p_file_handle);
            }
        }
        else
        {
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

        feed_start(
            p_feed_handle);

#if 0
        /* Note: get text buffer to flush */
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
#endif

        feed_destroy(
            p_feed_handle);
    }

    return
        0;

}

