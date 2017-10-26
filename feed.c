#include "feed_os.h"

#include "feed.h"

#include "feed_client.h"

#include "feed_heap.h"

#include "feed_tty.h"

#include "feed_input.h"

#include "feed_keys.h"

struct feed_handle
{
    /* -- */

    struct feed_client *
        p_client;

    struct feed_heap *
        p_heap;

    /* -- */

    struct feed_body_text *
        p_body_text;

    unsigned char *
        p_prompt;

    /* -- */

    struct feed_descriptor
        o_descriptor;

    struct feed_client
        o_client;

    struct feed_tty
        o_tty;

    /* -- */

    unsigned int
        i_prompt_len;

    unsigned int
        i_ocx;

    unsigned int
        i_ocy;

    unsigned int
        i_wx;

    /* -- */

    unsigned int
        i_wy;

    unsigned int
        a_padding_int[3u];

    /* -- */

    char
        b_more;

    unsigned char
        a_padding[7u];

}; /* struct feed_handle */

static
char
feed_init(
    struct feed_handle * const
        p_this,
    struct feed_heap * const
        p_heap,
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    char
        b_result;

    memset(
        p_this,
        0x00u,
        sizeof(
            *(
                p_this)));

    p_this->p_client =
        &(
            p_this->o_client);

    feed_client_init(
        p_this->p_client);

    p_this->p_heap =
        p_heap;

    p_this->o_client.p_heap =
        p_heap;

    p_this->o_descriptor =
        *(
            p_feed_descriptor);

    b_result =
        1;

    return
        b_result;

} /* feed_init() */

static
void
feed_cleanup(
    struct feed_handle * const
        p_this)
{
    if (p_this->i_prompt_len)
    {
        feed_heap_free(
            p_this->p_heap,
            (void *)(
                p_this->p_prompt));

        p_this->p_prompt =
            (unsigned char *)(
                0);

        p_this->i_prompt_len =
            0u;
    }

    feed_client_cleanup(
        p_this->p_client);

    p_this->p_client =
        (struct feed_client *)(
            0);

    p_this->p_heap =
        (struct feed_heap *)(
            0);

} /* feed_cleanup() */

struct feed_handle *
feed_create(
    struct feed_descriptor const * const
        p_feed_descriptor)
{
    struct feed_handle *
        p_this;

    struct feed_heap *
        p_heap;

    p_heap =
        feed_heap_create();

    if (
        p_heap)
    {
        p_this =
            (struct feed_handle *)(
                feed_heap_alloc(
                    p_heap,
                    sizeof(
                        struct feed_handle)));

        if (
            p_this)
        {
            if (
                feed_init(
                    p_this,
                    p_heap,
                    p_feed_descriptor))
            {
            }
            else
            {
                feed_heap_free(
                    p_heap,
                    (void *)(
                        p_this));

                p_this =
                    (struct feed_handle *)(
                        0);
            }
        }

        if (
            !(p_this))
        {
            feed_heap_destroy(
                p_heap);
        }
    }

    return
        p_this;

} /* feed_create() */

void
feed_destroy(
    struct feed_handle * const
        p_this)
{
    struct feed_heap *
        p_heap;

    p_heap =
        p_this->p_heap;

    feed_cleanup(
        p_this);

    feed_heap_free(
        p_heap,
        p_this);

} /* feed_destroy() */

int
feed_prompt(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    int
        i_result;

    /* Free previous prompt */
    if (p_this->i_prompt_len)
    {
        feed_heap_free(
            p_this->p_heap,
            (void *)(
                p_this->p_prompt));

        p_this->p_prompt =
            (unsigned char *)(
                0);

        p_this->i_prompt_len =
            0u;
    }

    /* Allocate new prompt */
    if (i_data_length)
    {
        p_this->p_prompt =
            (unsigned char *)(
                feed_heap_alloc(
                    p_this->p_heap,
                    i_data_length));

        if (p_this->p_prompt)
        {
            memcpy(
                p_this->p_prompt,
                p_data,
                i_data_length);

            p_this->i_prompt_len =
                i_data_length;

            i_result =
                0;
        }
        else
        {
            i_result =
                -1;
        }
    }
    else
    {
        i_result =
            0;
    }

    return
        i_result;

} /* feed_prompt() */

int
feed_start(
    struct feed_handle * const
        p_this)
{
    (void)(
        p_this);

    return
        -1;

} /* feed_start() */

int
feed_stop(
    struct feed_handle * const
        p_this)
{
    (void)(
        p_this);

    return
        -1;

} /* feed_stop() */

int
feed_suggest(
    struct feed_handle * const
        p_this,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length)
{
    (void)(
        p_this);
    (void)(
        p_data);
    (void)(
        i_data_length);

    return
        -1;

} /* feed_suggest() */

/* end-of-file: feed.c */
