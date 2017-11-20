/* See LICENSE for license details */

/*

Module: feed_suggest.h

Description:

*/

/* Header file dependency */
#if !defined(INC_FEED_LIST_H)
#error include feed_list.h before
#endif /* #if !defined(INC_FEED_LIST_H) */

/* Reverse include guard */
#if defined(INC_FEED_SUGGEST_H)
#error include feed_suggest.h once
#endif /* #if defined(INC_FEED_SUGGEST_H) */

#define INC_FEED_SUGGEST_H

struct feed_client;

struct feed_suggest_descriptor
{
    unsigned char const *
        p_buffer;

    void *
        pv_padding[1u];

    unsigned long int
        i_length;

    unsigned long int
        i_cursor_offset;

    unsigned long int
        i_word_offset;

    unsigned long int
        i_word_length;

};

struct feed_suggest_node
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_suggest_descriptor
        o_descriptor;

}; /* struct feed_suggest_node */

struct feed_suggest_node *
feed_suggest_node_create(
    struct feed_client * const
        p_client,
    struct feed_suggest_descriptor const * const
        p_suggest_descriptor);

void
feed_suggest_node_destroy(
    struct feed_suggest_node * const
        p_suggest_node);

struct feed_suggest_list
{
    struct feed_list
        o_nodes;

    struct feed_client *
        p_client;

    unsigned long int
        i_count;

    unsigned long int
        ul_padding[1u];

}; /* struct feed_suggest_list */

char
feed_suggest_list_init(
    struct feed_client * const
        p_client,
    struct feed_suggest_list * const
        p_suggest_list);

void
feed_suggest_list_cleanup(
    struct feed_suggest_list * const
        p_suggest_list);

void
feed_suggest_list_clear(
    struct feed_suggest_list * const
        p_suggest_list);

char
feed_suggest_list_append(
    struct feed_suggest_list * const
        p_suggest_list,
    struct feed_suggest_node * const
        p_suggest_node);

struct feed_suggest_node *
feed_suggest_list_first(
    struct feed_suggest_list * const
        p_suggest_list);

struct feed_suggest_node *
feed_suggest_list_next(
    struct feed_suggest_list * const
        p_suggest_list,
    struct feed_suggest_node * const
        p_suggest_node);

/* end-of-file: feed_suggest.h */
