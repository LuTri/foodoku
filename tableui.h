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
/* Kantenlänge des gesamten Sudokus */
#define BOUNDARY 9
/* Anzahl der Felder im Sudoku */
#define BOUNDARY_SQUARE (BOUNDARY * BOUNDARY)
/* Kangentlänge des kleinen Quadrats */
#define BOUNDARY_ROOT (int)(sqrt((double)BOUNDARY))

#define MIN_WIDTH 145
#define MIN_HEIGHT 45

#define _CRT_SECURE_NO_WARNINGS

extern char cSudoku[BOUNDARY][BOUNDARY];
extern char cShownSudoku[BOUNDARY][BOUNDARY];


/* 
   ============================================================================
   Nach außen verfügbare Schnittstellen
   ============================================================================ 
*/

void  move_cursor(char cDirection);

void  show_ui(char cShowHelp);
void  show_result(int iHelps, int iFilled, int iSeconds);

void  null_sudoku(void);

void  clear_help(int iX, int iY);
void  set_help(char cHelpData[BOUNDARY][BOUNDARY][BOUNDARY]);
void  set_help_pos(int iX, int iY, char iValue);

void  startup_sudoku(void);
void  shutdown_sudoku(void);

char  get_input(void);
void  get_cursor_pos(int* iX, int* iY);

int   getTerminalSize(void);

#endif
