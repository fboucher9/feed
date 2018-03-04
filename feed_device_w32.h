/* See LICENSE for license details */

/*

Module: feed_device_w32.h

Description:

    Low-level layer for access to tty device.  At this level, the interface is
    minimal, so no knowledge of escape sequences and there's no caching of
    input or output.

*/

/* Reverse include guard */
#if defined(INC_FEED_DEVICE_W32_H)
#error include feed_device_w32.h once
#endif /* #if defined(INC_FEED_DEVICE_W32_H) */

#define INC_FEED_DEVICE_W32_H

/* Predefine handles */
struct feed_client;

struct feed_device_w32;

struct feed_device_intf;

/* Create an instance of this module */
struct feed_device_w32 *
feed_device_w32_create(
    struct feed_client * const
        p_client);

/* Destroy an instance of this module */
void
feed_device_w32_destroy(
    struct feed_device_w32 * const
        p_device_w32);

/* Query feed_device_intf structure for this module */
struct feed_device_intf const *
feed_device_w32_intf(void);

/* end-of-file: feed_device_w32.h */
