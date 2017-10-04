/* See LICENCE for license details */

/*

Module: feed_os.h

Description:

    Include of operating system header files and precompiled header file.

*/

#include <stdarg.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <termios.h>

#include <errno.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <sys/ioctl.h>

/* Avoid unused macro warning while using precompiled header file */
#if defined(__GNUC__)
#if defined(__SSP_STRONG__)
#endif
#endif

/* end-of-file: feed_os.h */
