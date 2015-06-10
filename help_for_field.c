/*
   ========================================================================
   Autor                 : Marian Klemm
   Firma                 : HHBK
   Dateiname             : help_for_field.c
   Datum                 : 10.06.2015
   Beschreibung          : Generierung von validen Zahlen f�r ein Feld im
                           Sudoku als Vorschlag/Hilfe f�r den Nutzer.
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

int * showValuesForHelp(int iCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   Zeigt dem Nutzer eine Auswahl an Zahlen an, die in dem Feld, wo die Hilfe
   angefordert wurde valide sind.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. R�ckgabewert: Ein Array mit validen Zahlen f�r das Feld
   ============================================================================
*/
   
   int iRueckgabe;
   int iZaehler;
   int iPossibleNumbers[9];
   int iZaehler2;

   for (iZaehler = 0; iZaehler <= 9; iZaehler++) {
      iCurrentArray[x][y] = iZaehler;
      iRueckgabe = checkIfOk(iCurrentArray, x, y);
      if (iRueckgabe == 1) {
         iPossibleNumbers[iZaehler] = iZaehler;
       }
    }
    return iPossibleNumbers;
}