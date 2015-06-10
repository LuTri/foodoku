#include "db.h"
#include "tableui.h"
#include <stdio.h>
#include <stdlib.h>
#include "menue.h"

int main(void)
{
	startup_sudoku();
	show_ui(1);

	get_input();
	shutdown_sudoku();
	return 0;
}