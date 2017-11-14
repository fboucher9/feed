/* See LICENSE for license details */

struct feed_screen_info
{
    unsigned long int
        i_screen_size;

    unsigned long int
        ul_padding[1u];

    unsigned short int
        i_screen_width;

    unsigned short int
        i_screen_height;

    unsigned short int
        us_padding[2u];

};

void
feed_screen_info_init(
    struct feed_screen_info * const
        p_screen_info);

void
feed_screen_info_cleanup(
    struct feed_screen_info * const
        p_screen_info);

void
feed_screen_info_update(
    struct feed_screen_info * const
        p_screen_info,
    unsigned short int
        i_screen_width,
    unsigned short int
        i_screen_height);

/* end-of-file: feed_screen_info.h */
