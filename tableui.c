#include "tableui.h"
#include "curses.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEP_OUTER "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
#define SEP_INNER "@-----+-----+-----@-----+-----+-----@-----+-----+-----@\n"
#define SEP_VERTI "@     |     |     @     |     |     @     |     |     @\n"

char cSudoku[BOUNDARY][BOUNDARY];

char cHelp[BOUNDARY][BOUNDARY][BOUNDARY];

char cCursorPos[2] = {0,0};

char get_input(void)
{
   return wgetch(stdscr);
}

char* help_text(char* cpText)
{
   static char* cpHelpText = NULL;
   
   if (cpText && cpHelpText)
   {
      free(cpHelpText);
   }

   if (cpText)
   {
      cpHelpText = (char*) malloc(strlen(cpText) + 1);
      strcpy(cpHelpText,cpText);
   }

   return cpHelpText;
}

char pretty_value(char cValue) {
   if (cValue > 0)
   {
      return cValue + '0';
   }
   return ' ';
}

void move_cursor(char cDirection)
{
   switch (cDirection)
   {
      case CURS_UP:
         cCursorPos[0]--;
         if (cCursorPos[0] == -1)
         {
            cCursorPos[0] = BOUNDARY - 1;
         }
         break;
      case CURS_DOWN:
         cCursorPos[0]++;
         cCursorPos[0] = cCursorPos[0] % BOUNDARY;
         break;
      case CURS_LEFT:
         cCursorPos[1]--;
         if (cCursorPos[1] == -1)
         {
            cCursorPos[1] = BOUNDARY - 1;
         }
         break;
      case CURS_RIGHT:
         cCursorPos[1]++;
         cCursorPos[1] = cCursorPos[1] % BOUNDARY;
         break;
   }
}

void show_help(void)
{
   int i;

   for (i = 0; i < BOUNDARY; i++)
   {
      int iX, iY;
      char cValue;
      iY = cCursorPos[0] * 4 + 1 + i / 3;
      iX = cCursorPos[1] * 6 + 2 + i % 3;
      cValue = cHelp[cCursorPos[0]][cCursorPos[1]][i];
      mvaddch(iY, iX, pretty_value(cValue));
   }
}

void show_ui(char cShowHelp)
{
   int i,j;
   char* cpHelpText;

   move(0,0);
   for (i = 0; i < BOUNDARY; i++)
   {
      if (i % 3 == 0)
      {
         printw(SEP_OUTER);
      }
      else
      {
      printw(SEP_INNER);
      }
      printw(SEP_VERTI);
      for (j = 0; j < BOUNDARY; j++) {
         if (j % 3 == 0)
         {
            printw("@");
         }
         else
         {
            printw("|");
         }
         printw("  ");
         printw("%c", pretty_value(cSudoku[i][j]));
         printw("  ");
      }
      printw("@\n");
      printw(SEP_VERTI);
   }
   printw(SEP_OUTER);

   cpHelpText = help_text(NULL);
   if (cpHelpText)
   {
      printw("\n\n%s",cpHelpText);
   }
   if (cShowHelp)
   {
      show_help();
   }

   refresh();
   wmove(stdscr,cCursorPos[0] * 4 + 2, cCursorPos[1] * 6 + 3);
}

void set_sudoku(char sudoku[BOUNDARY][BOUNDARY])
{
   int i,j;

   for (i = 0; i < BOUNDARY; i++)
   {
      for (j = 0; j < BOUNDARY; j++) {
         cSudoku[i][j] = sudoku[i][j];
      }
   }
}

void set_sudoku_pos(int iX, int iY, char cValue)
{
   cSudoku[iY][iX] = cValue;
}

void startup_sudoku(void)
{
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   curs_set(2);
}

void shutdown_sudoku(void)
{
   endwin();
}