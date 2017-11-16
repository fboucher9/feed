/* See LICENSE for license details */

/*

Module: feed_screen.h

Description:

    Manager for refresh of visible characters on terminal screen.

Comments:

    Handling of limited screen width and height.

    Avoid writing on last character of screen

    Handle grow of region, which may do a vertical scroll

    Backbuffer and front buffer

    Wrap of wide characters

    Do relative movements for cursor, so that region may grow.

    When cursor y > region height, then grow region height or clip or scroll.

    When cursor x > region width, then grow or clip or wrap or scroll.

*/

/* Header file dependency */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Reverse include guard */
#if defined(INC_FEED_SCREEN_H)
#error include feed_screen.h once
#endif /* #if defined(INC_FEED_SCREEN_H) */

#define INC_FEED_SCREEN_H

struct feed_client;

struct feed_screen;

struct feed_screen *
feed_screen_create(
    struct feed_client * const
        p_client);

void
feed_screen_destroy(
    struct feed_screen * const
        p_screen);

void
feed_screen_set_physical_size(
    struct feed_screen * const
        p_screen,
    unsigned int const
        i_physical_width,
    unsigned int const
        i_physical_height);

unsigned long int
feed_screen_get_cursor_pos(
    struct feed_screen * const
        p_screen);

void
feed_screen_set_cursor_pos(
    struct feed_screen * const
        p_screen,
    unsigned long int const
        i_cursor_address);

void
feed_screen_newline_raw(
    struct feed_screen * const
        p_screen);

void
feed_screen_newline(
    struct feed_screen * const
        p_screen);

void
feed_screen_write(
    struct feed_screen * const
        p_screen,
    unsigned char const * const
        p_data,
    unsigned int const
        i_count);

void
feed_screen_clear_region(
    struct feed_screen * const
        p_screen);

/* end-of-file: feed_screen.h */
