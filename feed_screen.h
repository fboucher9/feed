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

struct feed_input;

struct feed_screen
{
    struct feed_client *
        p_client;

    struct feed_input *
        p_input;

    unsigned int
        i_screen_width;

    unsigned int
        i_screen_height;

    unsigned int
        i_region_height;

    unsigned int
        i_cursor_x;

    unsigned int
        i_cursor_y;

    unsigned int
        ui_padding[3u];

}; /* struct feed_screen */

struct feed_screen *
feed_screen_create(
    struct feed_client * const
        p_client,
    unsigned int const
        i_screen_width,
    unsigned int const
        i_screen_height);

void
feed_screen_destroy(
    struct feed_screen * const
        p_screen);

void
feed_screen_cursor(
    struct feed_screen * const
        p_screen,
    unsigned int const
        i_cursor_x,
    unsigned int const
        i_cursor_y);

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
feed_screen_clear_line(
    struct feed_screen * const
        p_screen);

/* end-of-file: feed_screen.h */
