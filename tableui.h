#ifndef _TABLEUI_H
#define _TABLEUI_H

#define CURS_UP 3
#define CURS_DOWN 2
#define CURS_LEFT 4
#define CURS_RIGHT 5
#define BOUNDARY 9

#define _CRT_SECURE_NO_WARNINGS

void  move_cursor(char cDirection);

void  show_ui(char cShowHelp);

char* help_text(char* cpText);

void  set_sudoku(char cSudoku[9][9]);
void  set_sudoku_pos(int iX, int iY, char cValue);

void  set_help(char cHelp[9][9][9]);
void  set_help_pos(int iX, int iY, char iValue);

void  startup_sudoku(void);
void  shutdown_sudoku(void);

char  get_input(void);

#endif
