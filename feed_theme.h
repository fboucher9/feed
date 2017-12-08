/* See LICENSE for license details */

/*

Module: feed_theme.h

Description:

    Management of color theme.

*/

/* Reverse include guard */
#if defined(INC_FEED_THEME_H)
#error include feed_theme.h once
#endif /* #if defined(INC_FEED_THEME_H) */

#define INC_FEED_THEME_H

struct feed_color_node
{
    enum feed_color
        i_foreground;

    enum feed_color
        i_background;

};

struct feed_color_list
{
    struct feed_color_node
        a_nodes[16u];

};

void
feed_theme_init(
    struct feed_color_list * const
        p_color_list);

void
feed_theme_cleanup(
    struct feed_color_list * const
        p_color_list);

char
feed_theme_reset(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax);

char
feed_theme_set(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax,
    enum feed_color const
        i_foreground,
    enum feed_color const
        i_background);

char
feed_theme_get(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax,
    enum feed_color * const
        p_foreground,
    enum feed_color * const
        p_background);

/* end-of-file: feed_theme.h */
