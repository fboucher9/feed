/* See LICENSE for license details */

/*

Module: feed_device_std.c

Description:

    Low-level layer for access to tty device.  At this level, the interface is
    minimal, so no knowledge of escape sequences and there's no caching of
    input or output.

*/

#include "feed_os.h"

#if defined FEED_OS_LINUX

#include "feed_cfg.h"

#include "feed_device_std.h"

#include "feed_object.h"

#include "feed.h"

struct feed_device_std
{
    struct feed_client *
        p_client;

    struct termios *
        p_termios;

    /* -- */

    struct feed_device_std_descriptor
        o_descriptor;

    /* -- */

    char
        b_enabled;

    unsigned char
        uc_padding[7u];

    /* -- */

    void *
        a_termios_storage[
            (sizeof(struct termios) + sizeof(void *) - 1)
            / sizeof(void *)];

}; /* struct feed_device_std */

static
char
feed_device_std_init(
    struct feed_client * const
        p_client,
    struct feed_device_std * const
        p_device_std,
    struct feed_device_std_descriptor const * const
        p_device_std_descriptor)
{
    char
        b_result;

    p_device_std->p_client =
        p_client;

    p_device_std->p_termios =
        (struct termios *)(
            p_device_std->a_termios_storage);

    p_device_std->o_descriptor =
        *(
            p_device_std_descriptor);

    p_device_std->b_enabled =
        0;

    b_result =
        1;

    return
        b_result;

} /* feed_device_std_init() */

static
char
feed_device_std_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_device_std *
        p_device_std;

    struct feed_device_std_descriptor const *
        p_device_std_descriptor;

    p_device_std =
        (struct feed_device_std *)(
            p_object);

    p_device_std_descriptor =
        (struct feed_device_std_descriptor const *)(
            p_descriptor);

    b_result =
        feed_device_std_init(
            p_client,
            p_device_std,
            p_device_std_descriptor);

    return
        b_result;

} /* feed_device_std_init_cb() */

struct feed_device_std *
feed_device_std_create(
    struct feed_client * const
        p_client,
    struct feed_device_std_descriptor const * const
        p_device_std_descriptor)
{
    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_device_std)),
                &(
                    feed_device_std_init_cb),
                (void const *)(
                    p_device_std_descriptor)));

    return
        p_device_std;

} /* feed_device_std_create() */

static
void
feed_device_std_cleanup(
    struct feed_device_std * const
        p_device_std)
{
    (void)(
        p_device_std);

} /* feed_device_std_cleanup() */

static
void
feed_device_std_cleanup_cb(
    void * const
        p_object)
{
    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_object);

    feed_device_std_cleanup(
        p_device_std);

} /* feed_device_std_cleanup_cb() */

void
feed_device_std_destroy(
    struct feed_device_std * const
        p_device_std)
{
    struct feed_client *
        p_client;

    p_client =
        p_device_std->p_client;

    feed_object_destroy(
        p_client,
        (void *)(
            p_device_std),
        &(
            feed_device_std_cleanup_cb));

} /* feed_device_std_destroy() */

static
void
feed_device_std_set_raw_options(
    struct termios * const
        p_termios)
{
    p_termios->c_iflag &=
        (unsigned int)(~(unsigned int)(
            BRKINT
            | ICRNL
            | INPCK
            | ISTRIP
            | IXON));

    p_termios->c_oflag &=
        (unsigned int)(~(unsigned int)(
            OPOST));

    p_termios->c_cflag |=
        CS8;

    p_termios->c_lflag &=
        (unsigned int)(~(unsigned int)(
            ECHO
            | ICANON
            | IEXTEN
            | ISIG));

    p_termios->c_cc[VMIN] =
        1;

    p_termios->c_cc[VTIME] =
        0;

}

static
int
feed_device_std_enter(
    void * const
        p_context)
{
    char
        b_result;

    struct termios
        o_termios;

    int
        i_term_status;

    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_context);

    if (
        isatty(
            p_device_std->o_descriptor.i_tty_file_descriptor))
    {
        i_term_status =
            tcgetattr(
                p_device_std->o_descriptor.i_tty_file_descriptor,
                p_device_std->p_termios);

        if (
            0 <= i_term_status)
        {
            memcpy(
                &(
                    o_termios),
                p_device_std->p_termios,
                sizeof(
                    struct termios));

            feed_device_std_set_raw_options(
                &(
                    o_termios));

            i_term_status =
                tcsetattr(
                    p_device_std->o_descriptor.i_tty_file_descriptor,
                    TCSADRAIN,
                    &(
                        o_termios));

            if (
                0 <= i_term_status)
            {
                p_device_std->b_enabled =
                    1;

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
            1;
    }

    return
        b_result ? 0 : -1;

} /* feed_device_std_enter() */

static
int
feed_device_std_leave(
    void * const
        p_context)
{
    char
        b_result;

    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_context);

    if (
        p_device_std->b_enabled)
    {
        int
            i_term_status;

        i_term_status =
            tcsetattr(
                p_device_std->o_descriptor.i_tty_file_descriptor,
                TCSADRAIN,
                p_device_std->p_termios);

        if (
            0 <= i_term_status)
        {
            p_device_std->b_enabled =
                0;

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
            1;
    }

    return
        b_result ? 0 : -1;

} /* feed_device_std_leave() */

static
int
feed_device_std_query(
    void * const
        p_context,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows)
{
    char
        b_result;

    int
        i_result;

    struct winsize
        o_winsize;

    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_context);

    i_result =
        ioctl(
            p_device_std->o_descriptor.i_tty_file_descriptor,
            TIOCGWINSZ,
            &(
                o_winsize));

    if (
        0 <= i_result)
    {
        *(
            p_columns) =
            (unsigned short int)(
                o_winsize.ws_col);

        *(
            p_rows) =
            (unsigned short int)(
                o_winsize.ws_row);

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result ? 0 : -1;

} /* feed_device_std_query() */

static
signed long int
feed_device_std_read(
    void * const
        p_context,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    signed long int
        i_result;

    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_context);

    i_result =
        (signed long int)(
            read(
                p_device_std->o_descriptor.i_read_file_descriptor,
                p_buf,
                i_buf_len));

    return
        i_result;

} /* feed_device_std_read() */

static
signed long int
feed_device_std_write(
    void * const
        p_context,
    unsigned char const * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    signed long int
        i_result;

    struct feed_device_std *
        p_device_std;

    p_device_std =
        (struct feed_device_std *)(
            p_context);

    i_result =
        (signed long int)(
            write(
                p_device_std->o_descriptor.i_write_file_descriptor,
                p_buf,
                i_buf_len));

    return
        i_result;

} /* feed_device_std_write() */

static struct feed_device_intf const g_feed_device_std_intf =
{
    &(
        feed_device_std_enter),
    &(
        feed_device_std_leave),
    &(
        feed_device_std_query),
    &(
        feed_device_std_read),
    &(
        feed_device_std_write)

};

struct feed_device_intf const *
feed_device_std_intf(void)
{
    return
        &(
            g_feed_device_std_intf);

} /* feed_device_std_intf() */

#endif /* #if defined FEED_OS_LINUX */

/* end-of-file: feed_device_std.c */
