#ifndef _CURSES_OS_H
#define _CURSES_IS_H

#ifndef OS_LINUX

#include "curses.h"

#define FREE_SQL(x) free(x);

#else

#include <ncurses.h>

#define FREE_SQL(x)

#endif


#endif

