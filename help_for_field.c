<<<<<<< HEAD
/*
   ========================================================================
   Autor                 : Marian Klemm
   Firma                 : HHBK
   Dateiname             : help_for_field.c
   Datum                 : 10.06.2015
   Beschreibung          : Generierung von validen Zahlen für ein Feld im
                           Sudoku als Vorschlag/Hilfe für den Nutzer.
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

int * showValuesForHelp(int iCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   Zeigt dem Nutzer eine Auswahl an Zahlen an, die in dem Feld, wo die Hilfe
   angefordert wurde valide sind.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. Rückgabewert: Ein Array mit validen Zahlen für das Feld
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
=======
/*
   ========================================================================
   Autor                 : Marian Klemm
   Firma                 : HHBK
   Dateiname             : help_for_field.c
   Datum                 : 10.06.2015
   Beschreibung          : Generierung von validen Zahlen für ein Feld im
                           Sudoku als Vorschlag/Hilfe für den Nutzer.
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


///// benutz bitte das globale Array
char * showValuesForHelp(char aCurrentArray[9][9], int x, int y) {
/*
   ============================================================================
   Zeigt dem Nutzer eine Auswahl an Zahlen an, die in dem Feld, wo die Hilfe
   angefordert wurde valide sind.
      1. Parameter: Das Array des Spielfeldes
      2. Parameter: Die X-Koordinate des Feldes
      3. Parameter: Die Y-Koordinate des Feldes
      4. Rückgabewert: Ein Array mit validen Zahlen für das Feld
   ============================================================================
*/
   
   int iRueckgabe;
   int iZaehler;
   char iPossibleNumbers[9];
   int iZaehler2;

   for (iZaehler = 0; iZaehler < 9; iZaehler++) {
      aCurrentArray[x][y] = iZaehler;
      iRueckgabe = checkIfOk(aCurrentArray, x, y);
      if (iRueckgabe == 1) {
         iPossibleNumbers[iZaehler] = iZaehler;
      }
   }
   return iPossibleNumbers;
   ///// keine Arrays zurückgeben!
>>>>>>> a8648ecefca005caf7ba8cce9219169e04ec47ac
}