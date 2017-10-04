/* See LICENCE for license details */

/*

Module: feed_esc.h

Description:

    Encode of terminal escape codes.

*/

struct feed_buf;

char
feed_esc_write_n1(
    struct feed_buf * const
        p_buf,
    int const
        i_count,
    unsigned char const
        c_direction);

char
feed_esc_write_cuu(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

char
feed_esc_write_cud(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

char
feed_esc_write_cuf(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

char
feed_esc_write_cub(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

#define FEED_CSI_ED_BOTTOM 0
#define FEED_CSI_ED_TOP 1
#define FEED_CSI_ED_SCREEN 2
#define FEED_CSI_ED_HISTORY 3

char
feed_esc_write_ed(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

#define FEED_CSI_EL_END 0
#define FEED_CSI_EL_BEGIN 1
#define FEED_CSI_EL_LINE 2

char
feed_esc_write_el(
    struct feed_buf * const
        p_buf,
    int const
        i_count);

char
feed_esc_write_cup(
    struct feed_buf * const
        p_buf,
    int const
        i_row,
    int const
        i_column);

char
feed_esc_write_sgr(
    struct feed_buf * const
        p_buf,
    unsigned char const * const
        p_attr,
    unsigned int const
        i_count);

/* end-of-file: feed_esc.h */
