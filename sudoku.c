/* 
   ============================================================================
   Dateiname      : sudoku.c
   Autor          : Tristan Lucas
                    Heinrich-Hertz-Berufskolleg
   Datum          : 07.06.2015
   Thema          : Sudoku
   Version        : 1.0
   Beschreibung   : Implementierung von Basis Sudoku Regeln
   Schnittstellen :
   ============================================================================ 
*/

/* 
   ============================================================================
   Includedateien
   ============================================================================ 
*/

#include "sudoku.h"
#include "tableui.h"

char can_be_in_square(int iX, int iY, char cVal, char cUseRootSudoku)
/*
   ============================================================================
   Prüft, ob die Zahl in das aus tablui.h importierte Sudoku in das an der ge-
   wünschten Position befindliche kleine Quadrat eingefügt werden könnte
      1. Parameter: die gewünschte x-Ordinate
      2. Parameter: die gewünschte x-Ordinate
      3. Parameter: Die gewünschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. Rückgabewert: ob die Zahl eingesetzt werden könnte (1) oder nicht (0)
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int iXStart, iYStart;
   int i, j;

   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cUseRootSudoku) {
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

char can_be_in_line(int iX, int iY, char cVal, char cUseRootSudoku)
/*
   ============================================================================
   Prüft, ob die Zahl in das aus tablui.h importierte Sudoku in eine Reihe
   oder Spalte eingefügt werden könnte
      1. Parameter: die gewünschte x-Ordinate
      2. Parameter: die gewünschte x-Ordinate
      3. Parameter: Die gewünschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. Rückgabewert: ob die Zahl eingesetzt werden könnte (1) oder nicht (0)
   ============================================================================
*/
{
   /* Variablen-Definition und Initialisierung */
   int i;

   char (*cUsedSudoku)[BOUNDARY][BOUNDARY];

   if (cUseRootSudoku) {
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

char can_be_on_coord(int iX, int iY, char cVal, char cUseRootSudoku)
/*
   ============================================================================
   Prüft, ob die gewünschte Zahl an der entsprechende Stelle ohne Regelverstoß
   eingesetzt werden könnte
      1. Parameter: die gewünschte x-Ordinate
      2. Parameter: die gewünschte x-Ordinate
      3. Parameter: Die gewünschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. Rückgabewert: ob die Zahl eingesetzt werden könnte (1) oder nicht (0)
   ============================================================================
*/
{
   return can_be_in_square(iX,iY,cVal,cUseRootSudoku)
          && can_be_in_line(iX,iY,cVal,cUseRootSudoku);
}

