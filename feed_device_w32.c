/* See LICENSE for license details */

/*

Module: feed_device_w32.c

Description:

    Low-level layer for access to tty device.  At this level, the interface is
    minimal, so no knowledge of escape sequences and there's no caching of
    input or output.

*/

#include "feed_os.h"

#if defined FEED_OS_WINDOWS

#include "feed_cfg.h"

#include "feed_device_w32.h"

#include "feed_object.h"

#include "feed.h"

struct feed_device_w32
{
    struct feed_client *
        p_client;

    void *
        pv_padding[1u];

    /* -- */

    char
        b_enabled;

    unsigned char
        uc_padding[7u];

}; /* struct feed_device_w32 */

static
char
feed_device_w32_init(
    struct feed_client * const
        p_client,
    struct feed_device_w32 * const
        p_device_w32)
{
    char
        b_result;

    printf("feed_device_w32_init\n");

    p_device_w32->p_client =
        p_client;

    p_device_w32->b_enabled =
        0;

    b_result =
        1;

    return
        b_result;

} /* feed_device_w32_init() */

static
char
feed_device_w32_init_cb(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_device_w32 *
        p_device_w32;

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_object);

    (void)(
        p_descriptor);

    b_result =
        feed_device_w32_init(
            p_client,
            p_device_w32);

    return
        b_result;

} /* feed_device_w32_init_cb() */

struct feed_device_w32 *
feed_device_w32_create(
    struct feed_client * const
        p_client)
{
    struct feed_device_w32 *
        p_device_w32;

    printf("feed_device_w32_create\n");

    p_device_w32 =
        (struct feed_device_w32 *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_device_w32)),
                &(
                    feed_device_w32_init_cb),
                (void const *)(
                    0)));

    return
        p_device_w32;

} /* feed_device_w32_create() */

static
void
feed_device_w32_cleanup(
    struct feed_device_w32 * const
        p_device_w32)
{
    (void)(
        p_device_w32);

} /* feed_device_w32_cleanup() */

static
void
feed_device_w32_cleanup_cb(
    void * const
        p_object)
{
    struct feed_device_w32 *
        p_device_w32;

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_object);

    feed_device_w32_cleanup(
        p_device_w32);

} /* feed_device_w32_cleanup_cb() */

void
feed_device_w32_destroy(
    struct feed_device_w32 * const
        p_device_w32)
{
    struct feed_client *
        p_client;

    printf("feed_device_w32_destroy\n");

    p_client =
        p_device_w32->p_client;

    feed_object_destroy(
        p_client,
        (void *)(
            p_device_w32),
        &(
            feed_device_w32_cleanup_cb));

} /* feed_device_w32_destroy() */

static
int
feed_device_w32_enter(
    void * const
        p_context)
{
    char
        b_result;

    struct feed_device_w32 *
        p_device_w32;

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_context);

    printf("feed_device_w32_enter\n");

    if (
        !(
            p_device_w32->b_enabled))
    {
        setvbuf(stdin, NULL, _IONBF, 0);

        setvbuf(stdout, NULL, _IONBF, 0);

        setvbuf(stderr, NULL, _IONBF, 0);

        p_device_w32->b_enabled =
            1;

        b_result =
            1;
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result ? 0 : -1;

} /* feed_device_w32_enter() */

static
int
feed_device_w32_leave(
    void * const
        p_context)
{
    char
        b_result;

    struct feed_device_w32 *
        p_device_w32;

    printf("feed_device_w32_leave\n");

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_context);

    if (
        p_device_w32->b_enabled)
    {
        p_device_w32->b_enabled =
            0;

        b_result =
            1;
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result ? 0 : -1;

} /* feed_device_w32_leave() */

static
int
feed_device_w32_query(
    void * const
        p_context,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows)
{
    char
        b_result;

    struct feed_device_w32 *
        p_device_w32;

    printf("feed_device_w32_query\n");

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_context);

    (void)(
        p_device_w32);

    *(
        p_columns) =
        (unsigned short int)(
            80);

    *(
        p_rows) =
        (unsigned short int)(
            24);

    b_result =
        1;

    return
        b_result ? 0 : -1;

} /* feed_device_w32_query() */

static
signed long int
feed_device_w32_read(
    void * const
        p_context,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    signed long int
        i_result;

    struct feed_device_w32 *
        p_device_w32;

    printf("feed_device_w32_read\n");

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_context);

    (void)(
        p_device_w32);
    (void)(
        i_buf_len);

    p_buf[0] =
        (unsigned char)(
            fgetc(stdin));

    i_result =
        1;

    return
        i_result;

} /* feed_device_w32_read() */

static
signed long int
feed_device_w32_write(
    void * const
        p_context,
    unsigned char const * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    signed long int
        i_result;

    struct feed_device_w32 *
        p_device_w32;

    p_device_w32 =
        (struct feed_device_w32 *)(
            p_context);

    (void)(
        p_device_w32);

    printf("feed_device_w32_write...\n");

    fwrite(
        p_buf,
        i_buf_len,
        1,
        stdout);

    printf("...\n");

    i_result =
        (signed long int)(
            i_buf_len);

    return
        i_result;

} /* feed_device_w32_write() */

static struct feed_device_intf const g_feed_device_w32_intf =
{
    &(
        feed_device_w32_enter),
    &(
        feed_device_w32_leave),
    &(
        feed_device_w32_query),
    &(
        feed_device_w32_read),
    &(
        feed_device_w32_write)

};

struct feed_device_intf const *
feed_device_w32_intf(void)
{
    return
        &(
            g_feed_device_w32_intf);

} /* feed_device_w32_intf() */

#endif /* #if defined FEED_OS_WINDOWS */

/* end-of-file: feed_device_w32.c */
