/* See LICENSE for license details */

/*

Module: feed_theme.c

Description:

    Management of color theme.

*/

#include "feed_os.h"

#include "feed.h"

#include "feed_theme.h"

void
feed_theme_init(
    struct feed_color_list * const
        p_color_list)
{
    memset(
        p_color_list,
        0u,
        sizeof(
            *(
                p_color_list)));

} /* feed_theme_init() */

void
feed_theme_cleanup(
    struct feed_color_list * const
        p_color_list)
{
    (void)(
        p_color_list);

} /* feed_theme_cleanup() */

char
feed_theme_reset(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax)
{
    char
        b_result;

    unsigned int
        i_index;

    i_index =
        (unsigned int)(
            e_syntax);

    if (
        i_index
        < (unsigned int)(
            sizeof(
                p_color_list->a_nodes)
            / sizeof(
                p_color_list->a_nodes[0u])))
    {
        struct feed_color_node *
            p_color_node;

        p_color_node =
            p_color_list->a_nodes + i_index;

        p_color_node->i_foreground =
            feed_color_default;

        p_color_node->i_background =
            feed_color_default;

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

char
feed_theme_set(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax,
    enum feed_color const
        i_foreground,
    enum feed_color const
        i_background)
{
    char
        b_result;

    unsigned int
        i_index;

    i_index =
        (unsigned int)(
            e_syntax);

    if (
        i_index
        < (unsigned int)(
            sizeof(
                p_color_list->a_nodes)
            / sizeof(
                p_color_list->a_nodes[0u])))
    {
        struct feed_color_node *
            p_color_node;

        p_color_node =
            p_color_list->a_nodes + i_index;

        p_color_node->i_foreground =
            i_foreground;

        p_color_node->i_background =
            i_background;

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

} /* feed_theme_set() */

char
feed_theme_get(
    struct feed_color_list * const
        p_color_list,
    enum feed_syntax const
        e_syntax,
    enum feed_color * const
        p_foreground,
    enum feed_color * const
        p_background)
{
    char
        b_result;

    unsigned int
        i_index;

    i_index =
        (unsigned int)(
            e_syntax);

    if (
        i_index
        < (unsigned int)(
            sizeof(
                p_color_list->a_nodes)
            / sizeof(
                p_color_list->a_nodes[0u])))
    {
        struct feed_color_node *
            p_color_node;

        p_color_node =
            p_color_list->a_nodes + i_index;

        *(
            p_foreground) =
            p_color_node->i_foreground;

        *(
            p_background) =
            p_color_node->i_background;

        b_result =
            1;
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

} /* feed_theme_get() */

/* end-of-file: feed_theme.c */
