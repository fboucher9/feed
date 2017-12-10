/* See LICENSE for license details */

/*

Module: feed_keys.h

Description:
    Translation of escape sequences into key codes.

*/

#if defined(INC_FEED_KEYS_H)
#error include once
#endif /* #if defined(INC_FEED_KEYS_H) */

#define INC_FEED_KEYS_H

/* Ctrl combinations */

/* 0 : ctrl-@, ctrl-2 */
/* 1-26 : ctrl-a -> ctrl-z */
/* 27 : ctrl-[ ctrl-3 */
/* 28 : ctrl-\ ctrl-4 */
/* 29 : ctrl-] ctrl-5 */
/* 30 : ctrl-^, ctrl-6 */
/* 31 : ctrl-_, ctrl-7 */

/* --- Interface --- */

unsigned short int
feed_keys_lookup(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length);

struct feed_buf;

void
feed_keys_print(
    unsigned short int const
        i_code,
    struct feed_buf * const
        p_buf);

/* end-of-file: feed_keys.h */
