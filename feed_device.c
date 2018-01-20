/* See LICENSE for license details */

/*

Module: feed_device.c

Description:

    Dispatch of feed_device_intf to selected implementation.  Default
    implementation is feed_device_std or else user may decide to hook
    the implementation via application callbacks.  The feed_device_null
    module is used for when feed_device is destroyed.

*/

#include "feed_os.h"

#include "feed_cfg.h"

#include "feed_device.h"

#include "feed.h"

/*

Function: feed_device_null_enter

Description:

    Stub for feed_device_enter.

*/
static
int
feed_device_null_enter(
    void * const
        p_context)
{
    (void)(
        p_context);

    return
        -1;

} /* feed_device_null_enter() */

/*

Function: feed_device_null_leave

Description:

    Stub for feed_device_leave.

*/
static
int
feed_device_null_leave(
    void * const
        p_context)
{
    (void)(
        p_context);

    return
        -1;

} /* feed_device_null_leave() */

/*

Function: feed_device_null_query

Description:

    Stub for feed_device_query.

*/
static
int
feed_device_null_query(
    void * const
        p_context,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows)
{
    (void)(
        p_context);
    (void)(
        p_columns);
    (void)(
        p_rows);

    return
        -1;

} /* feed_device_null_query() */

/*

Function: feed_device_null_read

Description:

    Stub for feed_device_read.

*/
static
signed long int
feed_device_null_read(
    void * const
        p_context,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    (void)(
        p_context);
    (void)(
        p_buf);
    (void)(
        i_buf_len);

    return
        -1;

} /* feed_device_null_read() */

/*

Function: feed_device_null_write

Description:

    Stub for feed_device_write.

*/
static
signed long int
feed_device_null_write(
    void * const
        p_context,
    unsigned char const * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    (void)(
        p_context);
    (void)(
        p_buf);
    (void)(
        i_buf_len);

    return
        -1;

} /* feed_device_null_write() */

/* Interface of feed_device_null module */
static
struct feed_device_intf const
g_feed_device_null_intf =
{
    &(
        feed_device_null_enter),
    &(
        feed_device_null_leave),
    &(
        feed_device_null_query),
    &(
        feed_device_null_read),
    &(
        feed_device_null_write)
};

/* Context of feed_device_null module */
static
unsigned char
g_feed_device_null_context[1u] =
{
    (unsigned char)(
        0u)
};

/*

Function: feed_device_init

Description:

    Initialize feed_device module to reference the selected feed_device_intf
    callbacks and callback context.

*/
char
feed_device_init(
    struct feed_device * const
        p_device,
    struct feed_device_intf const * const
        p_device_intf,
    void * const
        p_context)
{
    char
        b_result;

    p_device->p_device_intf =
        p_device_intf;

    p_device->p_context =
        p_context;

    b_result =
        1;

    return
        b_result;

} /* feed_device_init() */

/*

Function: feed_device_cleanup

Description:

    Undo initialization of feed_device_init() function.

*/
void
feed_device_cleanup(
    struct feed_device * const
        p_device)
{
    p_device->p_device_intf =
        &(
            g_feed_device_null_intf);

    p_device->p_context =
        (void *)(
            g_feed_device_null_context);

} /* feed_device_cleanup() */

/*

Function: feed_device_enter

Description:

    Dispatch to selected implementation.

*/
int
feed_device_enter(
    struct feed_device * const
        p_device)
{
    return
        (*(p_device->p_device_intf->p_enter))(
            p_device->p_context);

} /* feed_device_enter() */

/*

Function: feed_device_leave

Description:

    Dispatch to selected implementation.

*/
int
feed_device_leave(
    struct feed_device * const
        p_device)
{
    return
        (*(p_device->p_device_intf->p_leave))(
            p_device->p_context);

} /* feed_device_leave() */

/*

Function: feed_device_query

Description:

    Dispatch to selected implementation.

*/
int
feed_device_query(
    struct feed_device * const
        p_device,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows)
{
    return
        (*(p_device->p_device_intf->p_query))(
            p_device->p_context,
            p_columns,
            p_rows);

} /* feed_device_query() */

/*

Function: feed_device_read

Description:

    Dispatch to selected implementation.

*/
signed long int
feed_device_read(
    struct feed_device * const
        p_device,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    return
        (*(p_device->p_device_intf->p_read))(
            p_device->p_context,
            p_buf,
            i_buf_len);

} /* feed_device_read() */

/*

Function: feed_device_write

Description:

    Dispatch to selected implementation.

*/
signed long int
feed_device_write(
    struct feed_device * const
        p_device,
    unsigned char const * const
        p_buf,
    unsigned long int const
        i_buf_len)
{
    return
        (*(p_device->p_device_intf->p_write))(
            p_device->p_context,
            p_buf,
            i_buf_len);

} /* feed_device_write() */

/* end-of-file: feed_device.c */
