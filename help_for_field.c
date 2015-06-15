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

#include "generate.h"
#include "help_for_field.h"
#include "tableui.h"
#include "sudoku.h"


void MakeValuesForHelp(int x, int y) {
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
   
   int iZaehler;
   for (iZaehler = 1; iZaehler < 10; iZaehler++) {
      if (can_be_on_coord(x,y,iZaehler,0))
      {
         set_help_pos(x,y,iZaehler);
      }
   }
}