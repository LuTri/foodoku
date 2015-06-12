#ifndef _CURSES_OS_H
#define _CURSES_IS_H

#include <stdlib.h>

#ifndef OS_LINUX

#include "curses.h"

#define FREE_SQL(x) free(x);

#define CLEAR_SCREEN system("cls");

#else

#include <ncurses.h>

#define FREE_SQL(x)

#define CLEAR_SCREEN system("clear");

#endif


#endif

