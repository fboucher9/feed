/* See LICENSE for license details */

/*

Module: feed_keys.c

Description:
    Translation of escape sequences into key codes.

*/

#include "feed_os.h"

#include "feed_keys.h"

/* Up ^[[A */
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

/* Down ^[[B */
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

/* Right ^[[C */
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

/* Left ^[[D */
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

/* Home ^[[H */
static unsigned char const g_feed_key_home[] =
{ FEED_KEY_HOME, 0, 27, '[', 'H' };

static unsigned char const g_feed_key_home_s[] =
{ FEED_KEY_HOME, 1, 27, '[', '1', ';', '2', 'H' };

static unsigned char const g_feed_key_home_a[] =
{ FEED_KEY_HOME, 2, 27, '[', '1', ';', '3', 'H' };

static unsigned char const g_feed_key_home_s_a[] =
{ FEED_KEY_HOME, 3, 27, '[', '1', ';', '4', 'H' };

static unsigned char const g_feed_key_home_c[] =
{ FEED_KEY_HOME, 4, 27, '[', '1', ';', '5', 'H' };

static unsigned char const g_feed_key_home_s_c[] =
{ FEED_KEY_HOME, 5, 27, '[', '1', ';', '6', 'H' };

static unsigned char const g_feed_key_home_a_c[] =
{ FEED_KEY_HOME, 6, 27, '[', '1', ';', '7', 'H' };

static unsigned char const g_feed_key_home_s_a_c[] =
{ FEED_KEY_HOME, 7, 27, '[', '1', ';', '8', 'H' };

/* End ^[[F */
static unsigned char const g_feed_key_end[] =
{ FEED_KEY_END, 0, 27, '[', 'F' };

static unsigned char const g_feed_key_end_s[] =
{ FEED_KEY_END, 1, 27, '[', '1', ';', '2', 'F' };

static unsigned char const g_feed_key_end_a[] =
{ FEED_KEY_END, 2, 27, '[', '1', ';', '3', 'F' };

static unsigned char const g_feed_key_end_s_a[] =
{ FEED_KEY_END, 3, 27, '[', '1', ';', '4', 'F' };

static unsigned char const g_feed_key_end_c[] =
{ FEED_KEY_END, 4, 27, '[', '1', ';', '5', 'F' };

static unsigned char const g_feed_key_end_s_c[] =
{ FEED_KEY_END, 5, 27, '[', '1', ';', '6', 'F' };

static unsigned char const g_feed_key_end_a_c[] =
{ FEED_KEY_END, 6, 27, '[', '1', ';', '7', 'F' };

static unsigned char const g_feed_key_end_s_a_c[] =
{ FEED_KEY_END, 7, 27, '[', '1', ';', '8', 'F' };

/* Insert ^[[2;1~ */
static unsigned char const g_feed_key_insert[] =
{ FEED_KEY_INSERT, 0, 27, '[', '2', '~' };

static unsigned char const g_feed_key_insert_s[] =
{ FEED_KEY_INSERT, 1, 27, '[', '2', ';', '2', '~' };

static unsigned char const g_feed_key_insert_a[] =
{ FEED_KEY_INSERT, 2, 27, '[', '2', ';', '3', '~' };

static unsigned char const g_feed_key_insert_s_a[] =
{ FEED_KEY_INSERT, 3, 27, '[', '2', ';', '4', '~' };

static unsigned char const g_feed_key_insert_c[] =
{ FEED_KEY_INSERT, 4, 27, '[', '2', ';', '5', '~' };

static unsigned char const g_feed_key_insert_s_c[] =
{ FEED_KEY_INSERT, 5, 27, '[', '2', ';', '6', '~' };

static unsigned char const g_feed_key_insert_a_c[] =
{ FEED_KEY_INSERT, 6, 27, '[', '2', ';', '7', '~' };

static unsigned char const g_feed_key_insert_s_a_c[] =
{ FEED_KEY_INSERT, 7, 27, '[', '2', ';', '8', '~' };

/* Delete ^[[3;1~ */
static unsigned char const g_feed_key_delete[] =
{ FEED_KEY_DELETE, 0, 27, '[', '3', '~' };

static unsigned char const g_feed_key_delete_s[] =
{ FEED_KEY_DELETE, 1, 27, '[', '3', ';', '2', '~' };

static unsigned char const g_feed_key_delete_a[] =
{ FEED_KEY_DELETE, 2, 27, '[', '3', ';', '3', '~' };

static unsigned char const g_feed_key_delete_s_a[] =
{ FEED_KEY_DELETE, 3, 27, '[', '3', ';', '4', '~' };

static unsigned char const g_feed_key_delete_c[] =
{ FEED_KEY_DELETE, 4, 27, '[', '3', ';', '5', '~' };

static unsigned char const g_feed_key_delete_s_c[] =
{ FEED_KEY_DELETE, 5, 27, '[', '3', ';', '6', '~' };

static unsigned char const g_feed_key_delete_a_c[] =
{ FEED_KEY_DELETE, 6, 27, '[', '3', ';', '7', '~' };

static unsigned char const g_feed_key_delete_s_a_c[] =
{ FEED_KEY_DELETE, 7, 27, '[', '3', ';', '8', '~' };

/* PageUp ^[[5;1~ */
static unsigned char const g_feed_key_pageup[] =
{ FEED_KEY_PAGEUP, 0, 27, '[', '5', '~' };

static unsigned char const g_feed_key_pageup_s[] =
{ FEED_KEY_PAGEUP, 1, 27, '[', '5', ';', '2', '~' };

static unsigned char const g_feed_key_pageup_a[] =
{ FEED_KEY_PAGEUP, 2, 27, '[', '5', ';', '3', '~' };

static unsigned char const g_feed_key_pageup_s_a[] =
{ FEED_KEY_PAGEUP, 3, 27, '[', '5', ';', '4', '~' };

static unsigned char const g_feed_key_pageup_c[] =
{ FEED_KEY_PAGEUP, 4, 27, '[', '5', ';', '5', '~' };

static unsigned char const g_feed_key_pageup_s_c[] =
{ FEED_KEY_PAGEUP, 5, 27, '[', '5', ';', '6', '~' };

static unsigned char const g_feed_key_pageup_a_c[] =
{ FEED_KEY_PAGEUP, 6, 27, '[', '5', ';', '7', '~' };

static unsigned char const g_feed_key_pageup_s_a_c[] =
{ FEED_KEY_PAGEUP, 7, 27, '[', '5', ';', '8', '~' };

/* PageDown ^[[6;1~ */
static unsigned char const g_feed_key_pagedown[] =
{ FEED_KEY_PAGEDOWN, 0, 27, '[', '6', '~' };

static unsigned char const g_feed_key_pagedown_s[] =
{ FEED_KEY_PAGEDOWN, 1, 27, '[', '6', ';', '2', '~' };

static unsigned char const g_feed_key_pagedown_a[] =
{ FEED_KEY_PAGEDOWN, 2, 27, '[', '6', ';', '3', '~' };

static unsigned char const g_feed_key_pagedown_s_a[] =
{ FEED_KEY_PAGEDOWN, 3, 27, '[', '6', ';', '4', '~' };

static unsigned char const g_feed_key_pagedown_c[] =
{ FEED_KEY_PAGEDOWN, 4, 27, '[', '6', ';', '5', '~' };

static unsigned char const g_feed_key_pagedown_s_c[] =
{ FEED_KEY_PAGEDOWN, 5, 27, '[', '6', ';', '6', '~' };

static unsigned char const g_feed_key_pagedown_a_c[] =
{ FEED_KEY_PAGEDOWN, 6, 27, '[', '6', ';', '7', '~' };

static unsigned char const g_feed_key_pagedown_s_a_c[] =
{ FEED_KEY_PAGEDOWN, 7, 27, '[', '6', ';', '8', '~' };

/* Note: Fn keys support only one modifier at a time */

/* F1 ^[[OP */

/* F2 ^[[OQ */

/* F3 ^[[OR */

/* F4 ^[[OS */

/* F5 ^[[15;1~ */

/* etc. */

struct feed_keys_node
{
    unsigned char const *
        p_name;

    size_t
        i_name_length;

}; /* struct feed_keys_node */

static struct feed_keys_node const g_feed_keys_lookup_table[] =
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

    { g_feed_key_home, sizeof(g_feed_key_home) },

    { g_feed_key_home_s, sizeof(g_feed_key_home_s) },

    { g_feed_key_home_a, sizeof(g_feed_key_home_a) },

    { g_feed_key_home_s_a, sizeof(g_feed_key_home_s_a) },

    { g_feed_key_home_c, sizeof(g_feed_key_home_c) },

    { g_feed_key_home_s_c, sizeof(g_feed_key_home_s_c) },

    { g_feed_key_home_a_c, sizeof(g_feed_key_home_a_c) },

    { g_feed_key_home_s_a_c, sizeof(g_feed_key_home_s_a_c) },

    { g_feed_key_end, sizeof(g_feed_key_end) },

    { g_feed_key_end_s, sizeof(g_feed_key_end_s) },

    { g_feed_key_end_a, sizeof(g_feed_key_end_a) },

    { g_feed_key_end_s_a, sizeof(g_feed_key_end_s_a) },

    { g_feed_key_end_c, sizeof(g_feed_key_end_c) },

    { g_feed_key_end_s_c, sizeof(g_feed_key_end_s_c) },

    { g_feed_key_end_a_c, sizeof(g_feed_key_end_a_c) },

    { g_feed_key_end_s_a_c, sizeof(g_feed_key_end_s_a_c) },

    { g_feed_key_insert, sizeof(g_feed_key_insert) },

    { g_feed_key_insert_s, sizeof(g_feed_key_insert_s) },

    { g_feed_key_insert_a, sizeof(g_feed_key_insert_a) },

    { g_feed_key_insert_s_a, sizeof(g_feed_key_insert_s_a) },

    { g_feed_key_insert_c, sizeof(g_feed_key_insert_c) },

    { g_feed_key_insert_s_c, sizeof(g_feed_key_insert_s_c) },

    { g_feed_key_insert_a_c, sizeof(g_feed_key_insert_a_c) },

    { g_feed_key_insert_s_a_c, sizeof(g_feed_key_insert_s_a_c) },

    { g_feed_key_delete, sizeof(g_feed_key_delete) },

    { g_feed_key_delete_s, sizeof(g_feed_key_delete_s) },

    { g_feed_key_delete_a, sizeof(g_feed_key_delete_a) },

    { g_feed_key_delete_s_a, sizeof(g_feed_key_delete_s_a) },

    { g_feed_key_delete_c, sizeof(g_feed_key_delete_c) },

    { g_feed_key_delete_s_c, sizeof(g_feed_key_delete_s_c) },

    { g_feed_key_delete_a_c, sizeof(g_feed_key_delete_a_c) },

    { g_feed_key_delete_s_a_c, sizeof(g_feed_key_delete_s_a_c) },

    { g_feed_key_pageup, sizeof(g_feed_key_pageup) },

    { g_feed_key_pageup_s, sizeof(g_feed_key_pageup_s) },

    { g_feed_key_pageup_a, sizeof(g_feed_key_pageup_a) },

    { g_feed_key_pageup_s_a, sizeof(g_feed_key_pageup_s_a) },

    { g_feed_key_pageup_c, sizeof(g_feed_key_pageup_c) },

    { g_feed_key_pageup_s_c, sizeof(g_feed_key_pageup_s_c) },

    { g_feed_key_pageup_a_c, sizeof(g_feed_key_pageup_a_c) },

    { g_feed_key_pageup_s_a_c, sizeof(g_feed_key_pageup_s_a_c) },

    { g_feed_key_pagedown, sizeof(g_feed_key_pagedown) },

    { g_feed_key_pagedown_s, sizeof(g_feed_key_pagedown_s) },

    { g_feed_key_pagedown_a, sizeof(g_feed_key_pagedown_a) },

    { g_feed_key_pagedown_s_a, sizeof(g_feed_key_pagedown_s_a) },

    { g_feed_key_pagedown_c, sizeof(g_feed_key_pagedown_c) },

    { g_feed_key_pagedown_s_c, sizeof(g_feed_key_pagedown_s_c) },

    { g_feed_key_pagedown_a_c, sizeof(g_feed_key_pagedown_a_c) },

    { g_feed_key_pagedown_s_a_c, sizeof(g_feed_key_pagedown_s_a_c) },

};

static unsigned int const g_feed_keys_lookup_table_length =
(unsigned int)(
    sizeof(g_feed_keys_lookup_table)
    / sizeof(g_feed_keys_lookup_table[0u]));

static
char
feed_keys_compare_node(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length,
    struct feed_keys_node const * const
        p_node)
{
    char
        b_result;

    if (
        (i_data_length + 2u)
        == p_node->i_name_length)
    {
        if (
            0
            == memcmp(
                p_data,
                p_node->p_name + 2u,
                i_data_length))
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

char
feed_keys_lookup(
    unsigned char const * const
        p_data,
    unsigned int const
        i_data_length,
    struct feed_keys_descriptor * const
        p_info)
{
    char
        b_result;

    unsigned int
        i;

    struct feed_keys_node const *
        p_node;

    b_result =
        0;

    i =
        0;

    while (
        !(b_result)
        && (
            i < g_feed_keys_lookup_table_length))
    {
        p_node =
            g_feed_keys_lookup_table + i;

        b_result =
            feed_keys_compare_node(
                p_data,
                i_data_length,
                p_node);

        if (
            b_result)
        {
            p_info->i_code =
                p_node->p_name[0u];

            p_info->i_mods =
                p_node->p_name[1u];
        }
        else
        {
            i ++;
        }
    }

    return
        b_result;

} /* feed_keys_lookup() */

static unsigned char const g_feed_key_print_up[] =
{ FEED_KEY_UP, 'U', 'p' };

static unsigned char const g_feed_key_print_down[] =
{ FEED_KEY_DOWN, 'D', 'o', 'w', 'n' };

static unsigned char const g_feed_key_print_right[] =
{ FEED_KEY_RIGHT, 'R', 'i', 'g', 'h', 't' };

static unsigned char const g_feed_key_print_left[] =
{ FEED_KEY_LEFT, 'L', 'e', 'f', 't' };

static unsigned char const g_feed_key_print_home[] =
{ FEED_KEY_HOME, 'H', 'o', 'm', 'e' };

static unsigned char const g_feed_key_print_end[] =
{ FEED_KEY_END, 'E', 'n', 'd' };

static unsigned char const g_feed_key_print_insert[] =
{ FEED_KEY_INSERT, 'I', 'n', 's', 'e', 'r', 't' };

static unsigned char const g_feed_key_print_delete[] =
{ FEED_KEY_DELETE, 'D', 'e', 'l', 'e', 't', 'e' };

static unsigned char const g_feed_key_print_pageup[] =
{ FEED_KEY_PAGEUP, 'P', 'a', 'g', 'e', 'U', 'p' };

static unsigned char const g_feed_key_print_pagedown[] =
{ FEED_KEY_PAGEDOWN, 'P', 'a', 'g', 'e', 'D', 'o', 'w', 'n' };

static struct feed_keys_node const g_feed_keys_print_table[] =
{
    { g_feed_key_print_up, sizeof(g_feed_key_print_up) },

    { g_feed_key_print_down, sizeof(g_feed_key_print_down) },

    { g_feed_key_print_right, sizeof(g_feed_key_print_right) },

    { g_feed_key_print_left, sizeof(g_feed_key_print_left) },

    { g_feed_key_print_home, sizeof(g_feed_key_print_home) },

    { g_feed_key_print_end, sizeof(g_feed_key_print_end) },

    { g_feed_key_print_insert, sizeof(g_feed_key_print_insert) },

    { g_feed_key_print_delete, sizeof(g_feed_key_print_delete) },

    { g_feed_key_print_pageup, sizeof(g_feed_key_print_pageup) },

    { g_feed_key_print_pagedown, sizeof(g_feed_key_print_pagedown) },

};

static unsigned int const g_feed_keys_print_table_length =
(unsigned int)(
    sizeof(g_feed_keys_print_table)
    / sizeof(g_feed_keys_print_table[0u]));

char
feed_keys_print(
    struct feed_keys_descriptor const * const
        p_info,
    unsigned char * const
        p_buf,
    unsigned int const
        i_buf_len,
    unsigned int * const
        p_actual)
{
    char
        b_result;

    unsigned int
        i_actual;

    unsigned int
        i;

    struct feed_keys_node const *
        p_node;

    unsigned int
        i_name_length;

    i_actual =
        0u;

    b_result =
        0;

    i =
        0u;

    if (
        FEED_MOD_SHIFT & p_info->i_mods)
    {
        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = 'S';
            i_actual ++;
        }

        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = '-';
            i_actual ++;
        }
    }

    if (
        FEED_MOD_ALT & p_info->i_mods)
    {
        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = 'A';
            i_actual ++;
        }

        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = '-';
            i_actual ++;
        }
    }

    if (
        FEED_MOD_CTRL & p_info->i_mods)
    {
        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = 'C';
            i_actual ++;
        }

        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] = '-';
            i_actual ++;
        }
    }

    while (
        (!b_result)
        && (
            i < g_feed_keys_print_table_length))
    {
        p_node = g_feed_keys_print_table + i;

        if (
            p_info->i_code
            == p_node->p_name[0u])
        {
            i_name_length =
                (unsigned int)(
                    p_node->i_name_length - 1u);

            if (
                (i_actual + i_name_length) <= i_buf_len)
            {
                memcpy(
                    p_buf + i_actual,
                    p_node->p_name + 1u,
                    i_name_length);

                i_actual +=
                    i_name_length;
            }

            *(p_actual) =
                i_actual;

            b_result =
                1;
        }
        else
        {
            i++;
        }
    }

    if (!(b_result))
    {
        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] =
                (unsigned char)(
                    '0' + ((p_info->i_code / 100u) % 10u));
            i_actual ++;
        }

        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] =
                (unsigned char)(
                    '0' + ((p_info->i_code / 10u) % 10u));
            i_actual ++;
        }

        if (i_actual <  i_buf_len)
        {
            p_buf[i_actual] =
                (unsigned char)(
                    '0' + ((p_info->i_code / 1u) % 10u));
            i_actual ++;
        }

        b_result =
            1;
    }

    return
        b_result;

} /* feed_keys_print() */

/* end-of-file: feed_keys.c */
