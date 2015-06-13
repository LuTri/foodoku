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

   Programmschnittstelle: Siehe zugehörige Header-Datei  
   ============================================================================ 
*/

/* 
   ============================================================================
   Includedateien
   ============================================================================ 
*/
#include "os.h"
#include "tableui.h"
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

char cStartSudoku[BOUNDARY][BOUNDARY];

/* 
   ============================================================================
   Für dieses Modul globale Variablen
   ============================================================================ 
*/

char cHelp[BOUNDARY][BOUNDARY][BOUNDARY];

char cCursorPos[2] = {0,0};

SCREEN* sTerminal;

char pretty_value(char cValue)
/*
   ============================================================================
   Gibt je nach Wert entweder den Wert oder ein Leerzeichen zurück
      1. Parameter: Der zu überprüfende Wert
      2. Rückgabewert: Der verarbeitete Wert
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
         Alle möglichen Werte werden in einer 3x3-Matrix angezeigt, dabei hat
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
         (mögliche Werte: Siehe Headerdatei)
   ============================================================================
*/ 
{
   switch (cDirection)
   {
      case CURS_UP:
         /* Cursor Bewegung aufwärts */
         cCursorPos[0]--;
         /* Grenzen überprüfen und, gegebenenfalls rotieren */
         if (cCursorPos[0] == -1)
         {
            cCursorPos[0] = BOUNDARY - 1;
         }
         break;
         /* Alle weiteren Richtungen analog zu aufwärts-Bewegung behandeln */
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

void show_ui(char cShowHelp, char cFinished, char* cHelpText)
/*
   ============================================================================
   Zeigt den aktuellen Status des Spiels an
      1. Parameter: entscheidet, ob die Hilfe am aktuellen Cursor angezeigt
         werden soll
   ============================================================================
*/
{
   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];
   int i,j;
 
   if (cFinished) {
      cUsedSudoku = &cSudoku;
   } else {
      cUsedSudoku = &cShownSudoku;
   }

   move(0,0);
   for (i = 0; i < BOUNDARY; i++)
      /* Für alle Zeilen */
   {
      if (i % 3 == 0)
         /* Alle 3 Zeilen wird der Trenner angezeigt */
      {
         printw(SEP_OUTER);
      }
      else
         /* Alle anderen Zeilen werden mit dem dünneren Trenner getrennt */
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
         /* Den eingetragenen Wert mit Abständen anzeigen */
         printw("  ");
         printw("%c", pretty_value((*cUsedSudoku)[i][j]));
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

   show_fancy_box(cHelpText);

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
   Löscht (Setzt 0) alle eingetragenen Hilfestellungen, oder nur die gewünschte
      1. Parameter: Die gewünschte x-Koordinate
      2. Parameter: Die gewünschte y-Koordinate
   ============================================================================
*/
{
   if ((iX == -1 || iX > BOUNDARY) || (iY == -1 || iY < BOUNDARY))
      /* Falls ungültige Koordinaten angegeben wurden, lösche alle Hilfe-
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
      /* ansonsten lösche die Hilfestellung an der gewünschten Stelle */
   {
      int i;

      for (i = 0; i < BOUNDARY; i++)
      {
         cHelp[iY][iX][i] = 0;
      }
   }
}

void set_help_pos(int iX, int iY, char cValue)
/*
   ============================================================================
   Überschreibt die Hilfestellung für die gewünschte Koordinate
      1. Parameter: Die x-Koordinate
      2. Parameter: Die y-Koordinate
      3. Parameter: Der als Hilfestellung verfügbare Wert
   ============================================================================
*/
{
   /* Der Wert wird an den Koordinaten an der eigenen Stelle eingefügt */
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
   sTerminal = newterm(0,stdin,stdout);
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
   delscreen(sTerminal);
}

char get_input(void)
/*
   ============================================================================
   Fängt Eingaben vom Benutzer ab, eine PDcurses Funktion wird benutzt, um auch
   Sondertasten (bspw. Pfeiltasten) zu erkennen
      1. Rückgabewert: Der eingegebene Wert
   ============================================================================
*/
{
   return wgetch(stdscr);
}

int getTerminalSize(void) {
/*
   ============================================================================
   Überprüft die derzeitige Größe des Konsolen-Fensters und gibt diese aus.
      1. Rückgabewert: Entweder 1 falls Fenster größer als 145 x 45 oder 0 bei
         kleinerer Größe.
   ============================================================================
*/
   int iSizeY, iSizeX;

   sTerminal = newterm(0,stdin,stdout);
   iSizeY = getmaxy(stdscr);
   iSizeX = getmaxx(stdscr);
   endwin();
   delscreen(sTerminal);

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
      3. Rückgabewert: -- 
   ============================================================================
*/
{
   *iX = cCursorPos[1];
   *iY = cCursorPos[0];
}

int calc_errors(void)
{
   int i,j;
   int iErrors = 0;

   for (i = 0; i < BOUNDARY; i++)
   {
      for (j = 0; j < BOUNDARY; j++)
      {
         if (cSudoku[i][j] != cShownSudoku[i][j] && cShownSudoku[i][j] == 0)
         {
            iErrors++;
         }
      }
   }
   return iErrors;
}

int calc_correct_set(void)
{
   int i,j;
   int iCorrect = 0;

   for (i = 0; i < BOUNDARY; i++)
   {
      for (j = 0; j < BOUNDARY; j++)
      {
         if (cSudoku[i][j] == cShownSudoku[i][j]
             && cShownSudoku[i][j] != cStartSudoku[i][j])
         {
            iCorrect++;
         }
      }
   }
   return iCorrect;
}

char show_result(int iHelps, int iFilled, int iSeconds)
{
   char buff[200];
   int iErrors;
   int iCorrect;

   iErrors = calc_errors();
   iCorrect = calc_correct_set();
   show_ui(0, 1, 0);

   sprintf(buff,"Anzahl Fehler:                       %3d", iErrors);
   mvprintw(38,2,buff);
   sprintf(buff,"Anzahl richtig geloest:              %3d", iCorrect);
   mvprintw(39,2,buff);
   sprintf(buff,"Anzahl angezeigzer Kandidaten:       %3d", iHelps);
   mvprintw(40,2,buff);
   sprintf(buff,"Anzahl automatisch gefüllter Felder: %3d", iFilled);
   mvprintw(41,2,buff);
   sprintf(buff,"Benoetigte Zeit                    : %3d", iSeconds);
   mvprintw(42,2,buff);
   refresh();
   get_input();
   return iErrors == 0;   
}

void prepare_start_sudoku()
/*
   ============================================================================
   Erstellt ein Array mit den Werten, die am Start erstellt werden.
      1. Rückgabewert: Der eingegebene Wert
   ============================================================================
*/
{
	int iX;
	int iY;

	 for(iY = 0; iY < BOUNDARY; iY++){
		  for(iX = 0; iX < BOUNDARY; iX++){
			  cStartSudoku[iY][iX] = cShownSudoku[iY][iX];
		  }
	  }
}

int check_input(int iX, int iY)
/*
	============================================================================
	Prüft ob eine Zahl bei der creation vorgeben und somit während des
	Spiels unveränderbar ist.
		1. Rückgabewert: 0 wenn nicht veränderbar, 1 wenn veränderbar
	============================================================================
*/
{
	if(cStartSudoku[iY][iX] != 0){
		return 0;
	}else{
		return 1;
	}
}
