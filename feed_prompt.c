/* See LICENSE for license details */

/*

Module: feed_prompt.c

Description:

*/

#include "feed_os.h"

#include "feed_prompt.h"

#include "feed_client.h"

#include "feed_heap.h"

static
void
feed_prompt_init(
    struct feed_prompt * const
        p_prompt,
    struct feed_client * const
        p_client)
{
    p_prompt->p_client =
        p_client;

    p_prompt->a_prompt[0u] =
        NULL;

    p_prompt->a_prompt[1u] =
        NULL;
}

static
void
feed_prompt_cleanup(
    struct feed_prompt * const
        p_prompt)
{
    p_prompt->p_client =
        NULL;

}

struct feed_prompt *
feed_prompt_create(
    struct feed_client * const
        p_client)
{
    struct feed_prompt *
        p_prompt;

    if (
        p_client)
    {
        struct feed_heap *
            p_heap;

        p_heap =
            feed_client_get_heap(
                p_client);

        if (
            p_heap)
        {
            p_prompt =
                (struct feed_prompt *)(
                    feed_heap_alloc(
                        p_heap,
                        (unsigned int)(
                            sizeof(
                                struct feed_prompt))));

            if (
                p_prompt)
            {
                feed_prompt_init(
                    p_prompt,
                    p_client);
            }
        }
        else
        {
            p_prompt =
                NULL;
        }
    }
    else
    {
        p_prompt =
            NULL;
    }

    return
        p_prompt;

}

void
feed_prompt_destroy(
    struct feed_prompt * const
        p_prompt)
{
    if (
        p_prompt)
    {
        struct feed_client *
            p_client;

        p_client =
            p_prompt->p_client;

        if (p_client)
        {
            struct feed_heap *
                p_heap;

            p_heap =
                feed_client_get_heap(
                    p_client);

            if (
                p_heap)
            {
                feed_prompt_cleanup(
                    p_prompt);

                feed_heap_free(
                    p_heap,
                    (void *)(
                        p_prompt));
            }
        }
    }
}

static
char
feed_prompt_set(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length,
    unsigned int const
        i_index)
{
    char
        b_result;

    if (
        p_prompt)
    {
        if (
            i_index < 2u)
        {
            /* Delete the previous line */

            if (
                i_data_length)
            {
                if (
                    p_data)
                {
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

char
feed_prompt_set1(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    return
        feed_prompt_set(
            p_prompt,
            p_data,
            i_data_length,
            0u);
}

char
feed_prompt_set2(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    return
        feed_prompt_set(
            p_prompt,
            p_data,
            i_data_length,
            1u);
}

/* end-of-file: feed_prompt.c */
