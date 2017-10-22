/* See LICENSE for license details. */

/*

Module: feed_list.c

Description:

    Generic linked list to be used for all lists in feed project.

Comments:

    See header file for more details.

*/

#include "feed_os.h"

#include "feed_list.h"

/*

Function: feed_list_init()

Description:

    Initialize an element so that it's a list with a single element, itself.

Parameters:

    p_node
        Pointer to element to initialize

Returns: None.

Comments: None.

*/
void
feed_list_init(
    struct feed_list * const
        p_node)
{
    p_node->p_next =
        p_node;

    p_node->p_prev =
        p_node;

} /* feed_list_init() */

/*

Function: feed_list_join()

Description:

    Join two elements together.

Parameters:

    p_before
        Pointer to first element to join.  This element will end up before
        the p_after element.

    p_after
        Pointer to second element to join.  This element will end up after
        the p_before element.

Comments:

    If the elements are in different lists, the two lists will become one.

    If the elements are in the same list, then there will be two lists.

*/
void
feed_list_join(
    struct feed_list * const
        p_before,
    struct feed_list * const
        p_after)
{
    /* AB(C) - (D)EF > ABCDEF */
    p_before->p_next->p_prev =
        p_after->p_prev;

    p_after->p_prev->p_next =
        p_before->p_next;

    p_before->p_next =
        p_after;

    p_after->p_prev =
        p_before;

} /* feed_list_join() */

/* end-of-file: feed_list.c */
