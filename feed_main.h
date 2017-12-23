/* See LICENSE for license details */

/* Include guard */
#if defined(INC_FEED_MAIN_H)
#error include feed_main.h once
#endif /* #if defined(INC_FEED_MAIN_H) */

#define INC_FEED_MAIN_H

struct feed_buf;

struct feed_options
{
    struct feed_buf const *
        p_min;

    struct feed_buf const *
        p_max;

}; /* struct feed_options */

int
feed_main(
    struct feed_options const * const
        p_options);

