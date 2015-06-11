/*
   ========================================================================
   Autor                 : Marian Klemm
   Firma                 : HHBK
   Dateiname             : sudoku_solving.c
   Datum                 : 09.06.2015
   Beschreibung          : Generierung der Lösung für einzelne Felder im Sudoku.
   Version               : 1.0
   Änderungen            : -
   Compiler              : Visual Studio
   Programmschnittstelle : int main(void)
                           Das Programm gibt den Wert 0 oder 1 zurueck.
                           Das Programm erwartet beim Aufruf keine Argumente
   ========================================================================    
*/

#include <stdio.h>
#include <stdlib.h>
#include "sudoku_solving.h"

char checkIfOk(char cCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   Überprüft anhand des Spielfeld-Arrays, ob die Zahl an Stelle X Y valide ist,
   oder nicht.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. Rückgabewert: 1 falls die Zahl valide ist, 0 wenn nicht
   ============================================================================
*/

   int iX1 = (x / 3) * 3;
   int iY1 = (y / 3) * 3;

   int iZaehlerY;
   int iZaehlerX;
   int iZaehlerQuadratX;
   int iZaehlerQuadratY;

   /* Überprüfung der vertikalen Achse auf doppelten Wert  */
   for (iZaehlerY = 0; iZaehlerY < 9; iZaehlerY++) {
      if (iZaehlerY != y && cCurrentArray[x][y] != 0 && cCurrentArray[x][iZaehlerY]
         == cCurrentArray[x][y]) {
         /* Wenn Wert schon vorkommt, 0 zurückgeben */
         return 0;
      }
   }
   /* Überprüfung der horizontalen Achse auf doppelten Wert */
   for (iZaehlerX = 0; iZaehlerX < 9; iZaehlerX++) {
      if (iZaehlerX != x && cCurrentArray[x][y] != 0 && cCurrentArray[iZaehlerX][y]
         == cCurrentArray[x][y]) {
         /* Wenn Wert schon vorkommt, 0 zurückgeben */
         return 0;
      }
   }
   /* Überprüfung des 3*3 Quadrats auf doppelten Wert */
   for (iZaehlerQuadratX = iX1; iZaehlerQuadratX < iX1 + 3; iZaehlerQuadratX++) {
      for (iZaehlerQuadratY = iY1; iZaehlerQuadratY < iY1 + 3; iZaehlerQuadratY++) {
          if ((iZaehlerQuadratX!=x || iZaehlerQuadratY!=y) && cCurrentArray[x][y]
            != 0 && cCurrentArray[iZaehlerQuadratX][iZaehlerQuadratY]==
            cCurrentArray[x][y]) {
            /* Wenn Wert schon vorkommt, 0 zurückgeben */
            return 0;
          }
      }
   }
   /* Falls kein doppelter Wert vorhanden, 1 zurückgeben */
   return 1;
}

char setValueForField(char cCurrentArray[9][9], char cCompleteArray[9][9], int x, int y) {
/*
   ============================================================================
   Überprüft welche Zahl von 1-9 in das übergebene Feld passt.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. Rückgabewert: Gibt die valide Zahl zurück, die in das Feld passt
                       oder eine 0, wenn keine Zahl in das Feld eingesetzt
                       werden kann. 
   ============================================================================
*/
   int iZaehler;
   char iRueckgabe;
   int iValidNumbersCounter;
   int iValidNumbers[9];

   /* Zahlen von 1-9 durchtesten und die erste Zahl setzen, die funktioniert */
   for (iZaehler = 0; iZaehler <= 9; iZaehler++) {
      cCurrentArray[x][y] = iZaehler;
      iRueckgabe = checkIfOk(cCurrentArray, x, y);

      if(iRueckgabe == 1) {
         /* Wenn getestete Zahl für das Feld valide ist,
            Zahl dem Array hinzufügen und iValidNumbers
            hochzählen */
         iValidNumbers[iValidNumbersCounter] = iZaehler;
         iValidNumbersCounter++;
      }
   }

   if (iValidNumbersCounter > 1) {
      /* Richtige Zahl aus komplettem Spiel-Array holen und zurückgeben */
      return cCompleteArray[x][y];
   } else if(iValidNumbersCounter == 1) {
      /* Es gibt nur eine Zahl die passt, diese nun zurückgeben. */
      return iValidNumbers[0];
   }

   /* Wenn kein Wert funktioniert, dann 0 zurückgeben */
   return 0;


   ///// Wie besprochen, funktioniert nicht
}