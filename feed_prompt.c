/* See LICENSE for license details */

/*

Module: feed_prompt.c

Description:

*/

#include "feed_os.h"

#include "feed_prompt.h"

#include "feed_object.h"

#include "feed_list.h"

#include "feed_line.h"

#include "feed_utf8.h"

static
char
feed_prompt_init(
    void * const
        p_buf,
    struct feed_client * const
        p_client,
    void const * const
        p_descriptor)
{
    struct feed_prompt *
        p_prompt;

    (void)(
        p_descriptor);

    p_prompt =
        (struct feed_prompt *)(
            p_buf);

    if (
        p_prompt)
    {
        p_prompt->p_client =
            p_client;

        p_prompt->a_prompt[0u] =
            NULL;

        p_prompt->a_prompt[1u] =
            NULL;

    }

    return
        1;

}

static
void
feed_prompt_cleanup(
    void * const
        p_buf)
{
    struct feed_prompt *
        p_prompt;

    p_prompt =
        (struct feed_prompt *)(
            p_buf);

    if (
        p_prompt)
    {
        p_prompt->p_client =
            NULL;

        if (
            p_prompt->a_prompt[0u])
        {
            feed_line_destroy(
                p_prompt->a_prompt[0u]);

            p_prompt->a_prompt[0u] =
                NULL;
        }

        if (
            p_prompt->a_prompt[1u])
        {
            feed_line_destroy(
                p_prompt->a_prompt[1u]);

            p_prompt->a_prompt[1u] =
                NULL;
        }

    }
}

struct feed_prompt *
feed_prompt_create(
    struct feed_client * const
        p_client)
{
    return
        (struct feed_prompt *)(
            feed_object_create(
                p_client,
                (unsigned int)(
                    sizeof(
                        struct feed_prompt)),
                &(
                    feed_prompt_init),
                NULL));
}

void
feed_prompt_destroy(
    struct feed_prompt * const
        p_prompt)
{
    if (
        p_prompt)
    {
        feed_object_destroy(
            p_prompt->p_client,
            p_prompt,
            &(
                feed_prompt_cleanup));
    }
}

static
char
feed_prompt_set(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned long int const
        i_data_length,
    unsigned int const
        i_index)
{
    char
        b_result;

    /* Delete the previous prompt */
    if (p_prompt && (i_index < 2u))
    {
        if (
            p_prompt->a_prompt[i_index])
        {
            feed_line_destroy(
                p_prompt->a_prompt[i_index]);

            p_prompt->a_prompt[i_index] =
                (struct feed_line *)(
                    0);
        }

        if (
            i_data_length)
        {
            p_prompt->a_prompt[i_index] =
                feed_line_create(
                    p_prompt->p_client);

            if (
                p_prompt->a_prompt[i_index])
            {
                feed_line_set(
                    p_prompt->a_prompt[i_index],
                    p_data,
                    i_data_length);

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
    unsigned long int const
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
    unsigned long int const
        i_data_length)
{
    return
        feed_prompt_set(
            p_prompt,
            p_data,
            i_data_length,
            1u);
}

static
struct feed_line *
feed_prompt_get(
    struct feed_prompt * const
        p_prompt,
    unsigned int const
        i_index)
{
    struct feed_line *
        p_line;

    if (p_prompt && (i_index < 2u))
    {
        p_line =
            p_prompt->a_prompt[i_index];
    }
    else
    {
        p_line =
            (struct feed_line *)(
                0);
    }

    return
        p_line;

}

struct feed_line *
feed_prompt_get1(
    struct feed_prompt * const
        p_prompt)
{
    return
        feed_prompt_get(
            p_prompt,
            0u);

}

struct feed_line *
feed_prompt_get2(
    struct feed_prompt * const
        p_prompt)
{
    return
        feed_prompt_get(
            p_prompt,
            1u);

}

/* end-of-file: feed_prompt.c */
