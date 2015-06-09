/* 
   ============================================================================
   Programmname: tableui.h
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung von Programm-Funkionen zum Anzeigen des Spiel-
                 Status.
   ============================================================================ 
*/

/* 
   ============================================================================
   Präprozessor-Anweisungen, globale defines
   ============================================================================ 
*/

#ifndef _TABLEUI_H
#define _TABLEUI_H

#include <math.h>

#define CURS_UP 3
#define CURS_DOWN 2
#define CURS_LEFT 4
#define CURS_RIGHT 5
#define BOUNDARY 9
#define BOUNDARY_SQUARE (BOUNDARY * BOUNDARY)
#define BOUNDARY_ROOT (int)(sqrt((double)BOUNDARY))

#define _CRT_SECURE_NO_WARNINGS

extern char cSudoku[BOUNDARY][BOUNDARY];


/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

void  move_cursor(char cDirection);

void  show_ui(char cShowHelp);

char* help_text(char* cpText);

void  null_sudoku(void);

void  clear_help(int iX, int iY);
void  set_help(char cHelpData[BOUNDARY][BOUNDARY][BOUNDARY]);
void  set_help_pos(int iX, int iY, char iValue);

void  startup_sudoku(void);
void  shutdown_sudoku(void);

char  get_input(void);

int getTerminalSize(void);

#endif
