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
   Pr�ft, ob die Zahl in das aus tablui.h importierte Sudoku in das an der ge-
   w�nschten Position befindliche kleine Quadrat eingef�gt werden k�nnte
      1. Parameter: die gew�nschte x-Ordinate
      2. Parameter: die gew�nschte x-Ordinate
      3. Parameter: Die gew�nschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. R�ckgabewert: ob die Zahl eingesetzt werden k�nnte (1) oder nicht (0)
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
      /* f�r alle i in kleiner Kantenl�nge */
      for (j = 0; j < BOUNDARY_ROOT; j++) {
         /*
            f�r alle j in kleiner Kantenl�nge
            Es m�ssen nur die Felder in dem entsprechenden kleinen Quadrat
            gepr�ft werden
         */
         if ((*cUsedSudoku)[i + (iYStart * BOUNDARY_ROOT)]
                    [j + (iXStart * BOUNDARY_ROOT)] == cVal) {
            /* 
               Wenn einer der Werte im kleinen Quadrat der gew�nschten Zahl
               entspricht gib 0 (kann nicht eingesetzt werden) zur�ck
            */
            return 0;
         }
      }
   }

   /*
      An dieser Stelle wurde die Zahl nicht in dem kleinen Quadrat gefunden, die
      Zahl k�nnte als eingesetzt werden
   */
   return 1;
}

char can_be_in_line(int iX, int iY, char cVal, char cUseRootSudoku)
/*
   ============================================================================
   Pr�ft, ob die Zahl in das aus tablui.h importierte Sudoku in eine Reihe
   oder Spalte eingef�gt werden k�nnte
      1. Parameter: die gew�nschte x-Ordinate
      2. Parameter: die gew�nschte x-Ordinate
      3. Parameter: Die gew�nschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. R�ckgabewert: ob die Zahl eingesetzt werden k�nnte (1) oder nicht (0)
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
      /* f�r alle i in gro�er Kantenl�nge */
      if (((*cUsedSudoku)[i][iX] == cVal) || ((*cUsedSudoku)[iY][i] == cVal)) {
         /*
            Wenn die Zahl an irgendeiner Stelle in gleicher Spalte oder gleicher
            Zeile schon vorhanden ist gib 0 (kann nicht eingesetzt werden)
            zur�ck
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
   Pr�ft, ob die gew�nschte Zahl an der entsprechende Stelle ohne Regelversto�
   eingesetzt werden k�nnte
      1. Parameter: die gew�nschte x-Ordinate
      2. Parameter: die gew�nschte x-Ordinate
      3. Parameter: Die gew�nschte Zahl
      4. Parameter: Status, welches Sudoku-Array genutzt werden soll
      5. R�ckgabewert: ob die Zahl eingesetzt werden k�nnte (1) oder nicht (0)
   ============================================================================
*/
{
   return can_be_in_square(iX,iY,cVal,cUseRootSudoku)
          && can_be_in_line(iX,iY,cVal,cUseRootSudoku);
}

