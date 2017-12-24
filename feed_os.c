/* See LICENSE for license details */

/*

*/

#include "feed_os.h"

#include "feed_main.h"

#include "feed_buf.h"

static
char
feed_options_init(
    struct feed_options * const
        p_options,
    int const
        argc,
    char ** const
        argv,
    void * * const
        pp_placement)
{
    char
        b_result;

    int
        argi;

    unsigned long int
        i_placement_length;

    void *
        p_placement;

    i_placement_length =
        (unsigned long int)(
            (unsigned int)(
                argc)
            * sizeof(
                struct feed_buf));

    for (
        argi = 0;
        argi < argc;
        argi ++)
    {
        i_placement_length +=
            (unsigned long int)(
                strlen(
                    argv[
                        argi]));
    }

    p_placement =
        malloc(
            i_placement_length);

    if (
        p_placement)
    {
        struct feed_buf *
            p_table;

        unsigned char *
            p_data;

        p_table =
            (struct feed_buf *)(
                p_placement);

        p_options->p_min =
            p_table;

        p_options->p_max =
            p_table + argc;

        p_data =
            (unsigned char *)(
                p_table + argc);

        for (
            argi = 0;
            argi < argc;
            argi ++)
        {
            struct feed_buf *
                p_buf;

            unsigned long int
                argl;

            p_buf =
                p_table + argi;

            argl =
                strlen(
                    argv[argi]);

            memcpy(
                p_data,
                argv[argi],
                argl);

            p_buf->o_min.p =
                p_data;

            p_data +=
                argl;

            p_buf->o_max.p =
                p_data;
        }

        *(
            pp_placement) =
            p_placement;

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

static
void
feed_options_cleanup(
    void * const
        p_placement)
{
    free(
        p_placement);
}

#if 0
static
void
feed_options_report(
    struct feed_options const * const
        p_options)
{
    struct feed_buf const *
        p_iterator;

    unsigned int
        argi;

    printf("options count = %ld\n",
        (signed long int)(
            p_options->p_max - p_options->p_min));

    p_iterator =
        p_options->p_min;

    argi =
        0u;

    while (
        p_iterator != p_options->p_max)
    {
        unsigned long int
            argl;

        argl =
            (unsigned long int)(
                p_iterator->o_max.pc - p_iterator->o_min.pc);

        printf("option index=%u len=%lu value=%.*s\n",
            argi,
            argl,
            (int)(
                argl),
            p_iterator->o_min.pc);

        p_iterator ++;

        argi ++;
    }
}
#endif

int
main(
    int
        argc,
    char**
        argv)
{
    int
        i_result;

    struct feed_options
        o_options;

    void *
        p_placement;

    if (
        feed_options_init(
            &(
                o_options),
            argc,
            argv,
            &(
                p_placement)))
    {
#if 0
        feed_options_report(
            &(
                o_options));
#endif

        i_result =
            feed_main(
                &(
                    o_options));

        feed_options_cleanup(
            p_placement);
    }

    return
        i_result;

}

/* end-of-file: feed_os.c */
