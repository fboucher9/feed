/* See LICENSE for license details */

/*

Module: feed_device.h

Description:

    Low-level layer for access to tty device.  At this level, the interface is
    minimal, so no knowledge of escape sequences and there's no caching of
    input or output.

*/

/* Reverse include guard */
#if defined(INC_FEED_DEVICE_H)
#error include feed_device.h once
#endif /* #if defined(INC_FEED_DEVICE_H) */

#define INC_FEED_DEVICE_H

/* Predefine handles */
struct feed_device_intf;

/*

Structure: feed_device

Description:

    Public members for feed_device module.

*/
struct feed_device
{
    struct feed_device_intf const *
        p_device_intf;

    void *
        p_context;

}; /* struct feed_device */

char
feed_device_init(
    struct feed_device * const
        p_device,
    struct feed_device_intf const * const
        p_device_intf,
    void * const
        p_context);

void
feed_device_cleanup(
    struct feed_device * const
        p_device);

int
feed_device_enter(
    struct feed_device * const
        p_device);

int
feed_device_leave(
    struct feed_device * const
        p_device);

int
feed_device_query(
    struct feed_device * const
        p_device,
    unsigned short int * const
        p_columns,
    unsigned short int * const
        p_rows);

signed long int
feed_device_read(
    struct feed_device * const
        p_device,
    unsigned char * const
        p_buf,
    unsigned long int const
        i_buf_len);

signed long int
feed_device_write(
    struct feed_device * const
        p_device,
    unsigned char const * const
        p_buf,
    unsigned long int const
        i_buf_len);

/* end-of-file: feed_device.h */
