/* 
   ============================================================================
   Programmname: generate.h
   Autor       : Tristan Lucas
                 Marian Klemm
                 Heinrich-Hertz-Berufskolleg
   Datum       : 09.06.2015
   Thema       : Sudoku
   Version     : 1.0
   Beschreibung: Modularisierung der Erstellung eines gel�sten Sudokus mit Hilfe
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
#include "system.h"
#include "tableui.h"
#include "sudoku.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/* 
   ============================================================================
   Pr�prozessor-Anweisungen: globale defines
   ============================================================================ 
*/

#define MAX_RECURSION_STEPS 20000
#define DIFF_MAX_EASY 36
#define DIFF_MAX_MEDIUM 29
#define DIFF_MAX_HARD 22

const int iDifficultyArray[3] = {DIFF_MAX_EASY, DIFF_MAX_MEDIUM ,DIFF_MAX_HARD};

/*
   ============================================================================
   Typdefinition f�r eine Struktur "Koordinate", die nur in dieser Modul
   genutzt wird
   ============================================================================ 
*/

typedef struct {
   int x;
   int y;
} COORDINATE;

int possible_positions(char cUseRootSudoku, char cValue, COORDINATE coordPositions[BOUNDARY_SQUARE])
/*
   ============================================================================
   Berechnet die m�glichen Positionen einer Zahl im aus tablui.h importierten
   Sudoku (und die gesamt Anzahl dieser)
      1. Parameter: die gew�nschte Zahl
      2. Parameter: ein Array aus Coordinaten f�r alle Felder
      3. R�ckgabewert: Die Anzahl der m�glichen Positionen
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int iX,iY;
   int iNumPositions = 0;
   
   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cUseRootSudoku) {
      cUsedSudoku = &cSudoku;
   } else {
      cUsedSudoku = &cShownSudoku;
   }

   for (iY = 0; iY < BOUNDARY; iY++)
   /* f�r alle Zeilen */
   {
      for (iX = 0; iX < BOUNDARY; iX++)
      /* f�r alle Spalten */
      {
         if ((*cUsedSudoku)[iY][iX] == 0
            /* Wenn das Feld leer ist, */
            && can_be_on_coord(iX,iY,cValue, cUseRootSudoku))
            /* die Zahl keine Regeln in dieser Stelle keine Regelen verletzt */
         {

            /* 
               F�ge die m�gliche Position in das Array ein und erh�he die
               zur�ckzugebende Anzahl um 1
            */
            coordPositions[iNumPositions].x = iX;
            coordPositions[iNumPositions].y = iY;
            iNumPositions++;
         }
      }
   }

   /* R�ckgabe der Anzahl der m�glichen Positionen */
   return iNumPositions;
}

void shift(int iStart, int iEnd, char *cArray)
/*
   ============================================================================
   R�ckt den Inhalt eines char-Arrays beliebier Gr��e ab einer bestimmten
   Position um 1 nach links auf
      1. Parameter: die Start Position
      2. Parameter: die effektive L�nge des Arrays (bis hier soll aufger�ckt
                    werden)
   ============================================================================
*/
{
   int i;
   for (i = iStart; i < iEnd - 1; i++) {
      /* 
         F�r alle Elemente zwischen Start und Ende, kopiere den n�chsten Wert
         an die aktuelle Position
      */
      cArray[i] = cArray[i + 1];
   }
}

void randomize_coords(int iLength, COORDINATE coordArray[BOUNDARY_SQUARE])
/*
   ============================================================================
   Mischt eine Liste aus Koordinaten (f�r mehr "Zuf�lligkeit")
      1. Parameter: Die effektive L�nge, die gemischt werden soll
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
         F�hle die Position vor.
         Diese Liste dient als Speicher der noch nicht gemischten Positionen
      */
      cPositions[i] = i;
   }

   for (i = 0; i < iLength; i++)
   {
      /* 
         Berechne die i-te zuf�llige Position aus und trage diese in die
         zuf�llige Liste ein
      */
      iPos = rand() % iActLength;
      cRandomPositions[i] = cPositions[iPos];

      /* L�sche die gerade gemischte Position aus der Liste der noch �brigen */
      shift(iPos, iActLength--, cPositions);
   }

   for (i = 0; i < iLength; i++)
   {
      /*
         Bereite das eigentliche Mischen vor, indem die Koordinaten zun�chst 
         anhand der gemischten Positionen zwischengespeichert werden
      */
      coordRandom[i].x = coordArray[cRandomPositions[i]].x;
      coordRandom[i].y = coordArray[cRandomPositions[i]].y;
   }

   for (i = 0; i < iLength; i++)
   {
      /* Schreibe die gemischten Koordinaten zur�ck in die �bergebene Liste */
      coordArray[i].x = coordRandom[i].x;
      coordArray[i].y = coordRandom[i].y;
   }
}

char fill_backtrack(int iDepth)
/*
   ============================================================================
   Backtrack-Algorithmus zum erstellen eines gel�sten Sudoku
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
      gel�ste Sudoku erstellt, die Rekursion kann verlassen werden
   */
   if (iDepth == (BOUNDARY_SQUARE)) {
      return 1;
   }

   /*
      Folgender Code - BEKANNTER BUG:
      Dieser Algorithmus hat eine sehr lange worst-case-Laufzeit ( > 5 Minuten )
      F�r ein zufriedenstellendes Spielerlebnis wir nach einer definierten
      Anzahl an Versuchen das Erstellen abgebrochen. Bis zu einer zuk�nftigen
      Version wird dieser Bug untersucht und beseitigt werden.
   */
   iCounter++;
   if (iCounter > MAX_RECURSION_STEPS) {
      iCounter = 0;
      return -1;
   }

   /* Die aktuelle Zahl wird anhand der Rekursions-Tiefe brechnet */
   cValue = (iDepth / BOUNDARY) + 1;

   /* Berechne alle m�glichen Positionen f�r die Zahl und die Anzahl dieser */
   iNumPossible = possible_positions(1, cValue, coordPossiblePositions);

   if (iNumPossible == 0)
   {
      /* 
         Wenn die Anzahl der m�glichen Position 0 ist muss das Erstellen
         zur�ck gesetzt werden
      */
      return 0;
   }

   /* Mische die m�glichen Positionen */
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

      /* Teste das Erstellen auf der n�chsten Ebene / mit der n�chsten Zahl */
      cSuccess = fill_backtrack(iDepth + 1);
      
      if (cSuccess != 1) {
         /*
            Wenn keine der erfolgten Rekursionen einen Erfolg gemeldet hat, dann
            kann die zuletzt eingetragene Zahl nicht richtig eingetragen sein.
            Sie wird wieder gel�scht, die n�chste der �brigen Position wird aus-
            probiert
         */
         cSudoku[iY][iX] = 0;
         iActPos++;
      }
   }
   /* Reiche die Erfolgsmeldung an die vorherigen Rekursionen weiter */
   return cSuccess;
}

void fill_shown(void)
/*
   ============================================================================
   �bertrag die Eintr�ge aus dem vom Programm erstellen Sudoku in das angezeigte
   Sudoku
      Keine Parameter / R�ckgabewerte
   ============================================================================
*/
{
   int i;
   for (i = 0; i < BOUNDARY_SQUARE; i++)
   {
      cShownSudoku[i/9][i%9] = cSudoku[i/9][i%9];
   }
}

int find_filled(COORDINATE coordFilled[BOUNDARY_SQUARE])
/*
   ============================================================================
   Findet die Koordinaten im angezeigten Sudoku, in denen Eintr�ge vorhanden
   sind (und die Anzahl dieser)
      1. Parameter: Das Array, in welches die Koordinaten eingetragen werden
                    sollen
      2. R�ckgabewert: Die Anzahl der gefunden gef�llten Felder
   ============================================================================
*/
{
   int iNumFilled = 0;
   int i;

   for (i = 0; i < BOUNDARY_SQUARE; i++) {
      /* Im gesamten Sudoku */
      if (cShownSudoku[i/9][i%9] != 0) {
         /* 
            Wenn der Eintrag an der aktuellen Position nicht 0 ist, trage die
            Koordinate in das Array ein
         */
         coordFilled[iNumFilled].x = i % 9;
         coordFilled[iNumFilled++].y = i / 9;
      }
   }

   /* Gib die Anzahl der Koordinaten zur�ck */
   return iNumFilled;
}

char remove_recursive(int iDepth, int iTargetDepth)
/*
   ============================================================================
   Entfernt Eintr�ge aus dem angezeigten Sudoku, bis zu einer bestimmten Anzahl
      1. Parameter: Die aktuelle Rekursionstiefe
      2. Parameter: Die Ziel-Rekurstionstiefe ( = Anzahl der zu l�schenden
         Felder)
   ============================================================================
*/
{
   /* Variablendeklaration und Initialisierung */
   int iNumFilled;
   int iActCoord = 0;
   char cSuccess = 0;
   char cTempValue;

   COORDINATE coordFilled[BOUNDARY_SQUARE];

   /* 
      Wenn die Ziel-Rekursionstiefe erreicht wurde kann der Vorgang beendet 
      werden.
   */
   if (iDepth == iTargetDepth) {
      return 1;
   }

   /* 
      Die noch gef�llten Positionen im Sudoku (Koordinaten) finden und die Liste
      mischen
   */
   iNumFilled = find_filled(coordFilled);
   randomize_coords(iNumFilled,coordFilled);

   /* 
      Nimm die erste Koordinate aus der gemischten Liste und l�sche den Eintrag
      an der entsprechenden Stelle
   */
   cTempValue = cShownSudoku[coordFilled[0].y][coordFilled[0].x];
   cShownSudoku[coordFilled[0].y][coordFilled[0].x] = 0;


   /* 
      F�hre die Rekursion weiter, bis die gew�nschte Anzahl an Eintr�gen
      entfernt wurde
   */
   cSuccess = remove_recursive(iDepth + 1, iTargetDepth);

   return cSuccess;
}

void create_sudoku(int iDifficulty)
/*
   ============================================================================
   Erstellt ein gel�stes Sudoku
      Keine Parameter / R�ckgabewerte
   ============================================================================
*/
{
   /* Initialisiere den Zufallszahlengenerator */
   srand((unsigned int)time(NULL));
   do
   {
      null_sudoku();
      /*
         Wegen des oben genannten, bekannten Bugs:
         Setze das gesamte Spielfeld solange zur�ck, bis ein fertiges Sudoku
         erstellt wurde
      */
   } while (fill_backtrack(0) != 1);

   /* 
      Das angezeigte Sudoku vorbereiten und entsprechend der Schwierigkeit
      Felder aus dem angezeigten Sudoku l�schen
   */
   fill_shown();
   remove_recursive(0, BOUNDARY_SQUARE - iDifficultyArray[iDifficulty]);

   /*
      ANMERKUNG:
      Aufgrund fehlender Rechenleistung und fehlender Algorithmen k�nnen wir
      aktuelle kein eindeutig l�sbares Sudoku erstellen. Das f�hrt dazu, dass
      die Funktion zum automatischen F�llen eines Feldes nicht garantiert
      richtige Ergebnisse liefert.
      Dies wird in einer der n�chsten Versionen behoben
   */
}
