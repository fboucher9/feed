/* See LICENSE for license details */

/*

Module: feed_device_std.h

Description:

    Low-level layer for access to tty device.  At this level, the interface is
    minimal, so no knowledge of escape sequences and there's no caching of
    input or output.

*/

/* Reverse include guard */
#if defined(INC_FEED_DEVICE_STD_H)
#error include feed_device_std.h once
#endif /* #if defined(INC_FEED_DEVICE_STD_H) */

#define INC_FEED_DEVICE_STD_H

/* Predefine handles */
struct feed_client;

struct feed_device_std;

struct feed_device_intf;

/*

Structure: feed_device_std_descriptor

Description:

    Initialization options for feed_device_std module.  Select some file
    descriptors that will be used by module.

*/
struct feed_device_std_descriptor
{
    /* file descriptor of tty, for setting raw mode and for query of size */
    int
        i_tty_file_descriptor;

    /* file descriptor for read operations */
    int
        i_read_file_descriptor;

    /* file descriptor for write operations */
    int
        i_write_file_descriptor;

    unsigned int
        ui_padding[1u];

}; /* struct feed_device_std_descriptor */

/* Create an instance of this module */
struct feed_device_std *
feed_device_std_create(
    struct feed_client * const
        p_client,
    struct feed_device_std_descriptor const * const
        p_device_std_descriptor);

/* Destroy an instance of this module */
void
feed_device_std_destroy(
    struct feed_device_std * const
        p_device_std);

/* Query feed_device_intf structure for this module */
struct feed_device_intf const *
feed_device_std_intf(void);

/* end-of-file: feed_device_std.h */
