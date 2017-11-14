/* See LICENSE for license details */

/*

Module: feed_screen_info.c

Description:

*/

#include "feed_os.h"

#include "feed_screen_info.h"

void
feed_screen_info_init(
    struct feed_screen_info * const
        p_screen_info)
{
    p_screen_info->i_screen_size =
        0ul;

    p_screen_info->i_screen_width =
        0u;

    p_screen_info->i_screen_height =
        0u;

}

void
feed_screen_info_cleanup(
    struct feed_screen_info * const
        p_screen_info)
{
    p_screen_info->i_screen_size =
        0ul;

    p_screen_info->i_screen_width =
        0u;

    p_screen_info->i_screen_height =
        0u;

}

void
feed_screen_info_update(
    struct feed_screen_info * const
        p_screen_info,
    unsigned short int
        i_screen_width,
    unsigned short int
        i_screen_height)
{
    p_screen_info->i_screen_size =
        (
            (unsigned long int)(
                i_screen_width)
            * (unsigned long int)(
                i_screen_height));

    p_screen_info->i_screen_width =
        i_screen_width;

    p_screen_info->i_screen_height =
        i_screen_height;

}

/* end-of-file: feed_screen_info.c */
