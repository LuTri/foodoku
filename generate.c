/* 
   ============================================================================
   Programmname: generate.h
   Autor       : Tristan Lucas & Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung der Erstellung eines gelösten Sudokus mit Hilfe
                 eines "backtracking"-Algorithmus
   ============================================================================ 
*/

/*
   ============================================================================
   Includedateien
   Man beachte die "defines" in den jeweiligen Dateien
   ============================================================================ 
*/

#include "generate.h"
#include "os.h"
#include "tableui.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/* 
   ============================================================================
   Präprozessor-Anweisungen: globale defines
   ============================================================================ 
*/

#define MAX_RECURSION_STEPS 20000

const int iDifficultyArray[3] = {36, 29 ,22};

/*
   ============================================================================
   Typdefinition für eine Struktur "Koordinate", die nur in dieser Modul
   genutzt wird
   ============================================================================ 
*/

typedef struct {
   int x;
   int y;
} COORDINATE;

char can_be_in_square(int iX, int iY, char cVal, int cStatus)
/*
   ============================================================================
   Prüft, ob die Zahl in das aus tablui.h importierte Sudoku in das an der ge-
   wünschten Position befindliche kleine Quadrat eingefügt werden könnte
      1. Parameter: die gewünschte x-Ordinate
      2. Parameter: die gewünschte x-Ordinate
      3. Parameter: Die gewünschte Zahl
      4. Rückgabewert: ob die Zahl eingesetzt werden könnte (1) oder nicht (0)
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int iXStart, iYStart;
   int i, j;

   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cStatus) {
      cUsedSudoku = &cSudoku;
   } else {
      cUsedSudoku = &cShownSudoku;
   }

   /*
      Das korrekte kleine Quadrat wird berechnet.
      Die beiden Ordinaten haben dabei immer Werte zwischen 0 und 2
   */
   iXStart = iX / BOUNDARY_ROOT;
   iYStart = iY / BOUNDARY_ROOT;

   for (i = 0; i < BOUNDARY_ROOT; i++) {
      /* für alle i in kleiner Kantenlänge */
      for (j = 0; j < BOUNDARY_ROOT; j++) {
         /*
            für alle j in kleiner Kantenlänge
            Es müssen nur die Felder in dem entsprechenden kleinen Quadrat
            geprüft werden
         */
         if ((*cUsedSudoku)[i + (iYStart * BOUNDARY_ROOT)]
                    [j + (iXStart * BOUNDARY_ROOT)] == cVal) {
            /* 
               Wenn einer der Werte im kleinen Quadrat der gewünschten Zahl
               entspricht gib 0 (kann nicht eingesetzt werden) zurück
            */
            return 0;
         }
      }
   }

   /*
      An dieser Stelle wurde die Zahl nicht in dem kleinen Quadrat gefunden, die
      Zahl könnte als eingesetzt werden
   */
   return 1;
}

char can_be_in_line(int iX, int iY, char cVal, int cStatus)
/*
   ============================================================================
   Prüft, ob die Zahl in das aus tablui.h importierte Sudoku in eine Reihe
   oder Spalte eingefügt werden könnte
      1. Parameter: die gewünschte x-Ordinate
      2. Parameter: die gewünschte x-Ordinate
      3. Parameter: Die gewünschte Zahl
      4. Rückgabewert: ob die Zahl eingesetzt werden könnte (1) oder nicht (0)
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int i;

   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cStatus) {
      cUsedSudoku = &cSudoku;
   } else {
      cUsedSudoku = &cShownSudoku;
   }

   for (i = 0; i < BOUNDARY; i++) {
      /* für alle i in großer Kantenlänge */
      if (((*cUsedSudoku)[i][iX] == cVal) || ((*cUsedSudoku)[iY][i] == cVal)) {
         /*
            Wenn die Zahl an irgendeiner Stelle in gleicher Spalte oder gleicher
            Zeile schon vorhanden ist gib 0 (kann nicht eingesetzt werden)
            zurück
         */
         return 0;
      }
   }
   /* Die Zahl wurde in der Spalte und der Zeile nicht gefunden */
   return 1;
}

int possible_positions(char cStatus, char cValue, COORDINATE coordPositions[BOUNDARY_SQUARE])
/*
   ============================================================================
   Berechnet die möglichen Positionen einer Zahl im aus tablui.h importierten
   Sudoku (und die gesamt Anzahl dieser)
      1. Parameter: die gewünschte Zahl
      2. Parameter: ein Array aus Coordinaten für alle Felder
      3. Rückgabewert: Die Anzahl der möglichen Positionen
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int iX,iY;
   int iNumPositions = 0;
   
   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cStatus) {
      cUsedSudoku = &cSudoku;
   } else {
      cUsedSudoku = &cShownSudoku;
   }

   for (iY = 0; iY < BOUNDARY; iY++)
   /* für alle Zeilen */
   {
      for (iX = 0; iX < BOUNDARY; iX++)
      /* für alle Spalten */
      {
         if ((*cUsedSudoku)[iY][iX] == 0
            /* Wenn das Feld leer ist, */
            && can_be_in_square(iX,iY,cValue, cStatus)
            /* die Zahl in das kleine Quadrat eingetraten werden könnte */
            && can_be_in_line(iX,iY,cValue, cStatus))
            /* und die Zahl in die Zeile / Spalte eingetragen werden könnte */
         {

            /* 
               Füge die mögliche Position in das Array ein und erhöhe die
               zurückzugebende Anzahl um 1
            */
            coordPositions[iNumPositions].x = iX;
            coordPositions[iNumPositions].y = iY;
            iNumPositions++;
         }
      }
   }

   /* Rückgabe der Anzahl der möglichen Positionen */
   return iNumPositions;
}

void shift(int iStart, int iEnd, char *cArray)
/*
   ============================================================================
   Rückt den Inhalt eines char-Arrays beliebier Größe ab einer bestimmten
   Position um 1 nach links auf
      1. Parameter: die Start Position
      2. Parameter: die effektive Länge des Arrays (bis hier soll aufgerückt
                    werden)
   ============================================================================
*/
{
   int i;
   for (i = iStart; i < iEnd - 1; i++) {
      /* 
         Für alle Elemente zwischen Start und Ende, kopiere den nächsten Wert
         an die aktuelle Position
      */
      cArray[i] = cArray[i + 1];
   }
}

void randomize_coords(int iLength, COORDINATE coordArray[BOUNDARY_SQUARE])
/*
   ============================================================================
   Mischt eine Liste aus Koordinaten (für mehr "Zufälligkeit")
      1. Parameter: Die effektive Länge, die gemischt werden soll
      2. Parameter: Die Liste aus Koordinaten
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   char cPositions[BOUNDARY_SQUARE];
   char cRandomPositions[BOUNDARY_SQUARE];
   COORDINATE coordRandom[BOUNDARY_SQUARE];
   int iActLength = iLength;
   int i;
   int iPos;

   for (i = 0; i < iLength; i++) {
      /*
         Fühle die Position vor.
         Diese Liste dient als Speicher der noch nicht gemischten Positionen
      */
      cPositions[i] = i;
   }

   for (i = 0; i < iLength; i++)
   {
      /* 
         Berechne die i-te zufällige Position aus und trage diese in die
         zufällige Liste ein
      */
      iPos = rand() % iActLength;
      cRandomPositions[i] = cPositions[iPos];

      /* Lösche die gerade gemischte Position aus der Liste der noch übrigen */
      shift(iPos, iActLength--, cPositions);
   }

   for (i = 0; i < iLength; i++)
   {
      /*
         Bereite das eigentliche Mischen vor, indem die Koordinaten zunächst 
         anhand der gemischten Positionen zwischengespeichert werden
      */
      coordRandom[i].x = coordArray[cRandomPositions[i]].x;
      coordRandom[i].y = coordArray[cRandomPositions[i]].y;
   }

   for (i = 0; i < iLength; i++)
   {
      /* Schreibe die gemischten Koordinaten zurück in die übergebene Liste */
      coordArray[i].x = coordRandom[i].x;
      coordArray[i].y = coordRandom[i].y;
   }
}

char fill_backtrack(int iDepth)
/*
   ============================================================================
   Backtrack-Algorithmus zum erstellen eines gelösten Sudoku
      1. Parameter: Die aktuelle Rekursions-Tiefe
   ============================================================================
*/
 {
   /* Variablen-Definition und Initialisierung */
   static int iCounter = 0;
   COORDINATE coordPossiblePositions[BOUNDARY_SQUARE];
   int iActPos = 0;
   int iX, iY;
   char cSuccess = 0;
   int iNumPossible;
   char cValue;

   /*
      Wenn die Rekursionstiefe der Anzahl aller Felder entspricht wurde das
      gelöste Sudoku erstellt, die Rekursion kann verlassen werden
   */
   if (iDepth == (BOUNDARY_SQUARE)) {
      return 1;
   }

   /*
      Folgender Code - BEKANNTER BUG:
      Dieser Algorithmus hat eine sehr lange worst-case-Laufzeit ( > 5 Minuten )
      Für ein zufriedenstellendes Spielerlebnis wir nach einer definierten
      Anzahl an Versuchen das Erstellen abgebrochen. Bis zu einer zukünftigen
      Version wird dieser Bug untersucht und beseitigt werden.
   */
   iCounter++;
   if (iCounter > MAX_RECURSION_STEPS) {
      iCounter = 0;
      return -1;
   }

   /* Die aktuelle Zahl wird anhand der Rekursions-Tiefe brechnet */
   cValue = (iDepth / BOUNDARY) + 1;

   /* Berechne alle möglichen Positionen für die Zahl und die Anzahl dieser */
   iNumPossible = possible_positions(1, cValue, coordPossiblePositions);

   if (iNumPossible == 0)
   {
      /* 
         Wenn die Anzahl der möglichen Position 0 ist muss das Erstellen
         zurück gesetzt werden
      */
      return 0;
   }

   /* Mische die möglichen Positionen */
   randomize_coords(iNumPossible, coordPossiblePositions);

   while (!cSuccess && iActPos < iNumPossible) {
      /*
         Solange ein der folgenden Rekursionen keinen Erfolg meldet und noch
         Positionen zum Testen vorhanden sind
      */

      /* Trage die Zahl testweise an der aktuellen Position ein */
      iX = coordPossiblePositions[iActPos].x;
      iY = coordPossiblePositions[iActPos].y;
      cSudoku[iY][iX] = cValue;

      /* Teste das Erstellen auf der nächsten Ebene / mit der nächsten Zahl */
      cSuccess = fill_backtrack(iDepth + 1);
      
      if (cSuccess != 1) {
         /*
            Wenn keine der erfolgten Rekursionen einen Erfolg gemeldet hat, dann
            kann die zuletzt eingetragene Zahl nicht richtig eingetragen sein.
            Sie wird wieder gelöscht, die nächste der übrigen Position wird aus-
            probiert
         */
         cSudoku[iY][iX] = 0;
         iActPos++;
      }
   }
   /* Reiche die Erfolgsmeldung an die vorherigen Rekursionen weiter */
   return cSuccess;
}

void create_sudoku(void)
/*
   ============================================================================
   Erstellt ein gelöstes Sudoku
      Keine Parameter / Rückgabewerte
   ============================================================================
*/
{
   /* Initialisiere den Zufallszahlengenerator */
   srand(time(NULL));
   do
   {
      null_sudoku();
      /*
         Wegen des oben genannten, bekannten Bugs:
         Setze das gesamte Spielfeld solange zurück, bis ein fertiges Sudoku
         erstellt wurde
      */
   } while (fill_backtrack(0) != 1);
}

void easy_sudoku(int iDifficulty) {

  int iZaehlerX;
  int iZaehlerY;
  int iZaehlerX2;
  int iZaehlerY2;
  int iZaehler;
  int iAnzahl = 0;
  int iFoo;

  COORDINATE coordPossiblePositions[BOUNDARY_SQUARE];
  COORDINATE coordAllPositions[BOUNDARY_SQUARE];
  
  for (iZaehlerX = 0; iZaehlerX < 9; iZaehlerX++)
  {
     for (iZaehlerY = 0; iZaehlerY < 9; iZaehlerY++)
     {
        cShownSudoku[iZaehlerX][iZaehlerY] = cSudoku[iZaehlerX][iZaehlerY];
     }
  }

  for (iZaehler = 0; iZaehler < BOUNDARY_SQUARE; iZaehler++)
  {
     coordAllPositions[iZaehler].x = iZaehler / 9;
     coordAllPositions[iZaehler].y = iZaehler % 9;
  }

  randomize_coords(BOUNDARY_SQUARE, coordAllPositions);
  iZaehler = 0;
  while (iAnzahl < BOUNDARY_SQUARE - iDifficultyArray[iDifficulty])
  {
  
    cShownSudoku[coordAllPositions[iZaehler].y][coordAllPositions[iZaehler].x] = 0;
    if (!(possible_positions(0, cSudoku[coordAllPositions[iZaehler].y][coordAllPositions[iZaehler].x], coordPossiblePositions) == 1)) {
      cShownSudoku[coordAllPositions[iZaehler].y][coordAllPositions[iZaehler].x] = cSudoku[coordAllPositions[iZaehler].y][coordAllPositions[iZaehler].x];
    } else {
      iAnzahl++;
    }
    iZaehler = ++iZaehler % BOUNDARY_SQUARE;
  }
}
