/* 
   ============================================================================
   Programmname: tableui.c
   Autor       : Tristan Lucas
                 Heinrich-Hertz-Berufskolleg
   Datum       : 
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung von Programm-Funkionen zum Anzeigen des Spiel-
                 Status.

   Programmschnittstelle: Siehe zugeh�rige Header-Datei  
   ============================================================================ 
*/

/* 
   ============================================================================
   Includedateien
   ============================================================================ 
*/
#include "tableui.h"
#include "curses_os.h"
#include "fancy_box.h"
#include "menue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
   ============================================================================
   Vordefinierte Berahmung
   ============================================================================ 
*/

#define SEP_OUTER "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
#define SEP_INNER "@-----+-----+-----@-----+-----+-----@-----+-----+-----@\n"
#define SEP_VERTI "@     |     |     @     |     |     @     |     |     @\n"


/* 
   ============================================================================
   globale Variablen
   ============================================================================ 
*/

char cSudoku[BOUNDARY][BOUNDARY];

char cShownSudoku[BOUNDARY][BOUNDARY];

/* 
   ============================================================================
   F�r dieses Modul globale Variablen
   ============================================================================ 
*/

char cHelp[BOUNDARY][BOUNDARY][BOUNDARY];

char cCursorPos[2] = {0,0};

char pretty_value(char cValue)
/*
   ============================================================================
   Gibt je nach Wert entweder den Wert oder ein Leerzeichen zur�ck
      1. Parameter: Der zu �berpr�fende Wert
      2. R�ckgabewert: Der verarbeitete Wert
   ============================================================================
*/ 
{
   if (cValue > 0)
   {
      return cValue + '0';
   }
   return ' ';
}

void show_help(void)
/*
   ============================================================================
   Zeigt die Hilfestellung an der aktuellen Cursor-Position an
   ============================================================================
*/ 
{
   int i;

   for (i = 0; i < BOUNDARY; i++)
      /*
         Alle m�glichen Werte werden in einer 3x3-Matrix angezeigt, dabei hat
         jeder Wert seine festen Platz:
            123
            456
            789
      */
   {
      int iX, iY;
      char cValue;
      iY = cCursorPos[0] * 4 + 1 + i / 3;
      iX = cCursorPos[1] * 6 + 2 + i % 3;
      cValue = cHelp[cCursorPos[0]][cCursorPos[1]][i];
      mvaddch(iY, iX, pretty_value(cValue));
   }
}

void move_cursor(char cDirection)
/*
   ============================================================================
   Bewegt den Cursor anhand des Parameters innerhalb des Spielfelds
      1. Parameter: Die Richtung, in die der Cursor bewegt werden soll
         (m�gliche Werte: Siehe Headerdatei)
   ============================================================================
*/ 
{
   switch (cDirection)
   {
      case CURS_UP:
         /* Cursor Bewegung aufw�rts */
         cCursorPos[0]--;
         /* Grenzen �berpr�fen und, gegebenenfalls rotieren */
         if (cCursorPos[0] == -1)
         {
            cCursorPos[0] = BOUNDARY - 1;
         }
         break;
         /* Alle weiteren Richtungen analog zu aufw�rts-Bewegung behandeln */
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

void show_ui(char cShowHelp)
/*
   ============================================================================
   Zeigt den aktuellen Status des Spiels an
      1. Parameter: entscheidet, ob die Hilfe am aktuellen Cursor angezeigt
         werden soll
   ============================================================================
*/
{
   int i,j;
   char* cpHelpText;

   move(0,0);
   for (i = 0; i < BOUNDARY; i++)
      /* F�r alle Zeilen */
   {
      if (i % 3 == 0)
         /* Alle 3 Zeilen wird der Trenner angezeigt */
      {
         printw(SEP_OUTER);
      }
      else
         /* Alle anderen Zeilen werden mit dem d�nneren Trenner getrennt */
      {
         printw(SEP_INNER);
      }
      /* Zeile mit vertikalen Trennern ausgeben */
      printw(SEP_VERTI);
      for (j = 0; j < BOUNDARY; j++) {
         /* Trennzeichen in horizontaler Richtung werden analog zu den
         vertikalen angezeigt */
         if (j % 3 == 0)
         {
            printw("@");
         }
         else
         {
            printw("|");
         }
         /* Den eingetragenen Wert mit Abst�nden anzeigen */
         printw("  ");
         printw("%c", pretty_value(cSudoku[i][j]));
         printw("  ");
      }
      printw("@\n");
      /* Zeile mit vertikalen Trennern ausgeben */
      printw(SEP_VERTI);
   }

   /* Untersten Trenner ausgeben */
   printw(SEP_OUTER);

   /* Falls Hilfe aktiviert, zeige Hilfe an */
   if (cShowHelp)
   {
     show_help();
   }

   show_fancy_box(REGELN,0);

   /* Daten auf den Bildschirm, Cursor an die richtige Position bringen */
   refresh();
   wmove(stdscr,cCursorPos[0] * 4 + 2, cCursorPos[1] * 6 + 3);
}

void null_sudoku(void) {
   int i,j;
   for (i = 0; i < 9; i++)
   {
      for (j = 0; j < 9; j++)
      {
         cSudoku[i][j] = 0;
      }
   }
}

void clear_help(int iX, int iY)
/*
   ============================================================================
   L�scht (Setzt 0) alle eingetragenen Hilfestellungen, oder nur die gew�nschte
      1. Parameter: Die gew�nschte x-Koordinate
      2. Parameter: Die gew�nschte y-Koordinate
   ============================================================================
*/
{
   if ((iX == -1 || iX > BOUNDARY) || (iY == -1 || iY < BOUNDARY))
      /* Falls ung�ltige Koordinaten angegeben wurden, l�sche alle Hilfe-
      stellungen */
   {
      int i,j,k;

      for (i = 0; i < BOUNDARY; i++)
      {
         for (j = 0; j < BOUNDARY; j++)
         {
            for (k = 0; k < BOUNDARY; k++)
            {
               cHelp[i][j][k] = 0;
            }
         }
      }
   }
   else
      /* ansonsten l�sche die Hilfestellung an der gew�nschten Stelle */
   {
      int i;

      for (i = 0; i < BOUNDARY; i++)
      {
         cHelp[iY][iX][i] = 0;
      }
   }
}

void set_help(char cHelpData[BOUNDARY][BOUNDARY][BOUNDARY])
/*
   ============================================================================
   �berschreibt die gesamten bisherigen Hilfestellungen mit neuen Eintr�gen
      1. Parameter: Die neuen Hilfestellungen als 9x9x9 Matrix
   ============================================================================
*/
{
   int i,j,k;
   for (i = 0; i < BOUNDARY; i++)
   {
      for (j = 0; j < BOUNDARY; j++)
      {
         for(k = 0; k < BOUNDARY; k++)
         {
            /* F�r jede Zeile, Spalte und Werte, �bernimm neuen Wert */
            cHelp[i][j][k] = cHelpData[i][j][k];
         }
      }
   }
}

void set_help_pos(int iX, int iY, char cValue)
/*
   ============================================================================
   �berschreibt die Hilfestellung f�r die gew�nschte Koordinate
      1. Parameter: Die x-Koordinate
      2. Parameter: Die y-Koordinate
      3. Parameter: Der als Hilfestellung verf�gbare Wert
   ============================================================================
*/
{
   /* Der Wert wird an den Koordinaten an der eigenen Stelle eingef�gt */
   cHelp[iY][iX][cValue] = cValue;
}

void startup_sudoku(void)
/*
   ============================================================================
   Initialisiert die PDcurses Umgebung, zum abfangen der Pfeiltasten und
   besseren Darstellung des Spiels
   ============================================================================
*/
{
   initscr();
   raw();
   keypad(stdscr, TRUE);
   noecho();
   curs_set(2);
}

void shutdown_sudoku(void)
/*
   ============================================================================
   Beendet die PDcurses Umgebung
   ============================================================================
*/
{
   endwin();
}

char get_input(void)
/*
   ============================================================================
   F�ngt Eingaben vom Benutzer ab, eine PDcurses Funktion wird benutzt, um auch
   Sondertasten (bspw. Pfeiltasten) zu erkennen
      1. R�ckgabewert: Der eingegebene Wert
   ============================================================================
*/
{
   return wgetch(stdscr);
}

int getTerminalSize(void) {
/*
   ============================================================================
   �berpr�ft die derzeitige Gr��e des Konsolen-Fensters und gibt diese aus.
      1. R�ckgabewert: Entweder 1 falls Fenster gr��er als 145 x 45 oder 0 bei
         kleinerer Gr��e.
   ============================================================================
*/
   int iSizeY, iSizeX;

   initscr();
   iSizeY = getmaxy(stdscr);
   iSizeX = getmaxx(stdscr);
   endwin();

   if (iSizeY < MIN_HEIGHT || iSizeX < MIN_WIDTH) {
      return 0;
   }
   return 1;
}

void get_cursor_pos(int* iX, int* iY)
/*
   ============================================================================
   Schreibt die aktuelle Position des Cursors in iX und iY
      1. Parameter: Adresse von iX
      2. Parameter: Adresse von iY
      3. R�ckgabewert: -- 
   ============================================================================
*/
{
   *iX = cCursorPos[1];
   *iY = cCursorPos[0];
}
