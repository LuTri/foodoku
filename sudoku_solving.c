/*
   ========================================================================
   Autor                 : Marian Klemm
   Firma                 : HHBK
   Dateiname             : sudoku_solving.c
   Datum                 : 09.06.2015
   Beschreibung          : Generierung der L�sung f�r einzelne Felder im Sudoku.
   Version               : 1.0
   �nderungen            : -
   Compiler              : Visual Studio
   Programmschnittstelle : int main(void)
                           Das Programm gibt den Wert 0 oder 1 zurueck.
                           Das Programm erwartet beim Aufruf keine Argumente
   ========================================================================    
*/

#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solving.h"

int checkIfOk(int iCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   �berpr�ft anhand des Spielfeld-Arrays, ob die Zahl an Stelle X Y valide ist,
   oder nicht.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. R�ckgabewert: 1 falls die Zahl valide ist, 0 wenn nicht
   ============================================================================
*/

   int iX1 = (x / 3) * 3;
   int iY1 = (y / 3) * 3;

   int iZaehlerY;
   int iZaehlerX;
   int iZaehlerQuadratX;
   int iZaehlerQuadratY;

   /* �berpr�fung der vertikalen Achse auf doppelten Wert  */
   for (iZaehlerY = 0; iZaehlerY < 9; iZaehlerY++) {
      if (iZaehlerY != y && iCurrentArray[x][y] != 0 && iCurrentArray[x][iZaehlerY]
         == iCurrentArray[x][y]) {
         /* Wenn Wert schon vorkommt, 0 zur�ckgeben */
         return 0;
      }
   }
   /* �berpr�fung der horizontalen Achse auf doppelten Wert */
   for (iZaehlerX = 0; iZaehlerX < 9; iZaehlerX++) {
      if (iZaehlerX != x && iCurrentArray[x][y] != 0 && iCurrentArray[iZaehlerX][y]
         == iCurrentArray[x][y]) {
         /* Wenn Wert schon vorkommt, 0 zur�ckgeben */
         return 0;
      }
   }
   /* �berpr�fung des 3*3 Quadrats auf doppelten Wert */
   for (iZaehlerQuadratX = iX1; iZaehlerQuadratX < iX1 + 3; iZaehlerQuadratX++) {
      for (iZaehlerQuadratY = iY1; iZaehlerQuadratY < iY1 + 3; iZaehlerQuadratY++) {
          if ((iZaehlerQuadratX!=x || iZaehlerQuadratY!=y) && iCurrentArray[x][y]
            != 0 && iCurrentArray[iZaehlerQuadratX][iZaehlerQuadratY]==
            iCurrentArray[x][y]) {
            /* Wenn Wert schon vorkommt, 0 zur�ckgeben */
            return 0;
          }
      }
   }
   /* Falls kein doppelter Wert vorhanden, 1 zur�ckgeben */
   return 1;
}

int setValueForField(int iCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   �berpr�ft welche Zahl von 1-9 in das �bergebene Feld passt.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. R�ckgabewert: Gibt die valide Zahl zur�ck, die in das Feld passt
                       oder eine 0, wenn keine Zahl in das Feld eingesetzt
                       werden kann. 
   ============================================================================
*/
   int iZaehler;
   int iRueckgabe;

   /* Zahlen von 1-9 durchtesten und die erste Zahl setzen, die funktioniert */

   ////// ANMERKUNG: Das wird nicht funktionieren - nur wenn genau nur eine Zahl passt kannst du die einsetzen
   ////// ODER: benutz Backtracking
   for (iZaehler = 0; iZaehler <= 9; iZaehler++) {
      iCurrentArray[x][y] = iZaehler;
      iRueckgabe = checkIfOk(iCurrentArray, x, y);

      if(iRueckgabe == 1) {
         /* Wenn Wert im Feld valide ist, dann diesen zur�ckgeben */
         return iZaehler;
      }
   }
   /* Wenn kein Wert funktioniert, dann 0 zur�ckgeben */
   return 0;
}