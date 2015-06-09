#include "db.h"
#include "tableui.h"
#include <stdio.h>
#include <stdlib.h>
#include "generate.h"
#include <time.h>
#include "curses_os.h"

int main(void)
{
   startup_sudoku();
   create_sudoku(0,0);
   show_ui(0);
   get_input();
   shutdown_sudoku();
   return 0;
}
