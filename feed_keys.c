/* See LICENSE for license details */

/*

Module: feed_keys.c

Description:
    Translation of escape sequences into key codes.

*/

#include "feed_os.h"

#include "feed_keys.h"

static unsigned char const g_feed_key_up[] =
{ FEED_KEY_UP, 0, 27, '[', 'A' };

static unsigned char const g_feed_key_up_s[] =
{ FEED_KEY_UP, 1, 27, '[', '1', ';', '2', 'A' };

static unsigned char const g_feed_key_up_a[] =
{ FEED_KEY_UP, 2, 27, '[', '1', ';', '3', 'A' };

static unsigned char const g_feed_key_up_s_a[] =
{ FEED_KEY_UP, 3, 27, '[', '1', ';', '4', 'A' };

static unsigned char const g_feed_key_up_c[] =
{ FEED_KEY_UP, 4, 27, '[', '1', ';', '5', 'A' };

static unsigned char const g_feed_key_up_s_c[] =
{ FEED_KEY_UP, 5, 27, '[', '1', ';', '6', 'A' };

static unsigned char const g_feed_key_up_a_c[] =
{ FEED_KEY_UP, 6, 27, '[', '1', ';', '7', 'A' };

static unsigned char const g_feed_key_up_s_a_c[] =
{ FEED_KEY_UP, 7, 27, '[', '1', ';', '8', 'A' };

static unsigned char const g_feed_key_down[] =
{ FEED_KEY_DOWN, 0, 27, '[', 'B' };

static unsigned char const g_feed_key_down_s[] =
{ FEED_KEY_DOWN, 1, 27, '[', '1', ';', '2', 'B' };

static unsigned char const g_feed_key_down_a[] =
{ FEED_KEY_DOWN, 2, 27, '[', '1', ';', '3', 'B' };

static unsigned char const g_feed_key_down_s_a[] =
{ FEED_KEY_DOWN, 3, 27, '[', '1', ';', '4', 'B' };

static unsigned char const g_feed_key_down_c[] =
{ FEED_KEY_DOWN, 4, 27, '[', '1', ';', '5', 'B' };

static unsigned char const g_feed_key_down_s_c[] =
{ FEED_KEY_DOWN, 5, 27, '[', '1', ';', '6', 'B' };

static unsigned char const g_feed_key_down_a_c[] =
{ FEED_KEY_DOWN, 6, 27, '[', '1', ';', '7', 'B' };

static unsigned char const g_feed_key_down_s_a_c[] =
{ FEED_KEY_DOWN, 7, 27, '[', '1', ';', '8', 'B' };

static unsigned char const g_feed_key_right[] =
{ FEED_KEY_RIGHT, 0, 27, '[', 'C' };

static unsigned char const g_feed_key_right_s[] =
{ FEED_KEY_RIGHT, 1, 27, '[', '1', ';', '2', 'C' };

static unsigned char const g_feed_key_right_a[] =
{ FEED_KEY_RIGHT, 2, 27, '[', '1', ';', '3', 'C' };

static unsigned char const g_feed_key_right_s_a[] =
{ FEED_KEY_RIGHT, 3, 27, '[', '1', ';', '4', 'C' };

static unsigned char const g_feed_key_right_c[] =
{ FEED_KEY_RIGHT, 4, 27, '[', '1', ';', '5', 'C' };

static unsigned char const g_feed_key_right_s_c[] =
{ FEED_KEY_RIGHT, 5, 27, '[', '1', ';', '6', 'C' };

static unsigned char const g_feed_key_right_a_c[] =
{ FEED_KEY_RIGHT, 6, 27, '[', '1', ';', '7', 'C' };

static unsigned char const g_feed_key_right_s_a_c[] =
{ FEED_KEY_RIGHT, 7, 27, '[', '1', ';', '8', 'C' };

static unsigned char const g_feed_key_left[] =
{ FEED_KEY_LEFT, 0, 27, '[', 'D' };

static unsigned char const g_feed_key_left_s[] =
{ FEED_KEY_LEFT, 1, 27, '[', '1', ';', '2', 'D' };

static unsigned char const g_feed_key_left_a[] =
{ FEED_KEY_LEFT, 2, 27, '[', '1', ';', '3', 'D' };

static unsigned char const g_feed_key_left_s_a[] =
{ FEED_KEY_LEFT, 3, 27, '[', '1', ';', '4', 'D' };

static unsigned char const g_feed_key_left_c[] =
{ FEED_KEY_LEFT, 4, 27, '[', '1', ';', '5', 'D' };

static unsigned char const g_feed_key_left_s_c[] =
{ FEED_KEY_LEFT, 5, 27, '[', '1', ';', '6', 'D' };

static unsigned char const g_feed_key_left_a_c[] =
{ FEED_KEY_LEFT, 6, 27, '[', '1', ';', '7', 'D' };

static unsigned char const g_feed_key_left_s_a_c[] =
{ FEED_KEY_LEFT, 7, 27, '[', '1', ';', '8', 'D' };

/* Home [H */
/* End [F */
/* Insert [2;1~ */
/* Delete [3;1~ */
/* PageUp [5;1~ */
/* PageDown [6;1~ */
/* F1 [OP */
/* F2 [OQ */
/* F3 [OR */
/* F4 [OS */
/* F5 [15;1~ */
/* etc. */

struct feed_key_node
{
    unsigned char const *
        p_name;

    size_t
        i_name_length;

}; /* struct feed_key_node */

static struct feed_key_node const g_feed_key_table[] =
{
    { g_feed_key_up, sizeof(g_feed_key_up) },

    { g_feed_key_up_s, sizeof(g_feed_key_up_s) },

    { g_feed_key_up_a, sizeof(g_feed_key_up_a) },

    { g_feed_key_up_s_a, sizeof(g_feed_key_up_s_a) },

    { g_feed_key_up_c, sizeof(g_feed_key_up_c) },

    { g_feed_key_up_s_c, sizeof(g_feed_key_up_s_c) },

    { g_feed_key_up_a_c, sizeof(g_feed_key_up_a_c) },

    { g_feed_key_up_s_a_c, sizeof(g_feed_key_up_s_a_c) },

    { g_feed_key_down, sizeof(g_feed_key_down) },

    { g_feed_key_down_s, sizeof(g_feed_key_down_s) },

    { g_feed_key_down_a, sizeof(g_feed_key_down_a) },

    { g_feed_key_down_s_a, sizeof(g_feed_key_down_s_a) },

    { g_feed_key_down_c, sizeof(g_feed_key_down_c) },

    { g_feed_key_down_s_c, sizeof(g_feed_key_down_s_c) },

    { g_feed_key_down_a_c, sizeof(g_feed_key_down_a_c) },

    { g_feed_key_down_s_a_c, sizeof(g_feed_key_down_s_a_c) },

    { g_feed_key_right, sizeof(g_feed_key_right) },

    { g_feed_key_right_s, sizeof(g_feed_key_right_s) },

    { g_feed_key_right_a, sizeof(g_feed_key_right_a) },

    { g_feed_key_right_s_a, sizeof(g_feed_key_right_s_a) },

    { g_feed_key_right_c, sizeof(g_feed_key_right_c) },

    { g_feed_key_right_s_c, sizeof(g_feed_key_right_s_c) },

    { g_feed_key_right_a_c, sizeof(g_feed_key_right_a_c) },

    { g_feed_key_right_s_a_c, sizeof(g_feed_key_right_s_a_c) },

    { g_feed_key_left, sizeof(g_feed_key_left) },

    { g_feed_key_left_s, sizeof(g_feed_key_left_s) },

    { g_feed_key_left_a, sizeof(g_feed_key_left_a) },

    { g_feed_key_left_s_a, sizeof(g_feed_key_left_s_a) },

    { g_feed_key_left_c, sizeof(g_feed_key_left_c) },

    { g_feed_key_left_s_c, sizeof(g_feed_key_left_s_c) },

    { g_feed_key_left_a_c, sizeof(g_feed_key_left_a_c) },

    { g_feed_key_left_s_a_c, sizeof(g_feed_key_left_s_a_c) },

};

char
feed_keys_lookup(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length,
    struct feed_key * const
        p_info)
{
    char
        b_result;

    (void)(
        p_data);
    (void)(
        i_data_length);
    (void)(
        p_info);

    b_result =
        0;

    return
        b_result;

} /* feed_keys_lookup() */

/* end-of-file: feed_keys.c */
