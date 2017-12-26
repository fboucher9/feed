/* See LICENCE for license details */

/*

*/

/* Header include protection */
#if defined(INC_FEED_DICT_H)
#error include feed_dict.h once
#endif /* #if defined(INC_FEED_DICT_H) */

#define INC_FEED_DICT_H

/* Header file dependencies */
#if !defined(INC_FEED_LIST_H)
#error include feed_list.h before
#endif /* #if !defined(INC_FEED_LIST_H) */

/* Header file dependencies */
#if !defined(INC_FEED_BUF_H)
#error include feed_buf.h before
#endif /* #if !defined(INC_FEED_BUF_H) */

struct feed_client;

struct feed_dict;

struct feed_dict *
feed_dict_create(
    struct feed_client * const
        p_client);

void
feed_dict_destroy(
    struct feed_dict * const
        p_dict);

char
feed_dict_add(
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf);

char
feed_dict_lookup(
    struct feed_dict * const
        p_dict,
    struct feed_buf const * const
        p_buf);

/* end-of-file: feed_dict.h */
