/* See LICENCE for license details */

/*

Module: feed_dict.c

Description:

    Dictionnary of keywords for syntax.

*/

#include "feed_os.h"

#include "feed_list.h"

#include "feed_buf.h"

#include "feed_dict.h"

#include "feed_client.h"

#include "feed_heap.h"

#include "feed_object.h"

struct feed_dict_node;

struct feed_dict_node
{
    struct feed_list
        o_list;

    struct feed_client *
        p_client;

    struct feed_dict *
        p_dict;

    struct feed_buf
        o_buf;

}; /* struct feed_dict_node */

struct feed_dict
{
    struct feed_client *
        p_client;

    struct feed_dict_node * *
        p_lookup;

    struct feed_list
        o_words;

    unsigned long int
        i_word_count;

    unsigned long int
        ul_padding[1u];

    char
        b_lookup_valid;

    unsigned char
        uc_padding[7u];

}; /* struct feed_dict */

static
void
feed_dict_node_init(
    struct feed_dict_node * const
        p_dict_node,
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf)
{
    feed_list_init(
        &(
            p_dict_node->o_list));

    p_dict_node->p_client =
        p_dict->p_client;

    p_dict_node->p_dict =
        p_dict;

    p_dict_node->o_buf.o_min.p =
        (unsigned char *)(
            p_dict_node + 1);

    p_dict_node->o_buf.o_max.p =
        (unsigned char *)(
            p_dict_node->o_buf.o_min.p
            + (p_buf->o_max.p
                - p_buf->o_min.p));

    memcpy(
        p_dict_node->o_buf.o_min.p,
        p_buf->o_min.p,
        (unsigned long int)(
            p_buf->o_max.pc
            - p_buf->o_min.pc));
}

static
void
feed_dict_node_cleanup(
    struct feed_dict_node * const
        p_dict_node)
{
    feed_list_join(
        &(
            p_dict_node->o_list),
        &(
            p_dict_node->o_list));

    p_dict_node->p_client =
        (struct feed_client *)(
            0);

    p_dict_node->p_dict =
        (struct feed_dict *)(
            0);

    p_dict_node->o_buf.o_min.p =
        (unsigned char *)(
            0);

    p_dict_node->o_buf.o_max.p =
        (unsigned char *)(
            0);

}

static
struct feed_dict_node *
feed_dict_node_create(
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf)
{
    struct feed_dict_node *
        p_dict_node;

    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    void *
        p_placement;

    p_client =
        p_dict->p_client;

    p_heap =
        p_client->p_heap;

    p_placement =
        feed_heap_alloc(
            p_heap,
            (unsigned long int)(
                sizeof(
                    struct feed_dict_node)
                + (unsigned long int)(
                    p_buf->o_max.pc
                    - p_buf->o_min.pc)));

    if (
        p_placement)
    {
        p_dict_node =
            (struct feed_dict_node *)(
                p_placement);

        feed_dict_node_init(
            p_dict_node,
            p_dict,
            p_buf);
    }
    else
    {
        p_dict_node =
            (struct feed_dict_node *)(
                0);
    }

    return
        p_dict_node;

}

static
void
feed_dict_node_destroy(
    struct feed_dict_node * const
        p_dict_node)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    p_client =
        p_dict_node->p_client;

    p_heap =
        p_client->p_heap;

    feed_dict_node_cleanup(
        p_dict_node);

    feed_heap_free(
        p_heap,
        (void *)(
            p_dict_node));

}

static
char
feed_dict_init(
    void * const
        p_object,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    char
        b_result;

    struct feed_dict *
        p_dict;

    (void)(
        p_descriptor);

    p_dict =
        (struct feed_dict *)(
            p_object);

    p_dict->p_client =
        p_client;

    p_dict->p_lookup =
        (struct feed_dict_node * *)(
            0);

    feed_list_init(
        &(
            p_dict->o_words));

    p_dict->i_word_count =
        0ul;

    p_dict->b_lookup_valid =
        0;

    b_result =
        1;

    return
        b_result;

}

static
void
feed_dict_cleanup(
    void * const
        p_object)
{
    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    struct feed_dict *
        p_dict;

    p_dict =
        (struct feed_dict *)(
            p_object);

    p_client =
        p_dict->p_client;

    p_heap =
        p_client->p_heap;

    if (
        p_dict->b_lookup_valid)
    {
        feed_heap_free(
            p_heap,
            (void *)(
                p_dict->p_lookup));

        p_dict->p_lookup =
            (struct feed_dict_node * *)(
                0);

        p_dict->b_lookup_valid =
            0;
    }

    while (
        p_dict->o_words.p_next
        != &(p_dict->o_words))
    {
        struct feed_dict_node *
            p_dict_node;

        p_dict_node =
            (struct feed_dict_node *)(
                p_dict->o_words.p_next);

        feed_dict_node_destroy(
            p_dict_node);

        if (p_dict->i_word_count)
        {
            p_dict->i_word_count --;
        }
    }

    p_dict->i_word_count =
        0ul;

}

struct feed_dict *
feed_dict_create(
    struct feed_client * const
        p_client)
{
    return
        (struct feed_dict *)(
            feed_object_create(
                p_client,
                (unsigned long int)(
                    sizeof(
                        struct feed_dict)),
                &(
                    feed_dict_init),
                (void const *)(
                    0)));

}

void
feed_dict_destroy(
    struct feed_dict * const
        p_dict)
{
    struct feed_client *
        p_client;

    p_client =
        p_dict->p_client;

    feed_object_destroy(
        p_client,
        (void *)(
            p_dict),
        &(
            feed_dict_cleanup));

}

static
void
feed_dict_invalidate_lookup(
    struct feed_dict * const
        p_dict)
{
    if (
        p_dict->b_lookup_valid)
    {
        struct feed_client *
            p_client;

        struct feed_heap *
            p_heap;

        p_client =
            p_dict->p_client;

        p_heap =
            p_client->p_heap;

        feed_heap_free(
            p_heap,
            (void *)(
                p_dict->p_lookup));

        p_dict->p_lookup =
            (struct feed_dict_node * *)(
                0);

        p_dict->b_lookup_valid =
            0;
    }
}

static
char
feed_dict_validate_lookup(
    struct feed_dict * const
        p_dict)
{
    char
        b_result;

    if (
        !(p_dict->b_lookup_valid))
    {
        struct feed_client *
            p_client;

        struct feed_heap *
            p_heap;

        p_client =
            p_dict->p_client;

        p_heap =
            p_client->p_heap;

        p_dict->p_lookup =
            (struct feed_dict_node * *)(
                feed_heap_alloc(
                    p_heap,
                    (unsigned long int)(
                        p_dict->i_word_count
                        * sizeof(
                            struct feed_dict_node *))));

        if (
            p_dict->p_lookup)
        {
            struct feed_list *
                p_iterator;

            unsigned long int
                i_index;

            p_iterator =
                p_dict->o_words.p_next;

            i_index =
                0ul;

            while (
                p_iterator
                != &(
                    p_dict->o_words))
            {
                struct feed_dict_node *
                    p_dict_node;

                p_dict_node =
                    (struct feed_dict_node *)(
                        p_iterator);

                p_dict->p_lookup[i_index] =
                    p_dict_node;

                i_index ++;

                p_iterator =
                    p_iterator->p_next;
            }

            p_dict->b_lookup_valid =
                1;

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result;

}

#if 0
static
void
feed_dict_report(
    struct feed_dict * const
        p_dict)
{
    unsigned long int
        i_index;

    struct feed_list *
        p_iterator;

    printf("feed_dict report {...\n");

    p_iterator =
        p_dict->o_words.p_next;

    i_index =
        0ul;

    while (
        p_iterator
        != (
            &(
                p_dict->o_words)))
    {
        struct feed_dict_node *
            p_dict_node;

        p_dict_node =
            (struct feed_dict_node *)(
                p_iterator);

        printf(
            "[%lu] buf = {%.*s}\n",
            i_index,
            (int)(
                p_dict_node->o_buf.o_max.pc
                - p_dict_node->o_buf.o_min.pc),
            (char const *)(
                p_dict_node->o_buf.o_min.pc));

        i_index ++;

        p_iterator =
            p_iterator->p_next;
    }

    printf("feed_dict report ...}\n");
}
#endif

char
feed_dict_add(
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf)
{
    char
        b_result;

    struct feed_list *
        p_iterator;

    char
        b_found;

    char
        b_exact;

    p_iterator =
        p_dict->o_words.p_next;

    b_found =
        0;

    b_exact =
        0;

    while (
        (!b_exact)
        && (!b_found)
        && (p_iterator
            != &(p_dict->o_words)))
    {
        struct feed_dict_node *
            p_iterator_node;

        int
            i_compare;

        p_iterator_node =
            (struct feed_dict_node *)(
                p_iterator);

        /* Compare nodes */
        i_compare =
            feed_buf_compare(
                p_buf,
                &(
                    p_iterator_node->o_buf));

        if (
            0 == i_compare)
        {
            b_exact =
                1;
        }
        else if (
            0 > i_compare)
        {
            b_found =
                1;
        }
        else
        {
            p_iterator =
                p_iterator->p_next;
        }
    }

    if (!b_exact)
    {
        struct feed_dict_node *
            p_dict_node;

        p_dict_node =
            feed_dict_node_create(
                p_dict,
                p_buf);

        if (
            p_dict_node)
        {
            /* Insert into list */
            feed_list_join(
                &(
                    p_dict_node->o_list),
                p_iterator);

            p_dict->i_word_count ++;

            feed_dict_invalidate_lookup(
                p_dict);

            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            1;
    }

    return
        b_result;

}

char
feed_dict_lookup(
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf)
{
    char
        b_result;

    /* refresh the lookup */
    if (
        feed_dict_validate_lookup(
            p_dict))
    {
        /* do a quick search */
        /* define a range and a pivot */
        unsigned long int
            i_range_min;

        unsigned long int
            i_range_max;

        unsigned long int
            i_pivot;

        int
            i_compare;

        char
            b_continue;

        char
            b_found;

        i_range_min =
            0ul;

        i_range_max =
            p_dict->i_word_count;

        b_found =
            0;

        b_continue =
            1;

        while (
            b_continue && (i_range_min < i_range_max))
        {
            i_pivot =
                ((i_range_min + i_range_max) / 2ul);

            if (
                i_pivot < p_dict->i_word_count)
            {
                i_compare =
                    feed_buf_compare(
                        p_buf,
                        &(
                            p_dict->p_lookup[i_pivot]->o_buf));

                if (
                    0 == i_compare)
                {
                    b_found =
                        1;

                    b_continue =
                        0;
                }
                else if (
                    0 > i_compare)
                {
                    i_range_max =
                        i_pivot - 1ul;
                }
                else
                {
                    i_range_min =
                        i_pivot + 1ul;
                }
            }
            else
            {
                b_continue =
                    0;
            }
        }

        if (
            b_found)
        {
            b_result =
                1;
        }
        else
        {
            b_result =
                0;
        }
    }
    else
    {
        b_result =
            0;
    }

    return
        b_result;

}

