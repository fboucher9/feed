/* See LICENSE for license details */

/*

Module: feed_prompt.h

Description:

*/

/* Header file dependency */
#if !defined(INC_FEED_OS_H)
#error include feed_os.h first
#endif /* #if !defined(INC_FEED_OS_H) */

/* Reverse include guard */
#if defined(INC_FEED_PROMPT_H)
#error include feed_prompt.h once
#endif /* #if defined(INC_FEED_PROMPT_H) */

#define INC_FEED_PROMPT_H

struct feed_client;

struct feed_line;

struct feed_prompt
{
    struct feed_client *
        p_client;

    struct feed_line *
        a_prompt[2u];

}; /* struct feed_prompt */

struct feed_prompt *
feed_prompt_create(
    struct feed_client * const
        p_client);

void
feed_prompt_destroy(
    struct feed_prompt * const
        p_prompt);

char
feed_prompt_set1(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length);

char
feed_prompt_set2(
    struct feed_prompt * const
        p_prompt,
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length);

struct feed_line *
feed_prompt_get1(
    struct feed_prompt * const
        p_prompt);

struct feed_line *
feed_prompt_get2(
    struct feed_prompt * const
        p_prompt);

/* end-of-file: feed_prompt.h */
