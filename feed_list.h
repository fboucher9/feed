/* See LICENSE for license details. */

/*

Module: feed_list.h

Description:

    Generic linked list to be used for all lists in feed project.

Comments:

    The list is a circular doubly linked list.  A fake element is used
    to point to first and last elements.  The same structure is used for
    each element including the fake element.  However, the fake element
    does not have the same derived type as the other elements.

    The caller is reponsible of casting the element to the derived type.
    Before casting, it is important to make sure that the element is
    not the fake element.  If the list is not the first member of the
    derived type, then it may be useful to use the offsetof() macro to
    get a pointer to the start of the derived type.

    The circular feature ensures that each element is part of a list.
    Operations may be done on a single element or on groups of elements.
    The same method is used to insert elements or to remove elements.
    See the examples section for more details.

Examples:

    -   create an empty list

            feed_list_init(&o_list);

    -   insert a first list before a second

            feed_list_join(&o_first, &o_second);

    -   insert a first list after a second

            feed_list_join(&o_second, &o_first);

    -   remove a single element from a list

            feed_list_join(&o_element, &o_element);

    -   remove a group of elements from a list

            feed_list_join(&o_last, &o_first);

Notes:

    To insert all elements of a list into another, you first need to
    detach the elements from the fake or else the final list may end up
    with two fake elements.

        p_first = o_fake.p_next;
        feed_list_join(&o_fake, &o_fake);
        feed_list_join(p_first, &o_other);

*/

/* Header file dependencies */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Reverse include guard */
#if defined(INC_FEED_LIST_H)
#error include feed_list.h once
#endif /* #if defined(INC_FEED_LIST_H) */

#define INC_FEED_LIST_H

/*

Structure: feed_list

Description:

*/
struct feed_list
{
    struct feed_list *
        p_next;

    struct feed_list *
        p_prev;

}; /* struct feed_list */

/* Public functions ... */

void
feed_list_init(
    struct feed_list * const
        p_node);

void
feed_list_join(
    struct feed_list * const
        p_before,
    struct feed_list * const
        p_after);

/* end-of-file: feed_list.h */
